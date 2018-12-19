void loop() {
  
  server.handleClient();
  display.clear();

  
  Loop_ACC();
  Loop_ADC();
  Loop_Trigger();
  Loop_Thermo();
  Loop_APDS();
  Loop_Power();
  Loop_TOF();
  Loop_DAC();
  Loop_IMU();
 
  yield();

  mystring = String(WiFi.localIP());
  display.drawString(0, 30, mystring);
    
//  idle(20);  // allocate time to the idle process in order to prevent system failure



  
  
