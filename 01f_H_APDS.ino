#include "Adafruit_APDS9960.h"
Adafruit_APDS9960 apds;

uint16_t ambient_light = 0;
uint16_t red_light = 0;
uint16_t green_light = 0;
uint16_t blue_light = 0;


bool init_apds = true;

uint16_t r, g, b, a;


void Loop_APDS() {

  Wire.beginTransmission (0x39);

  if (Wire.endTransmission () == 0) {

    if (init_apds == true) {               // initialize adxl after first response
      apds.begin();
      apds.enableColor(true);
      display.setTextAlignment(TEXT_ALIGN_CENTER);
      display.drawString(64, 30, "APDS connected");
      display.display();
      display.setTextAlignment(TEXT_ALIGN_LEFT);
      delay(1000);
      display.clear();
      init_apds = false ;
      APDS_Tool = true ;
    }

    apds.getColorData(&r, &g, &b, &a);

    r = 100 * r / a;
    if (r > 100) {
      r = 100;
    }
    g = 100 * g / a;
    if (g > 100) {
      g = 100;
    }
    b = 100 * b / a;
    if (b > 100) {
      b = 100;
    }

    a = a / 20;
    if (a > 100) {
      a = 100;
    }

    display.drawString(122, 0, "r");
    display.drawProgressBar(0, 2, 120, 10 , r);
    display.drawString(122, 12, "g");
    display.drawProgressBar(0, 14, 120, 10 , g);
    display.drawString(122, 24, "b");
    display.drawProgressBar(0, 26, 120, 10 , b);
    display.drawString(122, 36, "a");
    display.drawProgressBar(0, 38, 120, 10 , a);


  }// END OF APDS LOOP


  //if (Wire.endTransmission () != 0) {
  else {
    init_apds = true ;
    APDS_Tool = false;
  }

}
