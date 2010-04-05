#pragma once

#include <stdint.h>

//#define DOT_ON(ch)          (ch & ~0x80)

#define CHAR_NONE       0x00

#define CHAR_0          0x3f
#define CHAR_1          0x06
#define CHAR_2          0x5b
#define CHAR_3          0x4f
#define CHAR_4          0x66
#define CHAR_5          0x6d
#define CHAR_6          0x7d
#define CHAR_7          0x07
#define CHAR_8          0x7f
#define CHAR_9          0x6f
#define CHAR_A          0x77
#define CHAR_B          0x7c
#define CHAR_C          0x39
#define CHAR_D          0x5e
#define CHAR_E          0x79
#define CHAR_F          0x71

#define CHAR_L          0x38
#define CHAR_N          0x37
#define CHAR_T          0x78

#define CHARSET_SIZE    16
extern const uint8_t charset[CHARSET_SIZE];
