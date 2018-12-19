/// for FFT etc /////////////////////////////////////////////////////////

#include <SparkFun_ADXL345.h>
ADXL345 adxl = ADXL345();             // https://github.com/sparkfun/SparkFun_ADXL345_Arduino_Library

#include "arduinoFFT.h" // Standard Arduino FFT library
// https://github.com/kosme/arduinoFFT, in IDE, Sketch, Include Library, Manage Library, then search for FFT
arduinoFFT FFT = arduinoFFT();

#define SAMPLING_FREQUENCY 512 //Hz, must be 900 or less due to ADC conversion time. Determines maximum frequency that can be analysed by the FFT.
#define SAMPLES 1024              //Must be a power of 2
#define amplitude 45  //max amplitude in Pixel
#define dmg_width 128 //maximum width of bearing detection bars
//#define DEVICE (0x53)      //ADXL345 device address
//#define DEVICE (0x3A)      //ADXL345 alternative device address
bool init_adxl = true;


unsigned int sampling_period_us, counter = 0;
unsigned long microseconds;
int x, y, z; //acceleration values of all axis


double vReal[SAMPLES];
double vImag[SAMPLES];
float  fReal[SAMPLES / 2];
double median;
float fres; //resolution of frequency spectrum
int    dTime[SAMPLES];
unsigned long newTime = 0, oldTime = 0;



double   dmg_inner, dmg_outer, dmg_ball, dmg_max;
int dmg_temp;


