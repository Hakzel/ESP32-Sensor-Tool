void setup() {
  Wire.begin(I2C_SDA_PIN,I2C_SCL_PIN);        // join i2c bus (address optional for master)
  Wire.setClock(400000); // choose 400 kHz I2C rate
  
  display.init();
  display.setFont(ArialMT_Plain_10);
  display.flipScreenVertically(); // Adjust to suit or remove
  
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.drawString(64, 0, "Sensor-Tool V01");
  display.drawString(64, 15, "connect WiFi to:");
  display.drawString(64, 25, ssid);
  display.drawString(64, 35, password);
  display.drawString(64, 50, "© A Höffer");
  display.display();
  delay(1000);
  display.setTextAlignment(TEXT_ALIGN_LEFT);



//Sd2Card card;
  
  if(!SD.begin())
    {
      //display.drawString(20, 20, "SD-Card not found");
      //display.display();
      //delay(2000);
    }else{
      display.drawString(20, 20, "SD-Card found");
      display.display();
      delay(2000);
      hasSD = true;
      filetowrite += datafile += ".CSV";
    }
  
  
  /// initialize Webserver ///////////////////////////////////////////////

  // setup globals
  ulReqcount = 0;

  // AP mode
  //WiFi.mode(WIFI_AP);
  //WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  //WiFi.softAP(ssid, password);

  WiFi.softAP(ssid, password);        //configuration of the WiFi Access Point
  IPAddress IP = WiFi.softAPIP();

//  WiFi.mode(WIFI_STA);
//  WiFi.begin("WLAN24", "198994357017134733422568");

  

  
  server.begin();

  server.on("/", handleRoot);      //Which routine to handle at root location. This is display page
  server.on("/getPlot", handlePlot);
  server.on("/GetData", handleData);
  server.on("/GetString", handleString);
 


  
// //print plotPage
//  server.on("/plot", []() {
//    server.send(200, "text/html", getPlot());
//  });

//  //print LinkPage
//  server.on("/links", []() {
//    server.send(200, "text/html", getLinks());
//  });

//  // control refresh links:
//  server.on("/refreshlinks", []() {
//      root = SD.open("/");
//      getfilelinks ="";
//while(true) {
//     File entry =  root.openNextFile();
//     if (! entry) {
//       // no more files
//       break;
//     }
//     mystring = "";
//     mystring += entry.name();
//     if(mystring.endsWith(".CSV")){
//        getfilelinks += "<p><a href='";
//        getfilelinks += mystring;
//        getfilelinks += "' class='button'>";
//        getfilelinks += mystring;
//        getfilelinks += "</a></p>";
//      }
//     entry.close();
//     cnt++;    
//}
//cnt=1;
//    server.send(200, "text/html", getLinks());
//});


// things for data server on sd card


  server.on("/list", HTTP_GET, printDirectory);
  server.on("/edit", HTTP_DELETE, handleDelete);
  server.on("/edit", HTTP_PUT, handleCreate);
  server.on("/edit", HTTP_POST, []() {
    returnOK();
  }, handleFileUpload);
  server.onNotFound(handleNotFound);
  

}// end of setup loop
