
  if((FFT_Tool == false) && (ADC_Tool == false) && (Trigger_Tool == false) && (Trigger_Out_Tool == false) && (Thermo_Tool == false) && (APDS_Tool == false) && (Power_Tool == false) && (TOF_Tool == false) && (DAC_Tool == false) && (IMU_Tool == false)   ){
    if (millis() - lastcall > 100){ // getting data for plotting the trend
      lastcall=millis();
      i_display_buffer=i_display_buffer-2;
      if(i_display_buffer<-128){i_display_buffer=128;}
      if(i_display_radius<=1){
        display_radius_grow=true;   
      }
      if(i_display_radius>=6){
        display_radius_grow=false;   
      }
      if(display_radius_grow){
        i_display_radius++;   
      }else { i_display_radius--; }
   }
      
  if(millis()<5*60*1000){//disable the fixed text after 5 min to prevent burn in
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.drawString(64, 0, "Sensor-Tool V02");
    display.drawString(64, 15, "connect WiFi to:");
    display.drawString(64, 25, ssid);
    display.drawString(64, 35, password);
    display.setTextAlignment(TEXT_ALIGN_LEFT);
  }
  
  display.drawString(i_display_buffer, 50 , "no Sensor connected");
  display.drawCircle( 20,  35,  i_display_radius);
  display.drawCircle( 108,  35,  i_display_radius);
    
}


display.display();
}//End of loop
