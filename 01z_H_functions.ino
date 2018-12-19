//===============================================================
// This routine is executed when you open its IP in browser
//===============================================================
void handleRoot() {
  String s = MAIN_page; //Read HTML contents
  server.send(200, "text/html", s); //Send web page
}

void handlePlot() {
  String PlotPage = plotWebsite;
  server.send(200, "text/plane", PlotPage); //Send web page
}

void handleString() {
  if (server.arg("DataFileName") == "") {   //Parameter not found
  } else {    //Parameter found
    datafile = server.arg("DataFileName"); //Refer  xhttp.open("GET", "setLED?LEDstate="+led, true);
    filetowrite = "/";                        // generating the file name
    filetowrite += datafile += "\0";
    filetowrite += ".CSV";
    server.send(200, "text/plane", datafile); //Send web page
  }
  if (server.arg("DataFileComment") == "") {   //Parameter not found
  } else {    //Parameter found
    dataComment = server.arg("DataFileComment"); //Refer  xhttp.open("GET", "setLED?LEDstate="+led, true);
    dataNewComment = true;
    server.send(200, "text/plane", dataComment); //Send web page
  }
  if (server.arg("FFT_O_Inner") == "") {   //Parameter not found
  } else {    //Parameter found
    on_inner = server.arg("FFT_O_Inner").toFloat(); //Refer  xhttp.open("GET", "setLED?LEDstate="+led, true);
    server.send(200, "text/plane", ""); //Send web page
  }
  if (server.arg("FFT_O_Outer") == "") {   //Parameter not found
  } else {    //Parameter found
    on_outer = server.arg("FFT_O_Outer").toFloat(); //Refer  xhttp.open("GET", "setLED?LEDstate="+led, true);
    server.send(200, "text/plane", ""); //Send web page
  }
  if (server.arg("FFT_O_Roller") == "") {   //Parameter not found
  } else {    //Parameter found
    on_ball = server.arg("FFT_O_Roller").toFloat(); //Refer  xhttp.open("GET", "setLED?LEDstate="+led, true);
    server.send(200, "text/plane", ""); //Send web page
  }
  if (server.arg("FFT_F_Rot") == "") {   //Parameter not found
  } else {    //Parameter found
    f_rot = server.arg("FFT_F_Rot").toFloat(); //Refer  xhttp.open("GET", "setLED?LEDstate="+led, true);
    server.send(200, "text/plane", ""); //Send web page
  }
  if (server.arg("Trigger_Dia") == "") {   //Parameter not found
  } else {    //Parameter found
    trigger_dia = server.arg("Trigger_Dia").toFloat(); //Refer  xhttp.open("GET", "setLED?LEDstate="+led, true);
    server.send(200, "text/plane", ""); //Send web page
  }
  if (server.arg("TOF_MaxDisplay") == "") {   //Parameter not found
  } else {    //Parameter found
    max_display = server.arg("TOF_MaxDisplay").toInt();     //Gets the value of the query parameter
    scale_display = (max_display - min_display) / 50;
    server.send(200, "text/plane", ""); //Send web page
  }

  if (server.arg("TOF_MinDisplay") == "") {   //Parameter not found
  } else {    //Parameter found
    min_display = server.arg("TOF_MinDisplay").toInt();     //Gets the value of the query parameter
    scale_display = (max_display - min_display) / 50;
    server.send(200, "text/plane", ""); //Send web page
  }

  if (server.arg("TOF_Preset") == "") {   //Parameter not found
  } else {    //Parameter found
    tof_preset = server.arg("TOF_Preset").toInt();     //Gets the value of the query parameter
    if (TOF_Tool == true) { //avoid crash with disconnected Sensor when refreshing the page
      tof_offset = tof_preset - tof.readRangeSingleMillimeters();
    }
    if (tof_preset == 0) { //set the axis so that graph is oscilating around the middle
      max_display   =  100;
      min_display   = -100;
      scale_display = (max_display - min_display) / 50;
    }
    if (tof_preset == 9999) { //reset the preset and offset value
      tof_offset = 0;
      tof_preset = 0;
    }
    server.send(200, "text/plane", ""); //Send web page
  }




}// end of handleString function


