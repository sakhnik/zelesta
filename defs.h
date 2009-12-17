#pragma once

#include <avr/io.h>

// Undef the macro ARTIFICIAL_JIFFIES if the jiffies can't be read
// from the timer counter directly

//#define ARTIFICIAL_JIFFIES
#ifdef ARTIFICIAL_JIFFIES
volatile uint8_t jiffies;
#else
#   define jiffies      TCNT2
#endif

#define ARRAY_SIZE(x)           (sizeof(x)/sizeof((x)[0]))
