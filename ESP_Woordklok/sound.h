#ifndef PAGE_SOUND_H
#define PAGE_SOUND_H
//
//   The EXAMPLE PAGE
//
const char PAGE_Sound[] PROGMEM = R"=====(
  <meta name="viewport" content="width=device-width, initial-scale=1" />
  <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
  <h1>My Example goes here</h1>     
    
   <div id="mydynamicdata">Here comes the Dynamic Data in </div>   <!-- added a DIV, where the dynamic data goes to -->
  
  <table border="0"  cellspacing="0" cellpadding="3" style="width:300px">
  <tr><form action="" method="post">
  <td align="center" colspan="2">Geluid Klok</td><tr>
    <td align="center"><input type='radio' id="Sound_ON" name='Sound' " value='ON'> ON</td>
    <td align="center"><input type='radio' id="Sound_OFF" name='Sound' " value='OFF'> OFF</td>
  </tr></table>
  <hr>
  <table border="0"  cellspacing="0" cellpadding="3" style="width:300px">
  <tr>
    <td align="center" colspan="2">Notatie Hoekpunten</td>
  </tr><tr>
    <td align="center"><input type='radio' id="Notat_1" name='Notat' " value='1'> 1 min</td>
    <td align="center"><input type='radio' id="Notat_5" name='Notat' " value='5'> 5 min</td>
  </tr></table>
  <hr>
  <table border="0"  cellspacing="0" cellpadding="3" style="width:300px">
  <tr>
    <td align="center" colspan="2">Licht Intensiteit</td>
  </tr><tr>
  <td align="center">Minimale</td><td align="center">Maximale</td></tr>
  </tr><tr>
    <td><input type='number' id="Light_Min" name='Light_Min'  min="1" max="100" value='1' pattern="[0-9]*" size='10'></td>
    <td><input type='number' id="Light_Max" name='Light_Max'  min="1" max="100" value='1' pattern="[0-9]*" size'='10'></td></tr>
  <tr><td align="center" colspan="2"><input type="submit" style="width:150px" class="btn btn--m btn--blue" value="Update light"></td></tr>
<tr><td align="center" colspan="2">Klok Tijd</td></tr>
 <tr><td><input type="time" name="Clock_Time" id="Clock_Time"></td></tr>
</form>
  
  </table>
  <hr>
   
   
   <script>                
		window.onload = function ()
		{
			load("style.css","css", function() 
			{
				load("microajax.js","js", function() 
				{
						setValues("/admin/filldynamicdataSound");  //-- this function calls the function on the ESP      
				});
			});
		}
		function load(e,t,n){if("js"==t){var a=document.createElement("script");a.src=e,a.type="text/javascript",a.async=!1,a.onload=function(){n()},document.getElementsByTagName("head")[0].appendChild(a)}else if("css"==t){var a=document.createElement("link");a.href=e,a.rel="stylesheet",a.type="text/css",a.async=!1,a.onload=function(){n()},document.getElementsByTagName("head")[0].appendChild(a)}}

   </script>

)=====";
#endif


void filldynamicdataSound()
{        
    String values ="";
    values += "mydynamicdata|" + (String) + "This is filled by AJAX. Millis since start: " + (String) millis() + "|div\n";   // Build a string, like this:  ID|VALUE|TYPE
    if (config.SoundOnOff == true) values += "Sound_ON| checked |chk\n";
    else values += "Sound_OFF| checked |chk\n";
    if (config.Notat == 1) values += "Notat_1| checked |chk\n";
    else values += "Notat_5| checked |chk\n";
    values += "Light_Min|" +  String(config.LMin) + "|input\n";
    values += "Light_Max|" +  String(config.LMax) + "|input\n";
    //values += "Clock_Time|" +  String(now()) + "|input\n";
    server.send ( 200, "text/plain", values);   
}

void processSound()
{     
    if (server.args() > 0 )  // Are there any POST/GET Fields ? 
    {
       for ( uint8_t i = 0; i < server.args(); i++ ) {  // Iterate through the fields
            if (server.argName(i) == "Notat") 
            {
                 // Your processing for the transmitted form-variable 
                 config.Notat = server.arg(i).toInt();
                 WriteClockConfig();
                 Serial.print("NOTAT");
            }
            if (server.argName(i) == "Light_Min") 
            {
                 // Your processing for the transmitted form-variable 
                 config.LMin = server.arg(i).toInt();
                 WriteClockConfig();
                 Serial.print("LMIN");
                 Serial.print(server.arg(i).toInt());
            }
            if (server.argName(i) == "Light_Max") 
            {
                 // Your processing for the transmitted form-variable 
                 config.LMax = server.arg(i).toInt();
                 WriteClockConfig();
                 Serial.print("LMAX");
            }
            if (server.argName(i) == "Sound") 
            {
                 // Your processing for the transmitted form-variable 
                 if (server.arg(i) == "ON") config.SoundOnOff = true;
                 else config.SoundOnOff = false;
                 WriteClockConfig();
                 Serial.print("CLICK ON");
            }
        }
    }
    server.send ( 200, "text/html", PAGE_Sound  ); 
}


