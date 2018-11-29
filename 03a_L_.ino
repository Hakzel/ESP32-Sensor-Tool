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

  



  
  
