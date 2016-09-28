/*********
ESP WOORDKLOK V0.1.0
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
#include <TimeLib.h>
#include<TimeAlarms.h>

MDNSResponder mdns;

const char* host = "esp8266-webupdate";
// Je kunt dit ook leeg laten, ik hoor graag jullie feedback!
//const char* ssid = "XXXXXXX";
//const char* password = "XXXXXXXX";

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
String Clock_Notat = "0";
int Clock_Lmax = 100;
int Clock_Lmin = 10;
String Debug1;
String Debug2;
String Debug3;
String Debug4;
String Debug5;
int Daylightsaving = 3600;

ESP8266WebServer server(80);
ESP8266HTTPUpdateServer httpUpdater;

String webPage = "";

void handle_sound() {
  // get the value of request argument "state" and convert it to an int
  int state = server.arg("state").toInt();
  if (state == 1) {
  Serial.println("SET SOUND 1");
  EEPROM.write(310,1);
  }
  else
  {
  Serial.println("SET SOUND 0");
  EEPROM.write(310,0);
  }
  EEPROM.commit();
  server.send(200, "text/html", webPage);
  delay(1000);
}

void handle_notat() {
  // get the value of request argument "state" and convert it to an int
  int state = server.arg("state").toInt();
  if (state == 1) {
  Serial.println("SET NOTAT 1");
  EEPROM.write(320,1);
  }
  else
  {
  Serial.println("SET NOTAT 5");
  EEPROM.write(320,5);
  }
  EEPROM.commit();
  server.send(200, "text/html", webPage);
  delay(1000);
}

void handle_time() {
  // get the value of request argument "tijd"
  String tijd = server.arg("tijd");
  Serial.println("SET TIME " + tijd);
  server.send(200, "text/html", webPage);
  delay(1000);
}
void handle_lmax() {
  // get the value of request argument "lmax"
  String lmax = server.arg("LMAX");
  Serial.println("SET LMAX " + lmax);
  EEPROM.write(330,lmax.toInt());
  EEPROM.commit();
  server.send(200, "text/html", webPage);
  delay(1000);
}
void handle_lmin() {
  // get the value of request argument "lmin"
  String lmin = server.arg("LMIN");
  Serial.println("SET LMIN " + lmin);
  EEPROM.write(340,lmin.toInt());
  EEPROM.commit();
  server.send(200, "text/html", webPage);
  delay(1000);
}
void handle_tcomp() {
  // get the value of request argument "tcomp"
  String tcomp = server.arg("TCOMP");
  Serial.println("SET TCOMP " + tcomp);
  server.send(200, "text/html", webPage);
  delay(1000);
}
void handle_modes() {
  // get the value of request argument "state"
  String state = server.arg("mode");
  Serial.println("SET MODE " + state);
  EEPROM.write(350,state.toInt());
  EEPROM.commit();
  Debug5 = String(EEPROM.read(350));
  server.send(200, "text/html", webPage);
  delay(1000);
}

void handle_maninp() {
  // get the value of request argument "maninput"
  String ManInput = server.arg("ManInput");
  Serial.println(ManInput);
  server.send(200, "text/html", webPage);
  delay(1000);
}

void handle_autotime() {
  // get the value of request argument "state"
  int state = server.arg("mode").toInt();
  String tijd = server.arg("tijd");
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
  //delay(1000);
  String tijd_uur = getValue(tijd, ':', 0);
  String tijd_min = getValue(tijd, ':', 1);
  Alarm.alarmRepeat(tijd_uur.toInt(),tijd_min.toInt(),1, UpdateClockTime);
  EEPROM.write(400,tijd_uur.toInt());
  EEPROM.write(410,tijd_min.toInt());
  EEPROM.commit();
}

void handle_test(){
      SetClockTime();
      FormatTime();
      server.send(200, "text/html", webPage);
}


void Read_Serial(){
    Serial.setTimeout(500);
    Ser_Input = Serial.readString();// read the incoming data as string
}

 void getclocksettings() {
 // Get the settings of the clock by sending out some serial commands and waiting for the response
 Ser_Input = Serial.readString();// read the incoming data as string and do nothing to clear the buffer!
 Serial.println("GET TIME");
 Read_Serial();
 Clock_Time = Ser_Input.substring(14,22);
 Serial.println("GET LIGHT");
 Read_Serial();
 Clock_Light_rel = Ser_Input.substring(29,32);
 Clock_Light_abs = Ser_Input.substring(35,38);
 Serial.println("GET TCOMP");
 Read_Serial();
 Clock_TComp = Ser_Input.substring(19,23);
 Serial.println("GET SOUND");
 Read_Serial();
 Clock_Sound = Ser_Input.substring(14,21);
 if (Clock_Sound == "Enabled")
    Clock_Sound = "1";
 else
    Clock_Sound = "0";
}
  
void Update_web(){
  webPage = "";
  webPage += "<h1>ESP8266 WOORDKLOK VERSIE AUTO SETTINGS</h1>";
  if (Clock_Sound =="0")
  webPage += "<form action='Sound'>SOUND: <input type='radio' name='state' value='1'>On<input type='radio' name='state' value='0' checked>Off <input type='submit' value='Submit'></form>";
  else
  webPage += "<form action='Sound'>SOUND: <input type='radio' name='state' value='1' checked>On<input type='radio' name='state' value='0'>Off <input type='submit' value='Submit'></form>";
  if (Clock_Notat =="1")
  webPage += "<form action='Notat'>NOTATIE: <input type='radio' name='state' value='1' checked>1 min<input type='radio' name='state' value='0'>5 min<input type='submit' value='Submit'></form>";
  else
  webPage += "<form action='Notat'>NOTATIE: <input type='radio' name='state' value='1' >1 min<input type='radio' name='state' value='0' checked>5 min<input type='submit' value='Submit'></form>";
  webPage += "<form action='Time'>CURRENT CLOCK TIME: <input type='text' name='tijd' value='" + Clock_Time + "'><input type='submit' value='Submit'></form>";
  webPage += "<form action='Time'>INTERNET TIME: <input type='text' name='tijd' value='" + NTP_Time + "'><input type='submit' value='Submit'></form>";
  webPage += "<form action='Auto_Time'>";
  int Auto_Time_OnOff = EEPROM.read(300);
  Debug2 = Auto_Time_OnOff;
  if (Auto_Time_OnOff == 1){
    webPage += "AUTO TIME ON <input type='radio' name='autotime' value='1' checked><br>";
    webPage += "AUTO TIME OFF <input type='radio' name='autotime' value='0'><br>";
    webPage += "AUTO UPDATE TIME <input type='text' name='tijd' value='"+ String(EEPROM.read(400)) + ":" + String(EEPROM.read(410)) +"'><br>" ;
  }
  else
  {
    webPage += "AUTO TIME ON <input type='radio' name='mode' value='1' ><br>";
    webPage += "AUTO TIME OFF <input type='radio' name='mode' value='0' checked><br>";
    webPage += "AUTO UPDATE TIME <input type='text' name='tijd' value='"+ String(EEPROM.read(400)) + ":" + String(EEPROM.read(410)) +"'><br>" ;
  }
  webPage += "<input type='submit' value='Submit'></form>";
  webPage += "The Absolute Display Intensity is: " + Clock_Light_abs + "% <br>"; 
  webPage += "The Relative Display Intensity is: " + Clock_Light_rel + "% <br><br>";
  webPage += "<form action='LMax'>LIGHT MAX: <input type='text' name='LMAX' value='" + String(Clock_Lmax) + "'><input type='submit' value='Submit'></form>";
  webPage += "<form action='LMin'>LIGHT MIN: <input type='text' name='LMIN' value='" + String(Clock_Lmin) + "'><input type='submit' value='Submit'></form>";
  webPage += "<form action='TComp'>Tijd Compensatie: <input type='text' name='TCOMP' value='" + Clock_TComp + "'><input type='submit' value='Submit'></form>";
  webPage += "<form action='Modes'>";
  if (Clock_Mode == "110") 
  webPage += "Uit <input type='radio' name='mode' value='110' checked><br>";
  else
  webPage += "Uit <input type='radio' name='mode' value='110' ><br>";
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
  if (EEPROM.read(400) == 255)  EEPROM.write(400,3);
  if (EEPROM.read(410) == 255)  EEPROM.write(410,0);
  EEPROM.commit();
  WiFiManager wifiManager;
  wifiManager.autoConnect("AutoConnectAP");
  
  MDNS.begin(host);
  MDNS.addService("http", "tcp", 80);
  
  EEPROM.begin(512);
  httpUpdater.setup(&server);

  udp.begin(localPort);
  
  delay(1000);
  Serial.begin(9600);
  Alarm.alarmRepeat(EEPROM.read(400),EEPROM.read(410),1, UpdateClockTime);
  InitClock();
  server.on("/", [](){
    getclocksettings();
    SetClockTime();
    FormatTime();
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
  server.on("/Test", handle_test);

  server.begin(); //("HTTP server started");
}
 
void loop(void){
  server.handleClient();
} 

void UpdateClockTime(){
  int Auto_Time_OnOff = EEPROM.read(300);
  if (Auto_Time_OnOff == 1){
    SetClockTime();
    FormatTime();
    Serial.println("SET TIME " + NTP_Time);
    Debug3 = "Last Clock Update at :" + String(hour()) + ":" + String(minute()) + ":" + String(second()) + "/" + String(year()) + "-" + String(month()) + "-" + String(day());
  }
}

void InitClock(){
int Auto_Time_OnOff = EEPROM.read(300);
  if (Auto_Time_OnOff == 1){
    delay(1000); //Wait until clock is finished 
    SetClockTime();
    FormatTime();
    Serial.println("SET TIME " + NTP_Time);
    delay(500);
    Serial.println("SET TIME " + NTP_Time);
    Debug3 = ("SET TIME " + NTP_Time);
  }
delay(100);
Clock_Sound = String(EEPROM.read(310));
if (Clock_Sound == "0") Serial.println("SET SOUND 0");
delay(100);
Clock_Notat = String(EEPROM.read(320));
if (Clock_Notat == "5") Serial.println("SET NOTAT 5");
delay(100);
Clock_Lmax = EEPROM.read(330);
Clock_Lmin = EEPROM.read(340);
if (Clock_Lmax > Clock_Lmin) Serial.println("SET LMAX 0" + String(Clock_Lmax));
delay(100);
if (Clock_Lmax > Clock_Lmin) {
  if (10 > Clock_Lmin) Serial.println("SET LMIN 00" + String(Clock_Lmin));
  if (100 > Clock_Lmin) Serial.println("SET LMIN 0" + String(Clock_Lmin));
}
int Clock_Mode_INT;
Clock_Mode_INT = EEPROM.read(350);
Debug5 = String(Clock_Mode_INT);
if (170 > Clock_Mode_INT) {
  Clock_Mode = String(Clock_Mode_INT);
  Serial.println("SET MODE " + Clock_Mode);
  Debug4 = Clock_Mode;
}
}

void SetClockTime(){
unsigned long Unixtime;
int var = 0;
do {
    delay(200);
    Unixtime=getNTPtime();
    var ++;
    if (var > 5){      // bail out as no ntptime is available now
       break;
    }
    }while (Unixtime==0);
if (Unixtime >0) {
  setTime(Unixtime+3600);
  setTime(Unixtime+adjustDstEurope());
  }
}

void FormatTime(){
  NTP_Time = "";
    if ( hour() < 10 ) {
      // In the first 10 minutes of each hour, we'll want a leading '0'
      NTP_Time = "0";
    }
    NTP_Time = NTP_Time + String(hour()); // print the hour (86400 equals secs per day)
    NTP_Time = NTP_Time + ":";
    if ( minute() < 10 ) {
      // In the first 10 minutes of each hour, we'll want a leading '0'
      NTP_Time = NTP_Time + "0";
    }
    NTP_Time = NTP_Time + String(minute()); // print the minute (3600 equals secs per minute)
    NTP_Time = NTP_Time + ":";
    if ( second() < 10 ) {
      // In the first 10 seconds of each minute, we'll want a leading '0'
    NTP_Time = NTP_Time + "0";
    }
    NTP_Time = NTP_Time + String(second()); // print the second
}

int adjustDstEurope()
{
  // last sunday of march
  int beginDSTDate=  (31 - (5* year() /4 + 4) % 7);
  Serial.println(beginDSTDate);
  int beginDSTMonth=3;
  //last sunday of october
  int endDSTDate= (31 - (5 * year() /4 + 1) % 7);
  Serial.println(endDSTDate);
  int endDSTMonth=10;
  // DST is valid as:
  if (((month() > beginDSTMonth) && (month() < endDSTMonth))
      || ((month() == beginDSTMonth) && (day() >= beginDSTDate)) 
      || ((month() == endDSTMonth) && (day() <= endDSTDate)))
  return 7200;  // DST europe = utc +2 hour
  else return 3600; // nonDST europe = utc +1 hour
}

unsigned long getNTPtime(){
    //get a random server from the pool
  WiFi.hostByName(ntpServerName, timeServerIP); 

  sendNTPpacket(timeServerIP); // send an NTP packet to a time server
  // wait to see if a reply is available
  //  delay(1000);
  
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
    //epoch = epoch + 7200;
    return epoch;
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

String getValue(String data, char separator, int index)
{
 int found = 0;
  int strIndex[] = {
0, -1  };
  int maxIndex = data.length()-1;
  for(int i=0; i<=maxIndex && found<=index; i++){
  if(data.charAt(i)==separator || i==maxIndex){
  found++;
  strIndex[0] = strIndex[1]+1;
  strIndex[1] = (i == maxIndex) ? i+1 : i;
  }
 }
  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}
