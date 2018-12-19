/// for Webserver ////////////////////////////////////////////////////////////

#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include "index.h"

File root;
int cnt=1;
 
int temp;

int lastcall;
unsigned long datalog_millis;
float display_buffer[128];
int i_display_buffer = 0;
int i_display_radius = 0;
bool display_radius_grow = true;
String mystring; //temporary string for multiple usage
String outputtest;

const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 4, 1);

const char* ssid = "ESP-Tool";
const char* password = "12345678";  // set to "" for open access point w/o passwortd

String plotWebsite = "";
String getfilelinks ="";


unsigned long ulReqcount;

static bool hasSD = false;


// hostname for this system:
const char* host = "webcontrol";

// define GPIO pins:
const int output1 = 100;
const int output2 = 101;


String myValue   = "TEST";
String datafile  = "DATALOG";
String dataComment = "";
String filetowrite = "/";


float f_rot = 3, on_inner = 10.9 , on_outer = 8.13 , on_ball = 6.72 ;

String sampling_rate = "512";

// output status variables:
boolean output1_state = true;
boolean datalog = false;
boolean refreshlinks = false;
bool    datalogging = false;
bool    pause_datalogging = false;
boolean dataNewComment = false;

bool FFT_Tool;
bool ADC_Tool;
bool Trigger_Tool;
bool Trigger_Out_Tool;
bool Thermo_Tool;
bool APDS_Tool;
bool Power_Tool;
bool TOF_Tool;
bool DAC_Tool;
bool IMU_Tool;

int acc_measurement = 3;
int FFT_mode = 1;
int acc_measurand = 1;  //1=acceleration 2=displacement
int freq_max = 64, f_scale = 1;

float trigger_dia = 120.0;
float trigger_length = 0;
float trigger_speed = 0;
bool trigger_res = false;

int tof_rangeMode = 2; // 1 = Long_Range   2 = HighSpeed  3 = High Accuracy
int max_display = 200, min_display = 0, scale_display = (max_display - min_display) / 50, tof_preset = 0;

int imu_mode = 1;

double max_peak = 0;
double peak;
WebServer server(80);





// https://www.w3schools.com/code/tryit.asp?filename=FY7XAKT2A93U


