

//
//  HTML PAGE
//

const char PAGE_Welcome[] PROGMEM = R"=====(
<meta name="viewport" content="width=device-width, initial-scale=1" />
<strong>Woordklok v0.8</strong>
<hr>
<a href="clock.html" style="width:250px" class="btn btn--m btn--blue" >Clock Settings</a><br>
<a href="Log.html"   style="width:250px"  class="btn btn--m btn--blue" >Clock Log File</a><br>
<a href="admin.html" style="width:250px" class="btn btn--m btn--blue" >Admin Settings</a><br>

<script>
window.onload = function ()
{
	load("style.css","css", function() 
	{
		load("microajax.js","js", function() 
		{
				// Do something after load...
		});
	});
}
function load(e,t,n){if("js"==t){var a=document.createElement("script");a.src=e,a.type="text/javascript",a.async=!1,a.onload=function(){n()},document.getElementsByTagName("head")[0].appendChild(a)}else if("css"==t){var a=document.createElement("link");a.href=e,a.rel="stylesheet",a.type="text/css",a.async=!1,a.onload=function(){n()},document.getElementsByTagName("head")[0].appendChild(a)}}

</script>

)=====";


