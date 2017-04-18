#define LOG_H_INCLUDED 1
#include "log.h"
#include "FS.h"
#include <Time.h>

#define LOGFILE "/data.txt"

void ResetLogFile() {
	File bestand = SPIFFS.open(LOGFILE, "w"); // open het bestand in schrijf modus.
	bestand.println("New Logfile created on: " + String(hour()) + ":" + String(minute())
		+ ":" + String(second()) + "---" + String(year()) + "/"
		+ String(month()) + "/" + String(day()));
	bestand.close();
}

void WriteLogLine(const String &logLine) {
	File bestand = SPIFFS.open(LOGFILE, "a+"); // open het bestand in schrijf modus.
	bestand.println(String(hour()) + ":" + String(minute()) + ":" + String(second()) + " - " + logLine);
	bestand.close();
}
