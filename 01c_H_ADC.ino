#include <Adafruit_ADS1015.h>
Adafruit_ADS1015 adc(0x48);



bool init_adc = true;

double filter[128];
int i_filter, f_scale_ADC = 4;

float temp_val;


void Loop_ADC() {

  Wire.beginTransmission (0x48);

  if (Wire.endTransmission () == 0) {

    if (init_adc == true) {               // initialize adxl after first response
      display.setTextAlignment(TEXT_ALIGN_CENTER);
      display.drawString(64, 30, "ADC sensor connected");
      display.display();
      display.setTextAlignment(TEXT_ALIGN_LEFT);
      delay(1000);
      display.clear();

      adc.begin();
      //adc.startContinuous_Differential_0_1();
      adc.startContinuous_SingleEnded(0);
      // adc.setGain(GAIN_TWOTHIRDS);  // 2/3x gain +/- 6.144V  1 bit = 3mV      0.1875mV (default)
      adc.setGain(GAIN_ONE);        // 1x gain   +/- 4.096V  1 bit = 2mV      0.125mV
      // adc.setGain(GAIN_TWO);        // 2x gain   +/- 2.048V  1 bit = 1mV      0.0625mV
      // adc.setGain(GAIN_FOUR);       // 4x gain   +/- 1.024V  1 bit = 0.5mV    0.03125mV
      // adc.setGain(GAIN_EIGHT);      // 8x gain   +/- 0.512V  1 bit = 0.25mV   0.015625mV
      // adc.setGain(GAIN_SIXTEEN);    // 16x gain  +/- 0.256V  1 bit = 0.125mV  0.0078125mV

      adc.setSPS(ADS1015_DR_3300SPS); // for ADS1115 fastest samples per second is 860 (default is 128)

      init_adc = false ;
      ADC_Tool = true;
    }



    for (int i = 0; i < SAMPLES; i++) {
      newTime = micros();
      //vReal[i]=adc.readADC_Differential_0_1(); //read the ADC and store the data in vReal
      vReal[i] = adc.readADC_SingleEnded(0); //read the ADC and store the data in vReal
      vImag[i] = 0;
      dTime[i] = micros();
      while (micros() < (newTime + sampling_period_us)) {
        /* do nothing to wait */
      }
    }//End of getting data loop

    temp_val = vReal[10];

    median = 0;
    for (int i = 0; i < SAMPLES; i++) {  // calculate median
      median = median + vReal[i]; // add up all values
    }
    median = median / SAMPLES; //devide added values through numbers of samples
    for (int i = 0; i < SAMPLES; i++) {  // eleminate median
      vReal[i] = vReal[i] - median; //
    }


    FFT.Windowing(vReal, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
    FFT.Compute(vReal, vImag, SAMPLES, FFT_FORWARD);
    FFT.ComplexToMagnitude(vReal, vImag, SAMPLES);
    double peak = FFT.MajorPeak(vReal, SAMPLES, SAMPLING_FREQUENCY);
    fres = (float)SAMPLING_FREQUENCY / SAMPLES;


    if (true) { //Plotting the FFT
      max_peak = 1;
      for (int i = 2; i < 128 * f_scale_ADC; i++) { //find maximum value for scaling
        if (vReal[i] > max_peak) {
          max_peak = vReal[i];
        }
      }
      for (int i = 1; i < 129 ; i++) { // Don't use sample 0 and only first SAMPLES/2 are usable. Each array eleement represents a frequency and its value the amplitude.
        fReal[i] = fres * i * f_scale_ADC;
        display.drawLine(i , 54 - amplitude * vReal[i * f_scale_ADC] / (max_peak), (i + 1) , 54 - amplitude * vReal[(i + 1)*f_scale_ADC] / (max_peak));
        display.drawString(0, 0, "a");
        //Serial.println(vReal[i], 1);    //View only this line in serial plotter to visualize the bins
      }
    }

    for (int i = 1; i < 8 ; i++) {    // draw the x-axis
      //display.drawVerticalLine(16 * i, 13, 3);
      mystring = String((int)fReal[i * 16]);
      display.setTextAlignment(TEXT_ALIGN_CENTER);
      display.drawString(16 * i, 54, mystring);
    }

    mystring = String(peak);
    display.drawString(40, 20, mystring);

    mystring = String(1000000.0 / (dTime[10] - dTime[9]));
    display.drawString(40, 10, mystring);

    mystring = String(temp_val);
    display.drawString(80, 10, mystring);
    display.setTextAlignment(TEXT_ALIGN_LEFT);
  }



  //if (Wire.endTransmission () != 0) {
  else {
    init_adc = true ;
    ADC_Tool = false;
  }





}// end of Loop_ADC
