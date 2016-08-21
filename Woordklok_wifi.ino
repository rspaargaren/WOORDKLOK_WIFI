/*********
  Rui Santos
  Complete project details at http://randomnerdtutorials.com  
*********/

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

MDNSResponder mdns;

// Replace with your network credentials
const char* ssid = "XXXXXXXX";
const char* password = "XXXXXXX";

ESP8266WebServer server(80);

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

void setup(void){
  webPage += "<h1>ESP8266 WOORDKLOK</h1>";
  webPage += "<form action='Sound'>SOUND: <input type='radio' name='state' value='1' checked>On<input type='radio' name='state' value='0'>Off <input type='submit' value='Submit'></form>";
  webPage += "<form action='Notat'>NOTATIE: <input type='radio' name='state' value='1' checked>1 min<input type='radio' name='state' value='0'>5 min<input type='submit' value='Submit'></form>";
  webPage += "<form action='Time'>TIME: <input type='text' name='tijd' value='12:00:00'><input type='submit' value='Submit'></form>";
  webPage += "<form action='LMax'>LIGHT MAX: <input type='text' name='LMAX' value='90'><input type='submit' value='Submit'></form>";
  webPage += "<form action='LMin'>LIGHT MIN: <input type='text' name='LMIN' value='5'><input type='submit' value='Submit'></form>";
  webPage += "<form action='TComp'>Tijd Compensatie: <input type='text' name='TCOMP' value='-10'><input type='submit' value='Submit'></form>";
  webPage += "<form action='Modes'>";
  webPage += "Uit <input type='radio' name='mode' value='110' checked><br>";
  webPage += "Waterval <input type='radio' name='mode' value='120'><br>";
  webPage += "R -> L <input type='radio' name='mode' value='130'><br>";
  webPage += "L -> R <input type='radio' name='mode' value='140'><br>";
  webPage += "FADE OUT/IN <input type='radio' name='mode' value='150'><br>";
  webPage += "FADE IN <input type='radio' name='mode' value='160'><br>";
  webPage += "<input type='submit' value='Submit'></form>";
  
  delay(1000);
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  //Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    //Serial.print(".");
  }
  //Serial.println("");
  //Serial.print("Connected to ");
  //Serial.println(ssid);
  //Serial.print("IP address: ");
  //Serial.println(WiFi.localIP());
  
  if (mdns.begin("esp8266", WiFi.localIP())) {
    //Serial.println("MDNS responder started");
  }
  
  server.on("/", [](){
    server.send(200, "text/html", webPage);
  });
  server.on("/Sound", handle_sound);
  server.on("/Time", handle_time);
  server.on("/Modes", handle_modes);
  server.on("/Notat", handle_notat);
  server.on("/LMax", handle_lmax);
  server.on("/LMin", handle_lmin);
  server.on("/TComp", handle_tcomp);
  server.begin();
  //Serial.println("HTTP server started");
}
 
void loop(void){
  server.handleClient();
} 
