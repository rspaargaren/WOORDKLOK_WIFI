/* 
  Versie V0.1 Nieuw platform
  
  ESP_Woordklok 

  Copyright (c) 2016 Richard Spaargaren. All rights reserved.
  This is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.
  This software is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.
  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
  */


#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <Ticker.h>
#include <EEPROM.h>
#include <WiFiUdp.h>
#include <ESP8266HTTPUpdateServer.h>
#include "helpers.h"
#include "global.h"
#include <TimeLib.h>
#include "FS.h"
/*
Include the HTML, STYLE and Script "Pages"
*/
#include "Page_Root.h"
#include "Page_Admin.h"
#include "Page_Script.js.h"
#include "Page_Style.css.h"
#include "Page_NTPsettings.h"
#include "Page_Information.h"
#include "Page_General.h"
#include "PAGE_NetworkConfiguration.h"
#include "example.h"
#include "clock.h"
#include "Page_Welcome.h"


#define ACCESS_POINT_NAME  "WOORDKLOK"				
#define ACCESS_POINT_PASSWORD  "12345678" 
#define AdminTimeOut 240

ESP8266HTTPUpdateServer httpUpdater;


void setup ( void ) {
	EEPROM.begin(512);
	Serial.begin(9600);
  SPIFFS.begin();
	delay(500);
  WriteLogLine("ESP init: setting up clock.");
	//Serial.println("Starting ES8266");
	if (!ReadConfig())
	{
		// DEFAULT CONFIG
		config.ssid = "MYSSID";
		config.password = "MYPASSWORD";
		config.dhcp = true;
		config.IP[0] = 192;config.IP[1] = 168;config.IP[2] = 1;config.IP[3] = 100;
		config.Netmask[0] = 255;config.Netmask[1] = 255;config.Netmask[2] = 255;config.Netmask[3] = 0;
		config.Gateway[0] = 192;config.Gateway[1] = 168;config.Gateway[2] = 1;config.Gateway[3] = 1;
		config.ntpServerName = "0.de.pool.ntp.org";
		config.Update_Time_Via_NTP_Every =  0;
		config.timezone = -10;
		config.daylight = true;
		config.DeviceName = "Not Named";
		config.AutoTurnOff = false;
		config.AutoTurnOn = false;
		config.TurnOffHour = 0;
		config.TurnOffMinute = 0;
		config.TurnOnHour = 0;
		config.TurnOnMinute = 0;
    config.AutoStart = false;
    config.Clock_NTP_Update = false;
    config.GetTimeMinute = 0;
		WriteConfig();
    config.SoundOnOff = false;
    config.Notat = 1;
    config.LMin = 5;
    config.LMax = 100;
    config.ClockMode = 150;
    config.TouchOnOff = true;
    config.TouchFil = 5;
    config.TouchTrL = 15;
    config.TouchTrH = 30;
    config.TouchTiS = 10;
    config.TouchTiL = 100;
    WriteClockConfig;
		//Serial.println("General config applied");
	}
	ReadClockConfig();
	
	if (AdminEnabled)
	{
		WiFi.mode(WIFI_AP_STA);
		WiFi.softAP( ACCESS_POINT_NAME , ACCESS_POINT_PASSWORD);
	}
	else
	{
		WiFi.mode(WIFI_STA);
	}

	ConfigureWifi();
	
  httpUpdater.setup(&server);
	server.on ( "/", []() { server.send ( 200, "text/html", PAGE_Welcome );   }  );
	server.on ( "/admin/filldynamicdataClock", filldynamicdataClock );
  
	server.on ( "/favicon.ico",   []() { server.send ( 200, "text/html", "" );   }  );


	server.on ( "/admin.html", []() { server.send ( 200, "text/html", PAGE_AdminMainPage );   }  );
	server.on ( "/config.html", send_network_configuration_html );
	server.on ( "/info.html", []() {  server.send ( 200, "text/html", PAGE_Information );   }  );
	server.on ( "/ntp.html", send_NTP_configuration_html  );
	server.on ( "/general.html", send_general_html  );
//	server.on ( "/example.html", []() { server.send ( 200, "text/html", PAGE_EXAMPLE );  } );
	server.on ( "/style.css", []() { server.send ( 200, "text/plain", PAGE_Style_css );  } );
	server.on ( "/microajax.js", []() { server.send ( 200, "text/plain", PAGE_microajax_js );  } );
	server.on ( "/admin/values", send_network_configuration_values_html );
	server.on ( "/admin/connectionstate", send_connection_state_values_html );
	server.on ( "/admin/infovalues", send_information_values_html );
	server.on ( "/admin/ntpvalues", send_NTP_configuration_values_html );
	server.on ( "/admin/generalvalues", send_general_configuration_values_html);
	server.on ( "/admin/devicename",     send_devicename_value_html);
  server.on ( "/clock.html", processClock ); 
  server.on ( "/Log.html", handle_log ); 
  server.on ( "/ResetLog.html", handle_reset ); 

 

	server.onNotFound ( []() {  server.send ( 400, "text/html", "Page not Found" );   }  );
	server.begin();
	//Serial.println( "HTTP server started" );
	tkSecond.attach(1,Second_Tick);
}


