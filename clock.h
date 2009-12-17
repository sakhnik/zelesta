#pragma once

#include "defs.h"

// Initialize the chornometr
void clock_init(void);

// Must be called once a second
void clock_process(void);

// Show various appearances of the clock
void clock_show_stopped(void);
void clock_show_running(void);
void clock_show_adjust_hr(void);
void clock_show_adjust_min(void);
void clock_show_adjust_sec(void);

// Get packet BCD representation of seconds
uint8_t clock_get_sec(void);

// Get packet BCD represenation of minutes
uint8_t clock_get_min(void);

// Add one hour to the clock
// Returns 1 in case of overflow, 0 otherwise
uint8_t clock_add_hr(void);

// Add one minute to the clock
// Returns 1 in case of overflow, 0 otherwise
uint8_t clock_add_min(void);

// Add one second to the clock
// Returns 1 in case of overflow, 0 otherwise
uint8_t clock_add_sec(void);
