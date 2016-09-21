/*********
ESP WOORDKLOK 
*********/

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <WiFiManager.h>
#include <ESP8266HTTPUpdateServer.h>
#include <DNSServer.h>
#include <EEPROM.h>
#include <WiFiUdp.h>
//#include <ESP8266HTTPClient.h>
//#include <TimeLib.h>

MDNSResponder mdns;

// Replace with your network credentials
const char* host = "esp8266-webupdate";
const char* ssid = "XXXXXXXX";
const char* password = "XXXXXXXX";

// TIME SERVER PARAMETERS
unsigned int localPort = 2390;
IPAddress timeServerIP; // time.nist.gov NTP server address
const char* ntpServerName = "time.nist.gov";
const int NTP_PACKET_SIZE = 48; // NTP time stamp is in the first 48 bytes of the message
byte packetBuffer[ NTP_PACKET_SIZE]; //buffer to hold incoming and outgoing packets
WiFiUDP udp;

String Ser_Input;
String Clock_Time = "12:12:12";
String NTP_Time = "13:13:13";
String Clock_Light_rel;
String Clock_Light_abs;
String Clock_Mode = "110";
String Clock_TComp = "-10";
String Clock_Sound = "0";
String Debug1;
String Debug2;
String Debug3;

ESP8266WebServer server(80);
ESP8266HTTPUpdateServer httpUpdater;

String webPage = "";

void handle_sound() {
  // get the value of request argument "state" and convert it to an int
  int state = server.arg("state").toInt();
  if (state == 1) 
  Serial.println("SET SOUND 1");
  else
  Serial.println("SET SOUND 0");
  server.send(200, "text/html", webPage);
  delay(1000);
}

void handle_notat() {
  // get the value of request argument "state" and convert it to an int
  int state = server.arg("state").toInt();
  if (state == 1) 
  Serial.println("SET NOTAT 1");
  else
  Serial.println("SET NOTAT 5");
  server.send(200, "text/html", webPage);
  delay(1000);
}

void handle_time() {
  // get the value of request argument "state" and convert it to an int
  String tijd = server.arg("tijd");
  Serial.println("SET TIME " + tijd);
  server.send(200, "text/html", webPage);
  delay(1000);
}
void handle_lmax() {
  // get the value of request argument "state" and convert it to an int
  String lmax = server.arg("LMAX");
  Serial.println("SET LMAX " + lmax);
  server.send(200, "text/html", webPage);
  delay(1000);
}
void handle_lmin() {
  // get the value of request argument "state" and convert it to an int
  String lmin = server.arg("LMIN");
  Serial.println("SET LMIN " + lmin);
  server.send(200, "text/html", webPage);
  delay(1000);
}
void handle_tcomp() {
  // get the value of request argument "state" and convert it to an int
  String tcomp = server.arg("TCOMP");
  Serial.println("SET TCOMP " + tcomp);
  server.send(200, "text/html", webPage);
  delay(1000);
}
void handle_modes() {
  // get the value of request argument "state" and convert it to an int
  String state = server.arg("mode");
  Serial.println("SET MODE " + state);
  server.send(200, "text/html", webPage);
  delay(1000);
}

void handle_maninp() {
  // get the value of request argument "state" and convert it to an int
  String ManInput = server.arg("ManInput");
  Serial.println(ManInput);
  server.send(200, "text/html", webPage);
  delay(1000);
}

void handle_autotime() {
  // get the value of request argument "state" and convert it to an int
  int state = server.arg("mode").toInt();
  //Serial.println(ManInput);
  //Debug3 = state;
  if (state == 1) {
    EEPROM.write(300,1);
    Debug1 = "ONOFF IS ON";
  }
  else {
    EEPROM.write(300,0);
    Debug1 = "ONOFF iF OFF"; 
  }
  EEPROM.commit();
  server.send(200, "text/html", webPage);
  delay(1000);
  
}

void Read_Serial(){
 //   Serial.println("Ready");
    Serial.setTimeout(500);
    //delay(2000);
    //Serial.println("Read");
     //while(Serial.available()) {
      Ser_Input = Serial.readString();// read the incoming data as string
   //   Serial.println(Ser_Input);
  //}
}

 void getclocksettings() {
  // Get the settings of the clock by sending out some serial commands and waiting for the response
 //Ser_Input = Serial.readString();// read the incoming data as string and do nothing to clear the buffer!
  
 Serial.println("GET TIME");
 Read_Serial();
 Clock_Time = Ser_Input.substring(14,22);
 //Serial.println(Clock_Time);
 Serial.println("GET LIGHT");
 Read_Serial();
 Clock_Light_rel = Ser_Input.substring(29,32);
 Clock_Light_abs = Ser_Input.substring(35,38);
 //Serial.println(Clock_Light_rel);
 //Serial.println(Clock_Light_abs);
 Serial.println("GET MODE");
 Read_Serial();
 Clock_Mode = Ser_Input.substring(14,17);
 //Serial.println(Clock_Mode);
 Serial.println("GET TCOMP");
 Read_Serial();
 Clock_TComp = Ser_Input.substring(19,23);
 //Serial.println(Clock_TComp);
 Serial.println("GET SOUND");
 Read_Serial();
 Clock_Sound = Ser_Input.substring(14,21);
 if (Clock_Sound == "Enabled")
    Clock_Sound = "1";
 else
    Clock_Sound = "0";
 //Serial.println(Clock_Sound);
}
  