void handleData() {
  if (server.arg("GetDataLog") == "") {   //Parameter not found
  } else {    //Parameter found
    String datalog_state = server.arg("GetDataLog"); //Refer  xhttp.open("GET", "setLED?LEDstate="+led, true);
    String DataLogStat = "";
    if (datalog_state == "50") { //start the datalogging
      DataLogStat = "datalogging active";
      datalog = true;
      datalog_millis = millis();
      if (pause_datalogging == false) { //if the datalogging has been paused, don't create new file
        File dataFile = SD.open(filetowrite, FILE_WRITE);
        dataFile.close();
      }
    }
    if (datalog_state == "51") {
      DataLogStat = "dataloging inactive";  // stop datalogging
      datalog = false;
      pause_datalogging = false;
    }
    if (datalog_state == "52") {
      DataLogStat = "dataloging paused";  //pause datalogging
      datalog = false;
      pause_datalogging = true;
    }
    server.send(200, "text/plane", DataLogStat); //Send web page      //Gets the value of the query parameter
  }
  if (server.arg("GetPlot") == "") {   //Parameter not found
  } else {    //Parameter found
    String PlotPage = "";
    String plot_state = server.arg("GetPlot");
    if (plot_state == "1") {
      PlotPage = plotWebsite;
    }
    if (plot_state == "0") {
      PlotPage = "";
    }
    server.send(200, "text/plane", PlotPage); //Send web page
  }
  if (server.arg("GetControls") == "") {   //Parameter not found
  } else {    //Parameter found
    String ShowControls = "";
    String control_state = server.arg("GetControls");
    if (control_state == "1") {
      ShowControls = getControls();
    }
    if (control_state == "0") {
      ShowControls = "";
    }
    server.send(200, "text/plane", ShowControls); //Send web page
  }
  if (server.arg("GetFileLinks") == "") {   //Parameter not found
  } else {    //Parameter found
    String showfile_state = server.arg("GetFileLinks");
    if (showfile_state == "1") {
    root = SD.open("/");
    getfilelinks = "";
    while (true) {
      File entry =  root.openNextFile();
      if (! entry) {
        // no more files
        break;
      }
      mystring = "";
      mystring += entry.name();
      if (mystring.endsWith(".CSV")) {
        getfilelinks += "<p><a href='";
        getfilelinks += mystring;
        getfilelinks += "' class='button'>";
        getfilelinks += mystring;
        getfilelinks += "</a></p>";
      }
      entry.close();
      cnt++;
    }
    cnt = 1;
    }
    if (showfile_state == "2") {getfilelinks = "";}
    server.send(200, "text/plane", getfilelinks); //Send web page
  }
  if (server.arg("TOF_Mode") == "") {   //Parameter not found
  } else {    //Parameter found
    tof_rangeMode = server.arg("TOF_Mode").toInt();     //Gets the value of the query parameter
    if (TOF_Tool == true) { //avoid crash with disconnected Sensor when refreshing the page
      //extra functions for setting up the sensor
      if (tof_rangeMode == 1) { // LongRange // lower the return signal rate limit (default is 0.25 MCPS)
        tof.setSignalRateLimit(0.1);
        tof.setMeasurementTimingBudget(200000);
        // increase laser pulse periods (defaults are 14 and 10 PCLKs)
        tof.setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 18);
        tof.setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange, 14);
      }
      if (tof_rangeMode == 2) { // HighSpeed // reduce timing budget to 20 ms (default is about 33 ms)
        tof.setMeasurementTimingBudget(20000);
      }
      if (tof_rangeMode == 3) { // HighAccuracy // increase timing budget to 200 ms
        tof.setMeasurementTimingBudget(200000);
      }
    }// end avoid crash function
    String ShowControls = "";
    ShowControls = getControls();
    server.send(200, "text/plane", ShowControls); //Send web page
  }
  if (server.arg("FFT_Mode") == "") {   //Parameter not found
  } else {    //Parameter found
    FFT_mode = server.arg("FFT_Mode").toInt();     //Gets the value of the query parameter
    String ShowControls = "";
    ShowControls = getControls();
    server.send(200, "text/plane", ShowControls); //Send web page
  }
  if (server.arg("FFT_Orientation") == "") {   //Parameter not found
  } else {    //Parameter found
    acc_measurement = server.arg("FFT_Orientation").toInt();     //Gets the value of the query parameter
    String ShowControls = "";
    ShowControls = getControls();
    server.send(200, "text/plane", ShowControls); //Send web page
  }
  if (server.arg("FFT_Units") == "") {   //Parameter not found
  } else {    //Parameter found
    acc_measurand = server.arg("FFT_Units").toInt();     //Gets the value of the query parameter
    String ShowControls = "";
    ShowControls = getControls();
    server.send(200, "text/plane", ShowControls); //Send web page
  }
  if (server.arg("FFT_MaxFreq") == "") {   //Parameter not found
  } else {    //Parameter found
    freq_max = server.arg("FFT_MaxFreq").toInt();     //Gets the value of the query parameter
    String ShowControls = "";
    ShowControls = getControls();
    server.send(200, "text/plane", ShowControls); //Send web page
  }
  if (server.arg("Trigger_Reset") == "") {   //Parameter not found
  } else {    //Parameter found
    trigger_res = true;     //Gets the value of the query parameter
    String ShowControls = "";
    ShowControls = getControls();
    server.send(200, "text/plane", ShowControls); //Send web page
  }
  if (server.arg("IMU_Mode") == "") {   //Parameter not found
  } else {    //Parameter found
    imu_mode = server.arg("IMU_Mode").toInt();     //Gets the value of the query parameter
    if (imu_mode == 4) { //set the calibration trigger to true
      trigger_calib = true;
    }     //Gets the value of the query parameter
    String ShowControls = "";
    ShowControls = getControls();
    server.send(200, "text/plane", ShowControls); //Send web page
  }

}//end of handleData function
