


#include <mlx90615.h> //https://github.com/skiselev/MLX90615

MLX90615 thermo = MLX90615();


bool init_thermo = true;

float thermo_signal_amb = 0;
float thermo_signal_obj = 0;


void Loop_Thermo() {

  Wire.beginTransmission (0x5B);

  if (Wire.endTransmission () == 0) {

    if (init_thermo == true) {               // initialize adxl after first response

      display.setTextAlignment(TEXT_ALIGN_CENTER);
      display.drawString(64, 30, "Thermo connected");
      display.display();
      display.setTextAlignment(TEXT_ALIGN_LEFT);
      delay(1000);
      display.clear();
      thermo.begin();
      init_thermo = false ;
      Thermo_Tool = true ;
      i_display_buffer = 0;

      for (int i = 0; i <= 127 ; i++) { // deleting stored values in array
        display_buffer[i] = 0;
      }
    }

        
    // read Thermo Value of ambient temperature
    thermo_signal_amb = thermo.get_ambient_temp();
    mystring = "A: ";
    mystring += String(thermo_signal_amb);
    mystring += "°C";
    display.drawString(0, 0, mystring);

    // read Thermo Value of object temperature
    thermo_signal_obj = thermo.get_object_temp();
    mystring = "O: ";
    mystring += String(thermo_signal_obj);
    mystring += "°C";
    display.drawString(64, 0, mystring);

    // fo plotting the data the value of the object temperature is stored in an array every second
    if (millis() - lastcall > 1000) { // getting data for plotting the trend
      display_buffer[i_display_buffer] = thermo_signal_obj; // storing the actual temperature in a ring buffer
      i_display_buffer++;
      if (i_display_buffer >= 127) {
        i_display_buffer = 0;
      }
      lastcall = millis();
    }

    for (int i = 1; i <= 128 ; i++) { // plotting the trend   "i" represents the pixel to draw
      temp = i + i_display_buffer;
      if (temp > 127) {
        temp = temp - 128;
      }
      if (temp == 0) { // avoiding negative indizes when plotting the line at the array startover
        display.drawLine(i , 64 - display_buffer[127] , i + 1 , 64 - display_buffer[temp] );
      } else    // normal draw routine
        display.drawLine(i , 64 - display_buffer[temp - 1] , i + 1 , 64 - display_buffer[temp] );
    }

    plotWebsite = "<svg height='300' width='384'><polyline points='";
    for (int i = 1; i <= 128 ; i++) { //draw the lines of the FFT on the webpage
      temp = i + i_display_buffer;
      if (temp > 127) {
        temp = temp - 128;
      }
      plotWebsite += i * 3;
      plotWebsite += ",";
      if (temp == 0) {
        plotWebsite += 256 - (display_buffer[temp] - min_display) / (scale_display / 4);
      } else
        plotWebsite += 256 - (display_buffer[temp - 1] - min_display) / (scale_display / 4);
      plotWebsite += " ";
    }
    plotWebsite += "' style='fill:none;stroke:#4CAF50;stroke-width:1' />  Sorry, your browser does not support inline SVG.";
    plotWebsite += "<text x='05' y='15' fill='gray'>current reading: ";
    plotWebsite += thermo_signal_obj;
    plotWebsite += " °C </text>";

    plotWebsite += "</svg> ";

    

  }// END OF THERMO LOOP


  //if (Wire.endTransmission () != 0) {
  else {
    init_thermo = true ;
    Thermo_Tool = false;
  }

}
