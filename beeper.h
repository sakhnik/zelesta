#pragma once

#include <stdint.h>

// Initialize the PWM module
void beeper_init(void);

// Produce a sound:
//   freq — in Hz
//   duration — in jiffies
void beeper_set(uint16_t frequency,
                uint16_t duration);

// Service routine
void beeper_process(void);
