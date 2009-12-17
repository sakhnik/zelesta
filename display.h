#pragma once

#include <stdint.h>

#define MAX_ANODES              5

// Video RAM
extern volatile uint8_t display[MAX_ANODES];
extern volatile uint8_t display_dots;

// Initialize the display
void display_init(void);

// Show next anode. Must be called at least 256 times per second.
void display_process(void);
