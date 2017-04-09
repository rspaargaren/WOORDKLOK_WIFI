#ifndef LOG_H
#define LOG_H

#include "FS.h"

//#define DEBUG 1
//#define DEBUG_MEMORY 1
#ifdef DEBUG
#  define debug_print(x) Serial.println(x)
#  define debug_printf(fmt, ...) Serial.printf(fmt, __VA_ARGS__)
#  define debug_statement(x) x
#else
#  define debug_print(x)
#  define debug_printf(fmt, ...)
#  define debug_memory_printf(fmt, ...)
#  define debug_statement(x)
#endif
#ifdef DEBUG_MEMORY
#  define debug_memory_printf(fmt, ...) Serial.printf(fmt, __VA_ARGS__)
#else
#  define debug_memory_printf(fmt, ...)
#endif

void WriteLogLine(String LogLine) {
	File bestand = SPIFFS.open("/data.txt", "a+"); // open het bestand in schrijf modus.
	bestand.println(String(hour()) + ":" + String(minute()) + ":" + String(second()) + " - " + LogLine);
	bestand.close();
}

#endif