void Update_web(){
  webPage = "";
  webPage += "<h1>ESP8266 WOORDKLOK VERSIE OTA</h1>";
  if (Clock_Sound =="0")
  webPage += "<form action='Sound'>SOUND: <input type='radio' name='state' value='1'>On<input type='radio' name='state' value='0' checked>Off <input type='submit' value='Submit'></form>";
  else
  webPage += "<form action='Sound'>SOUND: <input type='radio' name='state' value='1' checked>On<input type='radio' name='state' value='0'>Off <input type='submit' value='Submit'></form>";
  // Geen uitlees mogelijkheden voor de notatie...
  webPage += "<form action='Notat'>NOTATIE: <input type='radio' name='state' value='1' checked>1 min<input type='radio' name='state' value='0'>5 min<input type='submit' value='Submit'></form>";
  webPage += "<form action='Time'>TIME: <input type='text' name='tijd' value='" + Clock_Time + "'><input type='submit' value='Submit'></form>";
  webPage += "<form action='Time'>INTERNET TIME: <input type='text' name='tijd' value='" + NTP_Time + "'><input type='submit' value='Submit'></form>";
  webPage += "<form action='Auto_Time'>";
  int Auto_Time_OnOff = EEPROM.read(300);
  Debug2 = Auto_Time_OnOff;
  if (Auto_Time_OnOff == 1){
    webPage += "AUTO TIME ON <input type='radio' name='autotime' value='1' checked><br>";
    webPage += "AUTO TIME OFF <input type='radio' name='autotime' value='0'><br>";
  }
  else
  {
    webPage += "AUTO TIME ON <input type='radio' name='mode' value='1' ><br>";
    webPage += "AUTO TIME OFF <input type='radio' name='mode' value='0' checked><br>";
  }
  webPage += "<input type='submit' value='Submit'></form>";
  webPage += "The Absolute Display Intensity is: " + Clock_Light_abs + "% <br>"; 
  webPage += "The Relative Display Intensity is: " + Clock_Light_rel + "% <br><br>";
  webPage += "<form action='LMax'>LIGHT MAX: <input type='text' name='LMAX' value='90'><input type='submit' value='Submit'></form>";
  webPage += "<form action='LMin'>LIGHT MIN: <input type='text' name='LMIN' value='5'><input type='submit' value='Submit'></form>";
  webPage += "<form action='TComp'>Tijd Compensatie: <input type='text' name='TCOMP' value='" + Clock_TComp + "'><input type='submit' value='Submit'></form>";
  webPage += "<form action='Modes'>";
  webPage += "Uit <input type='radio' name='mode' value='110' checked><br>";
  if (Clock_Mode == "120") 
    webPage += "Waterval <input type='radio' name='mode' value='120' checked><br>";
    else
    webPage += "Waterval <input type='radio' name='mode' value='120' ><br>";
  if (Clock_Mode == "130")
  webPage += "R -> L <input type='radio' name='mode' value='130' checked><br>";
  else 
  webPage += "R -> L <input type='radio' name='mode' value='130'><br>";
  if (Clock_Mode == "140")
  webPage += "L -> R <input type='radio' name='mode' value='140' checked><br>";
  else
  webPage += "L -> R <input type='radio' name='mode' value='140'><br>";
  if (Clock_Mode == "150")
  webPage += "FADE OUT/IN <input type='radio' name='mode' value='150' checked><br>";
  else
  webPage += "FADE OUT/IN <input type='radio' name='mode' value='150'><br>";
  if (Clock_Mode == "160")
  webPage += "FADE IN <input type='radio' name='mode' value='160' checked><br>";
  else
  webPage += "FADE IN <input type='radio' name='mode' value='160'><br>";
  webPage += "<input type='submit' value='Submit'></form>";
  webPage += "<form action='ManInp'>MANUAL INPUT: <input type='text' name='ManInput' value=''><input type='submit' value='Submit'></form>";
  webPage += "<form action='update'>Update ESP with new BIN file!<input type='submit' value='UPDATE'></form>";
  webPage += "<br> " + Debug1 + "<br>" + Debug2 + "<br>" + Debug3;
}



