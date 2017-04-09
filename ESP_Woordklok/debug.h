#ifndef DEBUG_H
#define DEBUG_H

#define DEBUG 1
#ifdef DEBUG
#  define debug_print(x) Serial.println(x)
#  define debug_statement(x) x
#else
#  define debug_print(x)
#  define debug_statement(x)
#endif

#endif