String getControls()
{
  // create content for the website:
  String content = "";
  
   if (FFT_Tool) {
    content += "Control the FFT_Tool<br /><br />";
    content += R"(<br>
        <table align='center'><tr>
            <td align='left'> ordinal number inner ring:</td>
            <td align='left'><form><input type='number' step='0.01' id='FFT_O_Inner' placeholder='10.90'></td>)";
    content +="<td align='left'> <input type='button' onclick=\"SendString('FFT_O_Inner','')\" class='button'></form></td>";
    content += R"(</tr>
            <tr>
            <td align='left'> ordinal number outer ring:</td>
            <td align='left'><form><input type='number' step='0.01' id="FFT_O_Outer" placeholder="8.13"></td>)";
    content += "<td align='left'> <input type='button' onclick=\"SendString('FFT_O_Outer','')\" class='button'></form></td>";
    content += R"(</tr>
          <tr>
            <td align='left'> ordinal number rolling element:</td>
            <td align='left'><form><input type='number' step='0.01' id='FFT_O_Roller' placeholder='6.72'></td>)";
    content +="<td align='left'> <input type='button' onclick=\"SendString('FFT_O_Roller','')\" class='button'></form></td>";
    content += R"(</tr>
          <tr>
            <td align='left'> frequency of rotation:</td>
            <td align='left'><form><input type='number' step='0.01' id='FFT_F_Rot' placeholder='3.00'></td>)";
    content +="<td align='left'> <input type='button' onclick=\"SendString('FFT_F_Rot','')\" class='button'></form></td>";
    content += "</tr></table>";
       
    content +=    "<br><br><br>measurement mode";
    content +=    "<form>";
    content +=    "<input type='radio' onclick=\"sendData('FFT_Mode',1,'ControlPage')\"";
    if (FFT_mode == 1)content +=    " checked ";
    content +=    ">show FFT<BR>";
    content +=    "<input type='radio' onclick=\"sendData('FFT_Mode',2,'ControlPage')\"";
    if (FFT_mode == 2)content += " checked";
    content +=    ">bearing detection<BR>";
    content +=    "<input type='radio' onclick=\"sendData('FFT_Mode',3,'ControlPage')\"";
    if (FFT_mode == 3)content += " checked";
    content +=    ">display raw values<BR>";
    content +=    "<input type='radio' onclick=\"sendData('FFT_Mode',4,'ControlPage')\"";
    if (FFT_mode == 4)content += " checked";
    content +=    ">display example FFT<BR>";
    content +=    "</form>";

    content +=    "<br><br>measurement direction";
    content +=    "<form>";
    content +=    "<input type='radio' onclick=\"sendData('FFT_Orientation',1,'ControlPage')\"";
    if (acc_measurement == 1)content +=    " checked ";
    content +=    ">x-direction<BR>";
    content +=    "<input type='radio' onclick=\"sendData('FFT_Orientation',2,'ControlPage')\"";
    if (acc_measurement == 2)content += " checked";
    content +=    ">y-direction<BR>";
    content +=    "<input type='radio' onclick=\"sendData('FFT_Orientation',3,'ControlPage')\"";
    if (acc_measurement == 3)content += " checked";
    content +=    ">z-direction<BR>";
    content +=    "</form>";

    content +=    "<br><br>displayed FFT";
    content +=    "<form>";
    content +=    "<input type='radio' onclick=\"sendData('FFT_Units',1,'ControlPage')\"";
    if (acc_measurand == 1)content +=    " checked ";
    content +=    ">Acceleration<BR>";
    content +=    "<input type='radio' onclick=\"sendData('FFT_Units',2,'ControlPage')\"";
    if (acc_measurand == 2)content += " checked";
    content +=    ">Displacement<BR>";
    content +=    "</form>";


    content +=    "<br><br>max-plot-frequency";
    content +=    "<form>";
    content +=    "<input type='radio' onclick=\"sendData('FFT_MaxFreq',64,'ControlPage')\"";
    if (freq_max == 64)content +=    " checked ";
    content +=    ">64Hz<BR>";
    content +=    "<input type='radio' onclick=\"sendData('FFT_MaxFreq',128,'ControlPage')\"";
    if (freq_max == 128)content += " checked";
    content +=    ">128Hz<BR>";
    content +=    "<input type='radio' onclick=\"sendData('FFT_MaxFreq',256,'ControlPage')\"";
    if (freq_max == 256)content += " checked";
    content +=    ">256Hz<BR>";
    content +=    "</form>";


  }//END FFT TOOL

  if (ADC_Tool) {
    content += "Control the ADC_Tool<br /><br />";

    content += "nothing to control yet<br /><br />";

  }//END ADC_TOOL

  if (Trigger_Tool) {
    content += "Control the Trigger_Tool<br />";

    
    content += R"(<br>
        <table align='center'><tr>
            <td align='left'> diameter of roller:</td>
            <td align='left'><form><input type='number' step='0.01' id='Trigger_Dia' placeholder='200'></td>)";
    content +="<td align='left'> <input type='button' onclick=\"SendString('Trigger_Dia','')\" class='button'></form></td>";
    content += "</tr></table>";
    
    content += "<table align='center'><tr>";
    content += "<td align='left'> <input type='button' onclick=\"sendData('Trigger_Reset',1,'ControlPage')\" class='button' value='reset length'></form></td>";
    content += "</tr></table>";


    content += "<br /><br />";
    content += trigger_length;

    content += " m  <br /><br />";
    content += trigger_speed;
    content += " m/min <br /><br />";

  }//END Trigger_TOOL

  if (Thermo_Tool) {
    
    content += "Control the Thermo_Tool<br /><br />";
    content += "nothing to control yet<br /><br />";

  }//END ADC_TOOL

  if (TOF_Tool) {
    content += "Control the TOF_Tool<br />";

    content += R"(<br>
        <table align='center'><tr>
            <td align='left'> max value to display:</td>
            <td align='left'><form><input type='number' size='1' min='-9999' max='9999' id='TOF_MaxDisplay' placeholder='200'></td>)";
    content +="<td align='left'> <input type='button' onclick=\"SendString('TOF_MaxDisplay','')\" class='button'></form></td>";
    content += R"(</tr>
         <tr>
            <td align='left'> min value to display:</td>
            <td align='left'><form><input type='number' size='1' min='-9999' max='9999' id="TOF_MinDisplay" placeholder="0"></td>)";
    content += "<td align='left'> <input type='button' onclick=\"SendString('TOF_MinDisplay','')\" class='button'></form></td>";
    content += R"(</tr>
          <tr>
            <td align='left'> preset value for act position: <br/>(9999 for reset)</td>
            <td align='left'><form><input type='number' size='1' min='-9999' max='9999' id='TOF_Preset' placeholder='0'></td>)";
    content +="<td align='left'> <input type='button' onclick=\"SendString('TOF_Preset','')\" class='button'></form></td>";
    content += "</tr></table>";

    content +=    "<br><br>measurement mode";
    content +=    "<form>";
    content +=    "<input type='radio' onclick=\"sendData('TOF_Mode',1,'ControlPage')\"";
    if (tof_rangeMode == 1)content +=    " checked ";
    content +=    ">long range<BR>";
    content +=    "<input type='radio' onclick=\"sendData('TOF_Mode',2,'ControlPage')\"";
    if (tof_rangeMode == 2)content += " checked";
    content +=    ">high speed<BR>";
    content +=    "<input type='radio' onclick=\"sendData('TOF_Mode',3,'ControlPage')\"";
    if (tof_rangeMode == 3)content += " checked";
    content +=    ">high precision<BR>";
    content +=    "</form>";

  }//END TOF TOOL


  if (IMU_Tool) {
    content += "Control the IMU_Tool<br />";

    //content += plotWebsite;

    content +=    "<br><br>measurement mode and calibration";
    content +=    "<form>";
    content +=    "<input type='radio' onclick=\"sendData('IMU_Mode',1,'ControlPage')\"";
    if (imu_mode == 1)content +=    " checked ";
    content +=    ">bubble level<BR>";
    content +=    "<input type='radio' onclick=\"sendData('IMU_Mode',2,'ControlPage')\"";
    if (imu_mode == 2)content += " checked";
    content +=    ">compass<BR>";
    content +=    "<input type='radio' onclick=\"sendData('IMU_Mode',3,'ControlPage')\"";
    if (imu_mode == 3)content += " checked";
    content +=    ">raw data<BR>";
    content +=    "<input type='radio' onclick=\"sendData('IMU_Mode',4,'ControlPage')\"";
    if (imu_mode == 4)content += " checked";
    content +=    ">calibrate magnetometer<BR>";
    content +=    "</form>";


  }//END IMU TOOL


  if ( (FFT_Tool == false) && (ADC_Tool == false) && (Trigger_Tool == false) && (Trigger_Out_Tool == false) && (Thermo_Tool == false) && (APDS_Tool == false) && (Power_Tool == false) && (TOF_Tool == false) && (DAC_Tool == false) && (IMU_Tool == false)   ) {
    content += "no Sensor connected <br>";
    content += "<img src='/PICS/Sensor-Tool.png' alt='no SD Card found'>";
  }

 // content += "</div><div id='mybutton'>";
 // content += "<a href='/' class='button'>&#8634</a> </div>";
  
  content += "</body></html>";
  return content;

}// end of get content string

