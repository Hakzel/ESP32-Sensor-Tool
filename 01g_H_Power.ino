#include <Adafruit_INA219.h>

Adafruit_INA219 ina219;

bool init_power = true;

float shuntvoltage = 0;
float busvoltage = 0;
float current_mA = 0;
float loadvoltage = 0;
float power_mW = 0;


void Loop_Power() {

  Wire.beginTransmission (0x40);

  if (Wire.endTransmission () == 0) {

    if (init_power == true) {               // initialize adxl after first response
      ina219.begin();

      // To use a slightly lower 32V, 1A range (higher precision on amps):
      //ina219.setCalibration_32V_1A();
      // Or to use a lower 16V, 400mA range (higher precision on volts and amps):
      ina219.setCalibration_16V_400mA();

      display.setTextAlignment(TEXT_ALIGN_CENTER);
      display.drawString(64, 10, "Power-Sens connected");
      display.display();
      display.setTextAlignment(TEXT_ALIGN_LEFT);
      delay(1000);
      display.clear();
      init_power = false ;
      Power_Tool = true ;
    }

    shuntvoltage = ina219.getShuntVoltage_mV();
    busvoltage = ina219.getBusVoltage_V();
    current_mA = ina219.getCurrent_mA();
    //power_mW = ina219.getPower_mW();  //not available in library for some reason
    loadvoltage = busvoltage + (shuntvoltage / 1000);

    power_mW = current_mA * loadvoltage; // manual calculation of the power consumption


    display.drawString(0, 0, "Bus Voltage:");
    display.drawString(0, 12, "Shunt Voltage:");
    display.drawString(0, 24, "Load Voltage:");
    display.drawString(0, 36, "Current:");
    display.drawString(0, 48, "Power:");

    display.drawString(112, 0, "V");
    display.drawString(112, 12, "mV");
    display.drawString(112, 24, "V");
    display.drawString(112, 36, "mA");
    display.drawString(112, 48, "mW");

    display.setTextAlignment(TEXT_ALIGN_RIGHT);
    mystring = busvoltage;
    display.drawString(110, 0, mystring);
    mystring = shuntvoltage;
    display.drawString(110, 12, mystring);
    mystring = loadvoltage;
    display.drawString(110, 24, mystring);
    mystring = current_mA;
    display.drawString(110, 36, mystring);
    mystring = power_mW;
    display.drawString(110, 48, mystring);

    display.setTextAlignment(TEXT_ALIGN_LEFT);


  }// END OF APDS LOOP


  //if (Wire.endTransmission () != 0) {
  else {
    init_power = true ;
    Power_Tool = false;
  }

}
