#pragma once

// Initialize the alarm clock
void alarm_init(void);

// Check every second if it's time to make a noise
void alarm_process(void);

// Display alarm settings
void alarm_show(void);

// Cycle through available alarm configs
void alarm_adjust(void);