void handle_log(){
  File bestand = SPIFFS.open("/data.txt", "r");
  size_t sent = server.streamFile(bestand, "text/plain;charset=UTF-8");
  bestand.close();
}

void handle_reset(){
//    File bestand = SPIFFS.open("/data.txt", "w"); // open het bestand in schrijf modus.
//    bestand.println("New Logfile created on: " + String(hour()) + ":" + String(minute()) + ":" + String(second())+ "---" + String(year()) + "/" + String(month()) + "/" + String(day()));
//    bestand.close();
    //bestand = SPIFFS.open("/data.txt", "r");
    //size_t sent = server.streamFile(bestand, "text/plain");
    //bestand.close();
    ResetLogFile();
    server.sendHeader("Location","/Log.html");
    server.send(302, "text/plain","OK");
}

void loop ( void ) {
  
	if (AdminEnabled)
	{
		if (AdminTimeOutCounter > AdminTimeOut)
		{
			 AdminEnabled = false;
			 //Serial.println("Admin Mode disabled!");
			 WiFi.mode(WIFI_STA);
		}
	}
	if (config.Update_Time_Via_NTP_Every  > 0 )
	{
		if (cNTP_Update > 5 && firstStart)
		{
			boolean refresh = NTPRefresh();
      if (!refresh) {
        WriteLogLine("NTP failure. Clock might be out of time...");
      }
			cNTP_Update =0;
			firstStart = false;
      setTime(UnixTimestamp); //Convert to TimeLIB Library First Time the time is set af power up!
      UnixTimestamp_adjusted = UnixTimestamp + (config.timezone *  360);
      if (config.daylight) UnixTimestamp_adjusted = UnixTimestamp_adjusted + adjustDstEurope();
      setTime(UnixTimestamp_adjusted); //
      WriteLogLine("start clock settings");
      if (config.AutoStart) Update_Clock_Settings();
		}
		else if ( cNTP_Update > (config.Update_Time_Via_NTP_Every * 60) )
		{
      cNTP_Update = 0;
      if ( NTPRefresh() ) {
        UnixTimestamp_adjusted = UnixTimestamp + (config.timezone *  360);
        if (config.daylight) {
          UnixTimestamp_adjusted = UnixTimestamp_adjusted + adjustDstEurope();
        }
        WriteLogLine("NTP TIME UPDATED"); 
        setTime(UnixTimestamp_adjusted); //Convert to TimeLIB Library

        if (config.Clock_NTP_Update) {
          Serial.println ("SET TIME " + FormatTime(hour()) + ":" + FormatTime(minute()) + ":" + FormatTime(second()) );
          WriteLogLine ("AUTO NTP SET TIME " + FormatTime(hour()) + ":" + FormatTime(minute()) + ":" + FormatTime(second()) );
        }
      } else {
        WriteLogLine("NTP FAILED UPDATE");        
      }
		}
	}

  if (config.GetTimeMinute  > 0 )
  {
    if ( cGet_Time_Update > (config.GetTimeMinute * 60))
    {
      cGet_Time_Update = 0;
      Serial.println("GET TIME");
      WriteLogLine("GET TIME ");
    }
  }

  



  
	if(minute() != Minute_Old)
	{
		 Minute_Old = minute();
		 if (config.AutoTurnOn)
		 {
			 if (hour() == config.TurnOnHour && minute() == config.TurnOnMinute)
			 {
           Serial.println ("SET TIME " + FormatTime(hour()) + ":" + FormatTime(minute()) + ":" + FormatTime(second()) );
           WriteLogLine ("SET TIME " + FormatTime(hour()) + ":" + FormatTime(minute()) + ":" + FormatTime(second()) );
			 }
		 }


		 Minute_Old = minute();
		 if (config.AutoTurnOff)
		 {
			 if (hour() == config.TurnOffHour && minute() == config.TurnOffMinute)
			 {
				  //Serial.println("SwitchOff");
			 }
		 }
	}
	server.handleClient();


	/*
	*    Your Code here
	*/

  //check UART for data
  while(Serial.available()){
    Serial.setTimeout(75);
    String Ser_Input = Serial.readString();
    WriteLogLine("> " + Ser_Input);
  }

	if (Refresh)  
	{
		Refresh = false;
		///Serial.println("Refreshing...");
		 //Serial.printf("FreeMem:%d %d:%d:%d %d.%d.%d \n",ESP.getFreeHeap() , DateTime.hour,DateTime.minute, DateTime.second, DateTime.year, DateTime.month, DateTime.day);
	}



 


}

