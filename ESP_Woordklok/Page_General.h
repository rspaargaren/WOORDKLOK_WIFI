//
//  HTML PAGE
//

const char PAGE_AdminGeneralSettings[] PROGMEM = R"=====(
<meta name="viewport" content="width=device-width, initial-scale=1" />
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<a href="admin.html"  class="btn btn--s"><</a>&nbsp;&nbsp;<strong>General Settings</strong>
<hr>
<form action="" method="post">
<table border="0"  cellspacing="0" cellpadding="3" >
<tr>
	<td align="right">Name of Device</td>
	<td><input type="text" id="devicename" name="devicename" value=""></td>
</tr>

<tr>
	<td align="left" colspan="2"><hr></td>
</tr>

<tr>
	<td align="left" colspan="2">UPDATE CLOCK TIJD OM</td>
</tr>
<tr>
	<td align="right"> Enabled:</td>
	<td><input type="checkbox" id="tonenabled" name="tonenabled"></td>
</tr>

<tr>
	<td align="right"> Time:</td>
	<td><input type="text" id="tonhour" name="tonhour" size="2" value="00">:<input type="text" id="tonminute" name="tonminute" size="2" value="00"></td>
</tr>
<tr>
  <td align="left" colspan="2"><hr></td>
</tr>
<tr>
  <td align="right"> Update Clock at Start-UP?:</td>
  <td><input type="checkbox" id="Update_Start" name="Update_Start"></td>
</tr>
<tr>
  <td align="left" colspan="2"><hr></td>
</tr>
<tr>
  <td align="right"> Update Clock at NTP Update?:</td>
  <td><input type="checkbox" id="Update_NTP" name="Update_NTP"></td>
</tr>
<tr>
  <td align="left" colspan="2"><hr></td>
</tr>

<tr><td align="right">LOG CLOCK TIME EVERY:</td><td><input type='number' id="Get_Time" name='Get_Time'  min="0" max="60" value='0' pattern="[0-9]*" > minutes (0=disable)</td></tr>
<tr>
  <td align="left" colspan="2"><hr></td>
</tr>

<tr>
	<td align="left" colspan="2">GEEN FUNCTIE</td>
<tr>
	<td align="right"> Enabled:</td>
	<td><input type="checkbox" id="toffenabled" name="toffenabled"></td>
</tr>
<tr>
	<td align="right"> Time:</td>
	<td><input type="text" id="toffhour" name="toffhour" size="2" value="00">:<input type="text" id="toffminute" name="toffminute" size="2" value="00"></td>
</tr>
<tr><td colspan="2" align="center"><input type="submit" style="width:150px" class="btn btn--m btn--blue" value="Save"></td></tr>
</table>
<table border="0"  cellspacing="0" cellpadding="3" >
<tr><td colspan="2" align="center"><input type="submit" id="bGet_Touch" name='bGet_Touch' style="width:150px" class="btn btn--m btn--blue" value="GETTOUCH"></td></tr>
<tr><td colspan="2" align="center"><input type="submit" id="bGet_Light" name='bGet_Light' style="width:150px" class="btn btn--m btn--blue" value="GETLIGHT"></td></tr>
<tr><td colspan="2" align="center"><input type="submit" id="bGet_Mode" name='bGet_Mode' style="width:150px" class="btn btn--m btn--blue" value="GETMODE"></td></tr>
<tr><td colspan="2" align="center"><input type="submit" id="bGet_Tcomp" name='bGet_Tcomp' style="width:150px" class="btn btn--m btn--blue" value="GETTCOMP"></td></tr>
<tr><td colspan="2" align="center"><input type="submit" id="bGet_Sound" name='bGet_Sound' style="width:150px" class="btn btn--m btn--blue" value="GETSOUND"></td></tr>
<tr><td colspan="2" align="center"><input type="submit" id="bGet_Time" name='bGet_Time' style="width:150px" class="btn btn--m btn--blue" value="GETTIME"></td></tr>
<tr><td> <input type="text" id="man_input" name="man_input" value=""></td><td  align="center"><input type="submit" id="bManual" name='bManual' style="width:150px" class="btn btn--m btn--blue" value="GETMAN"></td></tr>



</table>
</form>
<script>

 