//start ´of creating webpage for plotting data

//String getPlot()
//{
//  // create content for the website:
//  String plotpage = //HeadOfPage;
//  
//  
//  plotpage +=     R"(<div class='pagination'>
//                  <a href='/'     >Home</a>
//                  <a href='/plot' class='active'>PlotPage</a>
//                  <a href='/links'>Downloads</a>
//                  </div><br>
//                  )";
//  
//  if (FFT_Tool || Thermo_Tool) {
//    plotpage += "<meta http-equiv='refresh' content='5'>";
//    //plotpage += "<style svg polyline{-webkit-animation: dash 0s linear forwards;animation: dash 0s linear forwards;} /style>  ";
//    plotpage += plotWebsite;
//  }
//  if (TOF_Tool) {
//    plotpage += "<meta http-equiv='refresh' content='1'>";
//    plotpage += plotWebsite;
//  }
//  
//  plotpage += "</div></body></html>";
//  
//  return plotpage;
//
//}// end of getting plotPage String




//String getLinks()
//{
//  // create content for the website:
//  String linkpage = //HeadOfPage;//"<html><head><title>ESP8266 WebControl</title></head><body><div class="content">";
//  //linkpage += "<meta name='viewport' content='width=device-width, initial-scale=1'>";   //fit webpage to screen size
//  
//  linkpage +=     R"(<div class='pagination'>
//                  <a href='/'     >Home</a>
//                  <a href='/plot' >PlotPage</a>
//                  <a href='/links' class='active'>Downloads</a>
//                  </div><br>
//                  )";
//  
//  linkpage += "<td align 'left'>  </td> <td align='center'> <a href='refreshlinks'><button class='button'> refresh the links</button></a></td></tr>";
//  linkpage += getfilelinks; 
//  linkpage += "</div></body></html>";
//  return linkpage;
//
//}// end of getting linkpage String



// data server functions




File uploadFile;


void returnOK() {
  server.send(200, "text/plain", "");
}

void returnFail(String msg) {
  server.send(500, "text/plain", msg + "\r\n");
}