void Loop_ACC() {

  Wire.beginTransmission (0x53);

  if (Wire.endTransmission () == 0) {

    if (init_adxl == true) {               // initialize adxl after first response
      display.init();
      display.setFont(ArialMT_Plain_10);
      display.flipScreenVertically(); // Adjust to suit or remove
      adxl.powerOn();                     // Power on the ADXL345

      adxl.setRangeSetting(2);           // Give the range settings
                                         // Accepted values are 2g, 4g, 8g or 16g
                                         // Higher Values = Wider Measurement Range
                                         // Lower Values = Greater Sensitivity
      adxl.setRate(3200);                // set Sampling Rate of Sensor
                                         // allowed: 6.25 , 12.5 , 25 , 50 , 100 , 200 , 400 , 800 , 1600 , 3200 
      sampling_period_us = round(1000000 * (1.0 / SAMPLING_FREQUENCY));

      display.setTextAlignment(TEXT_ALIGN_CENTER);
      display.drawString(64, 30, "acc. sensor connected");
      display.display();
      display.setTextAlignment(TEXT_ALIGN_LEFT);
      delay(1000);
      display.clear();

      init_adxl = false ;
      FFT_Tool = true;
    }
    if (FFT_mode == 2) {
      acc_measurand = 1;
    }

    if (FFT_mode == 1 || FFT_mode == 2 || FFT_mode == 4) { //sampling data for FFT

      if (FFT_mode == 4) // generate some example data for an FFT
      {
        for (int i = 0; i < SAMPLES; i++) {   //function generator for test
          //vReal[i] = (float)500 * sin(2 * 3.14 * 32.7 * i / SAMPLING_FREQUENCY) + 1000 * sin(2 * 3.14 * 24.39 * i / SAMPLING_FREQUENCY); //
          vReal[i] = (float)500 * sin(2 * 3.14 * 15 * i / SAMPLING_FREQUENCY) + 1000 * sin(2 * 3.14 * 25 * i / SAMPLING_FREQUENCY);
          vImag[i] = 0;
        }
      }

      if (FFT_mode != 4) {
        i_trigger_frequency = 0;
        for (int i = 0; i < SAMPLES; i++) {
          newTime = micros();
          adxl.readAccel(&x, &y, &z);         // Read the accelerometer values and store them in variables decl
          if (acc_measurement == 1) { // set direction of measurement
            vReal[i] = x;
          } //
          if (acc_measurement == 2) { // set direction of measurement
            vReal[i] = y;
          } //
          if (acc_measurement == 3) { // set direction of measurement
            vReal[i] = z;
          } //
          //dTime[i] = micros();
          //yield();
          while (micros() < (newTime + sampling_period_us)) {
            /* do nothing to wait */
          }
        }//End of getting data loop

        for (int i = 0; i < SAMPLES; i++) {
          vImag[i] = 0;
        }

        if (Trigger_Tool == true) { //getting the rotary freqency
          i_trigger_frequency = 0;
          trigger_temp = micros();
          while (i_trigger_frequency < 2) { //looking for one rotation
            trigger_signal = trigger.read(3);
            if (trigger_signal == 0) { // if an object is detected
              if (trigger_old == false) {
                trigger_frequency = 1000000.0 / (micros() - trigger_temp);
                trigger_temp = micros();
                i_trigger_frequency++;
              }//End of rising edge Loop
              trigger_old = true;
            }//End of Object detected Loop
            if (trigger_signal == 1) { // no object is detected
              trigger_old = false;
            }//End of no Object Loop
            if (micros() > trigger_temp + 1000000) { //cancel Loop if no Object can be found
              i_trigger_frequency = 10;
              trigger_frequency = 0;
            }//End of Cancel Loop
          }//End of rotary Loop
          f_rot = trigger_frequency;
        }//End of Trigger loop

        if (false) { //calculate the median of the sampling time
          dTime[1023] = 0;
          for (int i = 0 ; i < SAMPLES - 2 ; i++) {
            dTime[1023] = dTime[1023] + (dTime[i + 1] - dTime[i]);
          }
          dTime[1023] = dTime[1023] / (SAMPLES - 2);
        }

        median = 0;
        for (int i = 0; i < SAMPLES; i++) {  // calculate median
          median = median + vReal[i]; // add up all values
        }
        median = median / SAMPLES; //devide added values through numbers of samples
        for (int i = 0; i < SAMPLES; i++) {  // eleminate median
          vReal[i] = vReal[i] - median; //
        }
      }





      FFT.Windowing(vReal, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
      FFT.Compute(vReal, vImag, SAMPLES, FFT_FORWARD);
      FFT.ComplexToMagnitude(vReal, vImag, SAMPLES);
      peak = FFT.MajorPeak(vReal, SAMPLES, SAMPLING_FREQUENCY);
      fres = (float)SAMPLING_FREQUENCY / SAMPLES;
      f_scale = freq_max / 64;

      if (FFT_mode == 1 || FFT_mode == 4) { //Plotting the FFT
        if (acc_measurand == 1) { //plotting the acceleration FFT
          max_peak = 1000;
          for (int i = 2; i < 128 * f_scale; i++) { //find maximum value for scaling
            if (vReal[i] > max_peak) {
              max_peak = vReal[i];
            }
          }
          for (int i = 1; i < 129 ; i++) { // Don't use sample 0 and only first SAMPLES/2 are usable. Each array eleement represents a frequency and its value the amplitude.
            fReal[i] = fres * i * f_scale;
            display.drawLine(i , 54 - amplitude * vReal[i * f_scale] / (max_peak), (i + 1) , 54 - amplitude * vReal[(i + 1)*f_scale] / (max_peak));
          }//end of for loop
          display.drawString(0, 0, "a");
          //mystring=adxl.getRate();    //display the sampling rate
          //display.drawString(10, 0, mystring);
        }//end of acc_measurand == 1

        plotWebsite = "<svg height='300' width='384'><polyline points='";
        for (int i = 1; i < 129 ; i++) { //draw the lines of the FFT on the webpage
          plotWebsite += i * 3;
          plotWebsite += ",";
          plotWebsite += 256 - amplitude * vReal[i * f_scale] / (max_peak / 4);
          plotWebsite += " ";
        }
        plotWebsite += "' style='fill:none;stroke:#4CAF50;stroke-width:1' ></polyline>  Sorry, your browser does not support inline SVG.";
        plotWebsite += "<polygon points='0,0 0,260 48,260 48,40 48,260 96,260 96,40 96,260 144,260 144,40 144,260 192,260 192,40 192, 260 240,260 240,40 240,260 288,260 288,40 288,260 336,260 336,40 336,260 384,260 384,0' style='fill:none;stroke:gray;stroke-width:0.5'></polygon>";
        for (int i = 1; i <= 7 ; i++) { //draw the text for the frequencies
          plotWebsite += "<text x='";
          plotWebsite += 48 * i - 10;
          plotWebsite += "' y='280' fill='gray'>";
          plotWebsite += i * 8 * f_scale;
          plotWebsite += " </text>";
        }
        plotWebsite += "<text x='05' y='20' fill='gray'>max frequency: ";
        plotWebsite += peak;
        plotWebsite += " Hz </text>";

        plotWebsite += "</svg> ";
//        PlotPage = plotWebsite;
//        server.send(200, "text/plane", PlotPage); //Send web page
//        server.send(200, "text/plane", ledState); //Send web page
        
        if (datalog) { //logging of the data
          File dataFile = SD.open(filetowrite, FILE_APPEND);
          // if the file is available, write to it:
          if (dataFile) {
            datalogging = true;
            mystring ="";
            mystring += (int)millis() - datalog_millis;
            mystring += ",";
            mystring += (float)peak;
            mystring += ",";
            mystring += f_rot;
            if(dataNewComment == true){
              mystring += ",";
              mystring += dataComment;
              dataNewComment = false;
            }
            mystring += ";";
            dataFile.print(mystring);
            mystring ="";
            dataFile.println("");
            for (int i = 1; i <= 512 ; i++) {
              mystring += (int)vReal[i];
              mystring += ",";
            }
            mystring += ";";
            dataFile.print(mystring);
            mystring ="";
            dataFile.println("");
            dataFile.close();
            display.drawString(120, 10, "#");
          } else {
            display.drawString(20, 10, "datalog not possible");
            datalogging = false;
          }
        }


        if (acc_measurand == 2) { //plotting the displacement FFT
          for (int i = 3; i < 131; i++) { // Don't use sample 0 and only first SAMPLES/2 are usable. Each array eleement represents a frequency and its value the amplitude.
            fReal[i] = fres * i;
            vReal[i] = (float)vReal[i] / (2 * 3.14 * fReal[i] * 2 * 3.14 * fReal[i]);
            //display.drawLine(i, 54 - vReal[i] / (4 * amplitude), i+1 , 54 - vReal[i+1] / (4 * amplitude));
            //Serial.println(vReal[i], 1);    //View only this line in serial plotter to visualize the bins
          }
          max_peak = 5;
          vReal[1] = vReal[2] = 0;
          for (int i = 1; i < 128 * f_scale; i++) { //find maximum value for scaling
            if (vReal[i] > max_peak) {
              max_peak = vReal[i];
            }
          }
          for (int i = 1; i < 129; i++) {
            display.drawLine(i, 54 - amplitude * vReal[i * f_scale] / (max_peak), i + 1 , 54 - amplitude * vReal[(i + 1)*f_scale] / (max_peak));
            display.drawString(0, 0, "s");
          }
        }

        if (1) {  // display the bearing diagnostics
          display.drawVerticalLine((int)f_rot * on_inner / (f_scale * fres), 13, 3);
          display.drawVerticalLine((int)f_rot * on_outer / (f_scale * fres), 13, 3);
          display.drawVerticalLine((int)f_rot * on_ball / (f_scale * fres), 13, 3);
        }

        // mystring=String(1000000.0/dTime[1023]);
        // display.drawString(15,0,mystring);
        //   mystring=String(i_trigger_frequency);
        //   display.drawString(40,20,mystring);
        //   mystring=String(f_rot*on_ball);
        //   display.drawString(15,30,mystring);



        for (int i = 1; i < 8 ; i++) {    // draw the x-axis
          //display.drawVerticalLine(16 * i, 13, 3);
          mystring = String((int)fReal[i * 16]);
          display.setTextAlignment(TEXT_ALIGN_CENTER);
          display.drawString(16 * i, 54, mystring);
        }

        display.setTextAlignment(TEXT_ALIGN_RIGHT);//Plotting of the measurement direction

        mystring = String(peak);
        display.drawString(120, 0, mystring);
        if (Trigger_Tool == true) {
          mystring = String(f_rot);
          display.drawString(80, 0, mystring);
        }//End of Display-f_rot-Loop

        //  i_ISR_getdata = 0;
      }//End of FFT_mode=1 Loop

      if (FFT_mode == 2) { //Display the bearing detection bars
        dmg_temp = round(f_rot * on_inner * 2);
        dmg_inner = round(vReal[dmg_temp - 1] + vReal[dmg_temp] + vReal[dmg_temp + 1]);

        dmg_temp = round(f_rot * on_outer * 2);
        dmg_outer = round(vReal[dmg_temp - 1] + vReal[dmg_temp] + vReal[dmg_temp + 1]);

        dmg_temp = round(f_rot * on_ball * 2);
        dmg_ball = round(vReal[dmg_temp - 1] + vReal[dmg_temp] + vReal[dmg_temp + 1]);

        max_peak = 100000; //setting up the maximum peak

        if (dmg_inner > max_peak) {
          max_peak = dmg_inner;
        }
        if (dmg_outer > max_peak) {
          max_peak = dmg_outer;
        }
        if (dmg_ball > max_peak) {
          max_peak = dmg_outer;
        }

        //fillRect(int16_t x, int16_t y, int16_t width, int16_t height);
        display.drawString(0, 0, "inner ring");
        mystring = String (f_rot * on_inner);
        display.drawString(60, 0, mystring);
        //display.fillRect(0, 12 , (dmg_inner/max_peak)*dmg_width , 8);
        display.drawProgressBar(0, 12, 127, 8 , dmg_inner / max_peak * 100);
        display.drawString(0, 20, "outer Ring");
        mystring = String (f_rot * on_outer);
        display.drawString(60, 20, mystring);
        display.drawProgressBar(0, 32, 127, 8 , dmg_outer / max_peak * 100);
        display.drawString(0, 40, "ball");
        mystring = String (f_rot * on_ball);
        display.drawString(60, 40, mystring);
        display.drawProgressBar(0, 52, 127, 8 , dmg_ball / max_peak * 100);

        display.setTextAlignment(TEXT_ALIGN_RIGHT);//Plotting of the measurement direction
        mystring = String(peak);
        display.drawString(128, 40, mystring);
        if (true) {
          mystring = String(f_rot);
          display.drawString(128, 20, mystring);
        }//End of Display-f_rot-Loop


      }

      if (acc_measurement == 1) {
        display.drawString(128, 0, "x");
      } //
      if (acc_measurement == 2) {
        display.drawString(128, 0, "y");
      } //
      if (acc_measurement == 3) {
        display.drawString(128, 0, "z");
      } //
      display.setTextAlignment(TEXT_ALIGN_LEFT);
    }//END of FFT data Loop (FFT_mode 1 and FFT_mode 2)

    if (FFT_mode == 3) { //Display the raw data
      adxl.readAccel(&x, &y, &z);         // Read the accelerometer values and store them in variables decl
      x = (int) x * 3.9;
      y = (int) y * 3.9;
      z = (int) z * 3.9;


      display.setTextAlignment(TEXT_ALIGN_RIGHT);//Plotting of the measurement direction
      mystring = String(x);
      display.drawString(15, 20, "x=");
      display.drawString(40, 20, mystring);
      mystring = String(y);
      display.drawString(15, 30, "y=");
      display.drawString(40, 30, mystring);
      mystring = String(z);
      display.drawString(15, 40, "z=");
      display.drawString(40, 40, mystring);


      display.setTextAlignment(TEXT_ALIGN_LEFT);
    }



  }
  //if (Wire.endTransmission () != 0) {
  else {
    init_adxl = true ;
    FFT_Tool = false;
    plotWebsite = "";
  }
}//end of Loop_ACC