window.onload = function ()
{
	load("style.css","css", function() 
	{
		load("microajax.js","js", function() 
		{
				setValues("/admin/generalvalues");
		});
	});
}
function load(e,t,n){if("js"==t){var a=document.createElement("script");a.src=e,a.type="text/javascript",a.async=!1,a.onload=function(){n()},document.getElementsByTagName("head")[0].appendChild(a)}else if("css"==t){var a=document.createElement("link");a.href=e,a.rel="stylesheet",a.type="text/css",a.async=!1,a.onload=function(){n()},document.getElementsByTagName("head")[0].appendChild(a)}}



</script>
)=====";

// Functions for this Page
void send_devicename_value_html() {

	String values = "";
	values += "devicename|" + (String) config.DeviceName + "|div\n";
	server.send(200, "text/plain", values);
	//Serial.println(__FUNCTION__); 

}

void send_general_html() {

	if (server.args() > 0)  // Save Settings
			{
		config.AutoTurnOn = false;
		config.AutoTurnOff = false;
		config.AutoStart = false;
		config.Clock_NTP_Update = false;
		String temp = "";
		for (uint8_t i = 0; i < server.args(); i++) {
			if (server.argName(i) == "devicename")
				config.DeviceName = urldecode(server.arg(i));
			if (server.argName(i) == "tonenabled")
				config.AutoTurnOn = true;
			if (server.argName(i) == "toffenabled")
				config.AutoTurnOff = true;
			if (server.argName(i) == "tonhour")
				config.TurnOnHour = server.arg(i).toInt();
			if (server.argName(i) == "tonminute")
				config.TurnOnMinute = server.arg(i).toInt();
			if (server.argName(i) == "toffhour")
				config.TurnOffHour = server.arg(i).toInt();
			if (server.argName(i) == "toffminute")
				config.TurnOffMinute = server.arg(i).toInt();
			if (server.argName(i) == "Update_Start")
				config.AutoStart = true;
			if (server.argName(i) == "Update_NTP")
				config.Clock_NTP_Update = true;
			if (server.argName(i) == "Get_Time")
				config.GetTimeMinute = server.arg(i).toInt();
			if (server.argName(i) == "bGet_Touch") {
				Serial.println("GET TOUCH");
				WriteLogLine("GET TOUCH");
			}
			if (server.argName(i) == "bGet_Sound") {
				Serial.println("GET SOUND");
				WriteLogLine("GET SOUND");
			}
			if (server.argName(i) == "bGet_Light") {
				Serial.println("GET LIGHT");
				WriteLogLine("GET LIGHT");
			}
			if (server.argName(i) == "bGet_Mode") {
				Serial.println("GET MODE");
				WriteLogLine("GET MODE");
			}
			if (server.argName(i) == "bGet_Tcomp") {
				Serial.println("GET TCOMP");
				WriteLogLine("GET TCOMP");
			}
			if (server.argName(i) == "bGet_Time") {
				Serial.println("GET TIME");
				WriteLogLine("GET TIME");
			}
			if (server.argName(i) == "bManual") {
				Serial.println(String(server.arg("man_input")));
				WriteLogLine(String(server.arg("man_input")));
			}
		}
		WriteConfig();
		//firstStart = true;
	}
	server.send_P(200, CONTENT_Html, PAGE_AdminGeneralSettings);
	//Serial.println(__FUNCTION__); 

}

void send_general_configuration_values_html() {
	String values = "";
	values += "devicename|" + (String) config.DeviceName + "|input\n";
	values += "tonhour|" + (String) config.TurnOnHour + "|input\n";
	values += "tonminute|" + (String) config.TurnOnMinute + "|input\n";
	values += "toffhour|" + (String) config.TurnOffHour + "|input\n";
	values += "toffminute|" + (String) config.TurnOffMinute + "|input\n";
	values += "toffenabled|" + (String) (config.AutoTurnOff ? "checked" : "")
			+ "|chk\n";
	values += "tonenabled|" + (String) (config.AutoTurnOn ? "checked" : "")
			+ "|chk\n";
	values += "Update_Start|" + (String) (config.AutoStart ? "checked" : "")
			+ "|chk\n";
	values += "Update_NTP|" + (String) (config.Clock_NTP_Update ? "checked" : "")
			+ "|chk\n";
	values += "Get_Time|" + (String) config.GetTimeMinute + "|input\n";

	server.send(200, "text/plain", values);
	//Serial.println(__FUNCTION__); 
}
