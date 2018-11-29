
//Configuration of Trigger input and output

#include <pcf8574_esp.h>

PCF857x trigger(0x20, &Wire);
PCF857x trigger_out(0x27, &Wire);


// for trigger input
bool init_trigger = true;
bool init_trigger_out = true;
bool trigger_old;

int trigger_temp;
int trigger_signal = 0;
int i_trigger_frequency;

double trigger_frequency;

String trigger_active;


//for trigger output

bool trigger_out_state = false;


void Loop_Trigger() {


  Wire.beginTransmission (0x20);

  if (Wire.endTransmission () == 0) {

    if (init_trigger == true) {               // initialize adxl after first response

      trigger.begin(0xFF);
      // trigger.pinMode(0, INPUT_PULLUP);


      display.setTextAlignment(TEXT_ALIGN_CENTER);
      display.drawString(64, 30, "Trigger connected");
      display.display();
      display.setTextAlignment(TEXT_ALIGN_LEFT);
      delay(1000);
      if (FFT_Tool == false) {
        display.clear(); //only clear display if FFT-Tool is not active
      }
      init_trigger = false ;
      Trigger_Tool = true ;
    }

    if (Trigger_Tool == true && FFT_Tool == false) { // routine if the Trigger Sensor is connected alone

      trigger_signal = trigger.read(3);

      if (trigger_signal == 0) { // if an object is detected
        trigger_active = "Object: YES";
        if (trigger_old == false) {
          trigger_frequency = 1000000.0 / (micros() - trigger_temp);
          trigger_temp = micros();
          trigger_speed = trigger_frequency * 60.0 * trigger_dia / 1000 * 3.14159265359;
          trigger_length = trigger_length + trigger_dia / 1000 * 3.14159265359;
        }
        trigger_old = true;
      }
      if (trigger_signal == 1) {
        trigger_active = "Object: NO";
        trigger_old = false;
      }

      if (trigger_res == true) {
        trigger_length = 0.0;
        trigger_res = false;
      }


      //display.drawString(40,0,trigger_active);

      display.setTextAlignment(TEXT_ALIGN_RIGHT);
      mystring = String(trigger_frequency);
      display.drawString(90, 12, mystring);
      mystring = String(trigger_speed);
      display.drawString(90, 24, mystring);
      mystring = String(trigger_length);
      display.drawString(90, 36, mystring);

      display.setTextAlignment(TEXT_ALIGN_LEFT);
      display.drawString(95, 12, "Hz");
      display.drawString(95, 24, "m/min");
      display.drawString(95, 36, "m");

    }//End of Trigger alone Loop

  }// END OF TRIGGER LOOP


  //if (Wire.endTransmission () != 0) {
  else {
    init_trigger = true ;
    Trigger_Tool = false;
  }



  //for trigger output




  Wire.beginTransmission (0x27);

  if (Wire.endTransmission () == 0) {

    if (init_trigger_out == true) {               // initialize adxl after first response

      trigger_out.begin(0x00);

      display.setTextAlignment(TEXT_ALIGN_CENTER);
      display.drawString(64, 20, "Trigger Output connected");
      display.display();
      display.setTextAlignment(TEXT_ALIGN_LEFT);
      delay(1000);
      init_trigger_out = false ;
      Trigger_Out_Tool = true ;
    }

    delay(1000);
    trigger_out.write(1, 1);
    display.drawString(40, 10, "Trigger 1");
    display.display();

    delay(1000);
    trigger_out.write(1, 0);
    display.drawString(40, 10, "Trigger 0");

  }//End of Trigger_Out Loop




  //if (Wire.endTransmission () != 0) {
  else {
    init_trigger_out = true ;
    Trigger_Out_Tool = false;
  }















  /////// only for test of other PCF
  //
  //
  //
  //
  //
  //
  //
  //
  //
  //
  //
  //Wire.beginTransmission (0x21);
  //
  //if (Wire.endTransmission () == 0) {
  //
  //  if(init_LC == true){                 // initialize adxl after first response
  //
  //  LC.begin(0x0F);
  //
  //  display.setTextAlignment(TEXT_ALIGN_CENTER);
  //  display.drawString(64, 30, "LC connected");
  //  display.display();
  //  display.setTextAlignment(TEXT_ALIGN_LEFT);
  //  delay(1000);
  //  init_LC = false ;
  //  //LC_Tool = true ;
  //  }
  //
  //  if(laststate == true){
  //    LC.write(7,1);
  //    laststate=false;
  //  }else{
  //    LC.write(7,0);
  //    laststate=true;
  //  }
  //
  //  if(LC_signal == 0){ // if an object is detected
  //    LC_active="Object: YES";
  //  }
  //  if(LC_signal == 1){
  //    LC_active="Object: NO";
  //  }
  //
  //
  //  display.drawString(40,10,LC_active);
  //
  //
  //
  //}// END OF TRIGGER LOOP
  //
  //
  ////if (Wire.endTransmission () != 0) {
  //else{
  //   init_LC = true ;
  //   //LC_Tool=false;
  //}

}
