# ESP32-Sensor-Tool
updated version of the ESP-Sensor-Tool which is now working on an ESP32

use different sensors with the ESP32 and control them using a web interface

All sensors can be connected via I2C to the ESP. The SD Card is connected via SPI.

Below you see the main unit, which is controlling the sensors via i2c bus.

It is attached to a power bank via a pin connector in order to use it without a wall plug.



![CAD](https://github.com/Hakzel/ESP32-Sensor-Tool/blob/master/CAD/Sensor-Tool.79.png)


I added the eagle shematics of the main so you can get an idea of how the main unit is connected to the sensors.

All sensors are simply connected via i2c over the RJ45 headers on both partners

![Shematics](https://github.com/Hakzel/ESP32-Sensor-Tool/blob/master/EAGLE/ESP32-Sensor-Tool.PNG)