bool loadFromSdCard(String path) {
  String dataType = "text/plain";
  if (path.endsWith("/")) {
    path += "index.htm";
  }

  if (path.endsWith(".src")) {
    path = path.substring(0, path.lastIndexOf("."));
  } else if (path.endsWith(".htm")) {
    dataType = "text/html";
  } else if (path.endsWith(".css")) {
    dataType = "text/css";
  } else if (path.endsWith(".js")) {
    dataType = "application/javascript";
  } else if (path.endsWith(".png")) {
    dataType = "image/png";
  } else if (path.endsWith(".gif")) {
    dataType = "image/gif";
  } else if (path.endsWith(".jpg")) {
    dataType = "image/jpeg";
  } else if (path.endsWith(".ico")) {
    dataType = "image/x-icon";
  } else if (path.endsWith(".xml")) {
    dataType = "text/xml";
  } else if (path.endsWith(".pdf")) {
    dataType = "application/pdf";
  } else if (path.endsWith(".zip")) {
    dataType = "application/zip";
  }

  File dataFile = SD.open(path.c_str());
  if (dataFile.isDirectory()) {
    path += "/index.htm";
    dataType = "text/html";
    dataFile = SD.open(path.c_str());
  }

  if (!dataFile) {
    return false;
  }

  if (server.hasArg("download")) {
    dataType = "application/octet-stream";
  }

  if (server.streamFile(dataFile, dataType) != dataFile.size()) {

  }

  dataFile.close();
  return true;
}

void handleFileUpload() {
  if (server.uri() != "/edit") {
    return;
  }
  HTTPUpload& upload = server.upload();
  if (upload.status == UPLOAD_FILE_START) {
    if (SD.exists((char *)upload.filename.c_str())) {
      SD.remove((char *)upload.filename.c_str());
    }
    uploadFile = SD.open(upload.filename.c_str(), FILE_WRITE);

  } else if (upload.status == UPLOAD_FILE_WRITE) {
    if (uploadFile) {
      uploadFile.write(upload.buf, upload.currentSize);
    }

  } else if (upload.status == UPLOAD_FILE_END) {
    if (uploadFile) {
      uploadFile.close();
    }

  }
}

void deleteRecursive(String path) {
  File file = SD.open((char *)path.c_str());
  if (!file.isDirectory()) {
    file.close();
    SD.remove((char *)path.c_str());
    return;
  }

  file.rewindDirectory();
  while (true) {
    File entry = file.openNextFile();
    if (!entry) {
      break;
    }
    String entryPath = path + "/" + entry.name();
    if (entry.isDirectory()) {
      entry.close();
      deleteRecursive(entryPath);
    } else {
      entry.close();
      SD.remove((char *)entryPath.c_str());
    }
    yield();
  }

  SD.rmdir((char *)path.c_str());
  file.close();
}

void handleDelete() {
  if (server.args() == 0) {
    return returnFail("BAD ARGS");
  }
  String path = server.arg(0);
  if (path == "/" || !SD.exists((char *)path.c_str())) {
    returnFail("BAD PATH");
    return;
  }
  deleteRecursive(path);
  returnOK();
}

void handleCreate() {
  if (server.args() == 0) {
    return returnFail("BAD ARGS");
  }
  String path = server.arg(0);
  if (path == "/" || SD.exists((char *)path.c_str())) {
    returnFail("BAD PATH");
    return;
  }

  if (path.indexOf('.') > 0) {
    File file = SD.open((char *)path.c_str(), FILE_WRITE);
    if (file) {
      file.write(0);
      file.close();
    }
  } else {
    SD.mkdir((char *)path.c_str());
  }
  returnOK();
}

void printDirectory() {
  if (!server.hasArg("dir")) {
    return returnFail("BAD ARGS");
  }
  String path = server.arg("dir");
  if (path != "/" && !SD.exists((char *)path.c_str())) {
    return returnFail("BAD PATH");
  }
  File dir = SD.open((char *)path.c_str());
  path = String();
  if (!dir.isDirectory()) {
    dir.close();
    return returnFail("NOT DIR");
  }
  dir.rewindDirectory();
  server.setContentLength(CONTENT_LENGTH_UNKNOWN);
  server.send(200, "text/json", "");
  WiFiClient client = server.client();

  server.sendContent("[");
  for (int cnt = 0; true; ++cnt) {
    File entry = dir.openNextFile();
    if (!entry) {
      break;
    }

    String output;
    if (cnt > 0) {
      output = ',';
    }

    output += "{\"type\":\"";
    output += (entry.isDirectory()) ? "dir" : "file";
    output += "\",\"name\":\"";
    output += entry.name();
    output += "\"";
    output += "}";
    server.sendContent(output);
    entry.close();
  }
  server.sendContent("]");
  dir.close();
}

void handleNotFound() {
  if (hasSD && loadFromSdCard(server.uri())) {
    return;
  }
  String message = "SDCARD Not Detected\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " NAME:" + server.argName(i) + "\n VALUE:" + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);

}