void setup(void){
  
  WiFiManager wifiManager;
  wifiManager.autoConnect("AutoConnectAP");
  
  MDNS.begin(host);
  MDNS.addService("http", "tcp", 80);
  
  EEPROM.begin(512);
//  int EEPROM_ADR = 100;
//EEPROM.write(100,1);  
  httpUpdater.setup(&server);

  //getclocksettings();
  //TIME SERVER START
  udp.begin(localPort);
  
  //Update_web();
  
  delay(1000);
  Serial.begin(9600);
  //**WiFi.begin(ssid, password);
  //Serial.println("");

  // Wait for connection
  //**while (WiFi.status() != WL_CONNECTED) {
  //**  delay(500);
    //Serial.print(".");
  //**}
  //Serial.println("");
  //Serial.print("Connected to ");
  //Serial.println(ssid);
  //Serial.print("IP address: ");
  //Serial.println(WiFi.localIP());
  
  //**if (mdns.begin("esp8266", WiFi.localIP())) {
    //Serial.println("MDNS responder started");
  //**}
  int Auto_Time_OnOff = EEPROM.read(300);
  if (Auto_Time_OnOff == 1){
    delay(1000); //Wait until clock is finished 
    getTime();
    Serial.println("SET TIME " + NTP_Time);
    delay(1000);
    Serial.println("SET TIME " + NTP_Time);
    Debug3 = ("SET TIME " + NTP_Time);
  }
  
  server.on("/", [](){
    getclocksettings();
    getTime();
    Update_web();
    server.send(200, "text/html", webPage);
  });
  server.on("/Sound", handle_sound);
  server.on("/Time", handle_time);
  server.on("/Modes", handle_modes);
  server.on("/Notat", handle_notat);
  server.on("/LMax", handle_lmax);
  server.on("/LMin", handle_lmin);
  server.on("/TComp", handle_tcomp);
  server.on("/ManInp", handle_maninp);
  server.on("/Auto_Time", handle_autotime);
    
  server.begin();
  //Serial.println("HTTP server started");
}
 
void loop(void){
  server.handleClient();
} 

void getTime(){
    //get a random server from the pool
  WiFi.hostByName(ntpServerName, timeServerIP); 

  sendNTPpacket(timeServerIP); // send an NTP packet to a time server
  // wait to see if a reply is available
  delay(1000);
  
  int cb = udp.parsePacket();
  if (!cb) {
  //  Serial.println("no packet yet");
  }
  else {
  //  Serial.print("packet received, length=");
  //  Serial.println(cb);
    // We've received a packet, read the data from it
    udp.read(packetBuffer, NTP_PACKET_SIZE); // read the packet into the buffer

    //the timestamp starts at byte 40 of the received packet and is four bytes,
    // or two words, long. First, esxtract the two words:

    unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
    unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
    // combine the four bytes (two words) into a long integer
    // this is NTP time (seconds since Jan 1 1900):
    unsigned long secsSince1900 = highWord << 16 | lowWord;
   // Serial.print("Seconds since Jan 1 1900 = " );
   // Serial.println(secsSince1900);

    // now convert NTP time into everyday time:
   // Serial.print("Unix time = ");
    // Unix time starts on Jan 1 1970. In seconds, that's 2208988800:
    const unsigned long seventyYears = 2208988800UL;
    // subtract seventy years:
    unsigned long epoch = secsSince1900 - seventyYears;
    // print Unix time:
    //Serial.println(epoch);
epoch = epoch + 7200; // Zomertijd correctie +3600 voor wintertijd

    // print the hour, minute and second:
    //Serial.print("The UTC time is ");       // UTC is the time at Greenwich Meridian (GMT)
    NTP_Time = "";
    if ( (((epoch  % 86400L) / 3600)) < 10 ) {
      // In the first 10 minutes of each hour, we'll want a leading '0'
      NTP_Time = "0";
    }
    NTP_Time = (((epoch  % 86400L) / 3600)); // print the hour (86400 equals secs per day)
    NTP_Time = NTP_Time + ":";
    if ( ((epoch % 3600) / 60) < 10 ) {
      // In the first 10 minutes of each hour, we'll want a leading '0'
      NTP_Time = NTP_Time + "0";
      //Serial.print('0');
    }
    NTP_Time = NTP_Time + ((epoch  % 3600) / 60); // print the minute (3600 equals secs per minute)
    NTP_Time = NTP_Time + ":";
    //Serial.print(':');
    if ( (epoch % 60) < 10 ) {
      // In the first 10 seconds of each minute, we'll want a leading '0'
    NTP_Time = NTP_Time + "0";
      //Serial.print('0');
    }
    NTP_Time = NTP_Time + (epoch % 60); // print the second
  }
}


unsigned long sendNTPpacket(IPAddress& address)
{
  //Serial.println("sending NTP packet...");
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;

  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  udp.beginPacket(address, 123); //NTP requests are to port 123
  udp.write(packetBuffer, NTP_PACKET_SIZE);
  udp.endPacket();
}
