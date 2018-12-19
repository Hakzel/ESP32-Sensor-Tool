


#include <SPI.h>    //include SPI library
#include <SD.h>     //include SD-Card library
#include <Wire.h>   //include I2C library
#include <FS.h>
//#include <WiFiMulti.h>

#include "SH1106.h"     // https://github.com/squix78/esp8266-oled-ssd1306


#define I2C_SDA_PIN 21
#define I2C_SCL_PIN 22
SH1106 display(0x3c, I2C_SDA_PIN, I2C_SCL_PIN); // 1.3" OLED display object definition (address, SDA, SCL) Connect OLED SDA , SCL pins to ESP SDA, SCL pins
