
void handleSpecificArg() { 


if (server.arg("ordinal_number_inner")== ""){     //Parameter not found
}else{     //Parameter found
  on_inner = server.arg("ordinal_number_inner").toFloat();     //Gets the value of the query parameter
}

if (server.arg("ordinal_number_outer")== ""){     //Parameter not found
}else{     //Parameter found
  on_outer = server.arg("ordinal_number_outer").toFloat();     //Gets the value of the query parameter
}

if (server.arg("ordinal_number_ball")== ""){     //Parameter not found
}else{     //Parameter found
  on_ball = server.arg("ordinal_number_ball").toFloat();     //Gets the value of the query parameter
}

if (server.arg("rot_freq")== ""){     //Parameter not found
}else{     //Parameter found
  f_rot = server.arg("rot_freq").toFloat();     //Gets the value of the query parameter
}

if (server.arg("measurement_mode")== ""){     //Parameter not found
}else{     //Parameter found
  FFT_mode = server.arg("measurement_mode").toInt();     //Gets the value of the query parameter
}

if (server.arg("acc_orientation")== ""){     //Parameter not found
}else{     //Parameter found
  acc_measurement = server.arg("acc_orientation").toInt();     //Gets the value of the query parameter
}

if (server.arg("displayed_value")== ""){     //Parameter not found
}else{     //Parameter found
  acc_measurand = server.arg("displayed_value").toInt();     //Gets the value of the query parameter
}

if (server.arg("max_freq")== ""){     //Parameter not found
}else{     //Parameter found
  freq_max = server.arg("max_freq").toInt();     //Gets the value of the query parameter
}

if (server.arg("datafilename")== ""){     //Parameter not found
}else{     //Parameter found
  datafile = server.arg("datafilename");     //Gets the value of the query parameter
  filetowrite ="/";                         // generating the file name
  filetowrite += datafile += "\0";
  filetowrite += ".CSV";
}

if (server.arg("trigger_diameter")== ""){     //Parameter not found
}else{     //Parameter found
  trigger_dia = server.arg("trigger_diameter").toFloat();     //Gets the value of the query parameter
}


if (server.arg("web_max_display")== ""){     //Parameter not found
}else{     //Parameter found
  max_display = server.arg("web_max_display").toInt();     //Gets the value of the query parameter
  scale_display = (max_display-min_display)/50;
}

if (server.arg("web_min_display")== ""){     //Parameter not found
}else{     //Parameter found
  min_display = server.arg("web_min_display").toInt();     //Gets the value of the query parameter
  scale_display = (max_display-min_display)/50;
}

if (server.arg("web_tof_preset")== ""){     //Parameter not found
}else{     //Parameter found
  tof_preset = server.arg("web_tof_preset").toInt();     //Gets the value of the query parameter
  if(TOF_Tool == true){ //avoid crash with disconnected Sensor when refreshing the page
    tof_offset = tof_preset-tof.readRangeSingleMillimeters();
  }
  if(tof_preset==0){//set the axis so that graph is oscilating around the middle
    max_display   =  100;
    min_display   = -100;
    scale_display = (max_display-min_display)/50;
  }
  if(tof_preset == 9999){//reset the preset and offset value
    tof_offset=0;
    tof_preset=0;
  }
}


if (server.arg("tof_measurement_mode")== ""){     //Parameter not found
}else{     //Parameter found
  tof_rangeMode = server.arg("tof_measurement_mode").toInt();     //Gets the value of the query parameter
  if(TOF_Tool == true){ //avoid crash with disconnected Sensor when refreshing the page  
    //extra functions for setting up the sensor
    if (tof_rangeMode == 1){// LongRange // lower the return signal rate limit (default is 0.25 MCPS)
    tof.setSignalRateLimit(0.1);
    tof.setMeasurementTimingBudget(200000);
    // increase laser pulse periods (defaults are 14 and 10 PCLKs)
    tof.setVcselPulsePeriod(VL53L0X::VcselPeriodPreRange, 18);
    tof.setVcselPulsePeriod(VL53L0X::VcselPeriodFinalRange, 14);
    }
    if (tof_rangeMode == 2){// HighSpeed // reduce timing budget to 20 ms (default is about 33 ms)
    tof.setMeasurementTimingBudget(20000);
    }
    if (tof_rangeMode == 3){// HighAccuracy // increase timing budget to 200 ms
    tof.setMeasurementTimingBudget(200000);
    }
  }// end avoid crash function   
}

if (server.arg("imu_measurement_mode")== ""){     //Parameter not found
}else{     //Parameter found
  imu_mode = server.arg("imu_measurement_mode").toInt();     //Gets the value of the query parameter  
  if(imu_mode ==4){//set the calibration trigger to true
    trigger_calib = true;
  }

}

server.send(200, "text/html", getContent());          //Returns the HTTP response

}
