#include "MPU9250.h"  //https://github.com/bolderflight/MPU9250

MPU9250 IMU(Wire, 0x68);
int status;

bool init_imu = true;
bool trigger_calib = true;

float radius, x_middle, y_middle;

double accX, accY, accZ, alpha, beta;

double magX, magY, magZ, mag_alpha;


void Loop_IMU() {

  Wire.beginTransmission (0x68);

  if (Wire.endTransmission () == 0) {

    if (init_imu == true) {               // initialize tof after first response
      IMU.begin();

      display.setTextAlignment(TEXT_ALIGN_CENTER);
      display.drawString(64, 30, "9DOF connected");
      display.display();
      display.setTextAlignment(TEXT_ALIGN_LEFT);
      delay(1000);
      IMU.setMagCalX(12.0, 1.2);
      IMU.setMagCalY(53.0, 1.2);
      IMU.setMagCalZ(-77.5, 1.2);

      init_imu = false ;
      IMU_Tool = true ;

    }


    if (imu_mode == 1) { // bubble level mode

      IMU.readSensor();
      accX = IMU.getAccelX_mss();
      accY = IMU.getAccelY_mss();
      accZ = IMU.getAccelZ_mss();

      //if(accZ < -4){// put the level in z-direction
      alpha = atan(accX / accZ) / 3.142 * 180;
      beta = atan(accY / accZ) / 3.142 * 180;

      mystring = "alpha: ";
      mystring += alpha;
      mystring += "°";
      display.drawString(0, 0, mystring);

      mystring = "beta: ";
      mystring += beta;
      mystring += "°";
      display.drawString(64, 0, mystring);

      //x=alpha.toInt();
      //y=beta.toInt();

      display.drawCircle( 64,  37, 22  );
      display.drawCircle( 64 + alpha * 2,  37 + beta * 2 , 18  );

      //}// end of acceleration Z mode loop

    }//end of bubble level mode


    if (imu_mode == 2) { // compass mode


      IMU.readSensor();
      magX = IMU.getMagX_uT();
      magY = IMU.getMagY_uT();


      mag_alpha = atan2(magX, magY); ///3.142*180;

      mag_alpha -= PI / 2;

      if (mag_alpha < 0) mag_alpha += 2 * PI;
      if (mag_alpha > 2 * PI)mag_alpha -= 2 * PI;

      mystring = "alpha: ";
      mystring += mag_alpha;
      display.drawString(0, 0, mystring);

      display.drawCircle( x_middle - radius * cos(mag_alpha), y_middle - radius * sin(mag_alpha), 3  );


      radius = 30;
      x_middle = 80;
      y_middle = 32;
      //drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1);
      display.drawLine(x_middle + radius * cos(mag_alpha), y_middle + radius * sin(mag_alpha), x_middle - radius * cos(mag_alpha), y_middle - radius * sin(mag_alpha));


    }//end of compass mode




    if (imu_mode == 3) { // just plotting the raw data

      IMU.readSensor();

      mystring = "AccX: ";
      mystring += String(IMU.getAccelX_mss());
      //mystring += "mss";
      display.drawString(0, 0, mystring);

      mystring = "AccY: ";
      mystring += String(IMU.getAccelY_mss());
      //mystring += "mss";
      display.drawString(0, 10, mystring);

      mystring = "AccZ: ";
      mystring += String(IMU.getAccelZ_mss());
      //mystring += "mss";
      display.drawString(0, 20, mystring);



      mystring = "GyrX: ";
      mystring += String(IMU.getGyroX_rads());
      //mystring += "rads";
      display.drawString(64, 0, mystring);

      mystring = "GyrY: ";
      mystring += String(IMU.getGyroY_rads());
      //mystring += "rads";
      display.drawString(64, 10, mystring);

      mystring = "GyrZ: ";
      mystring += String(IMU.getGyroZ_rads());
      //mystring += "rads";
      display.drawString(64, 20, mystring);



      mystring = "MagX: ";
      mystring += String(IMU.getMagX_uT());
      //mystring += "uT";
      display.drawString(0, 34, mystring);

      mystring = "MagY: ";
      mystring += String(IMU.getMagY_uT());
      //mystring += "uT";
      display.drawString(0, 44, mystring);

      mystring = "MagZ: ";
      mystring += String(IMU.getMagZ_uT());
      //mystring += "uT";
      display.drawString(0, 54, mystring);



      //mystring = "Temp: ";
      mystring
        = String(IMU.getTemperature_C());
      mystring += "°C";
      display.drawString(80, 54, mystring);
    }



    if (imu_mode == 4) { // calibration of magnetometer

      if (trigger_calib == true) { //doing the calibration only once per click
        display.clear();
        display.drawString(25, 32, "calibration running");
        display.display();
        delay(2000);

        IMU.calibrateMag();
        display.clear();
        display.drawString(30, 32, "calibration done");
        display.display();
        delay(2000);
        display.clear();
        trigger_calib = false;
      }

      IMU.readSensor();

      mystring = "BiasX: ";
      mystring += String(IMU.getMagBiasX_uT());
      //mystring += "mss";
      display.drawString(0, 0, mystring);

      mystring = "BiasY: ";
      mystring += String(IMU.getMagBiasY_uT());
      //mystring += "mss";
      display.drawString(0, 10, mystring);

      mystring = "BiasZ: ";
      mystring += String(IMU.getMagBiasZ_uT());
      //mystring += "mss";
      display.drawString(0, 20, mystring);



      mystring = "ScaleX: ";
      mystring += String(IMU.getMagScaleFactorX());
      //mystring += "rads";
      display.drawString(64, 0, mystring);

      mystring = "ScaleY: ";
      mystring += String(IMU.getMagScaleFactorX());
      //mystring += "rads";
      display.drawString(64, 10, mystring);

      mystring = "Scale: ";
      mystring += String(IMU.getMagScaleFactorX());
      //mystring += "rads";
      display.drawString(64, 20, mystring);


      //mystring = "Temp: ";
      mystring
        = String(IMU.getTemperature_C());
      mystring += "°C";
      display.drawString(80, 54, mystring);




    }//end of calibration mode




  }// END OF IMU LOOP


  //if (Wire.endTransmission () != 0) {
  else {
    init_imu = true ;
    IMU_Tool = false;
  }

}
