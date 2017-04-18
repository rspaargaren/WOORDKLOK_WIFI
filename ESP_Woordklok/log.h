#ifndef LOG_H
#define LOG_H

#include <WString.h>

//#define DEBUG 1
//#define DEBUG_MEMORY 1
//#define DEBUG_WIFICONTROLLER 1

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
#ifdef DEBUG_WIFICONTROLLER
#  define debug_wc_print(x) Serial.println(x)
#else
#  define debug_wc_print(fx)
#endif

void WriteLogLine(const String &LogLine);
void ResetLogFile();

#endif
