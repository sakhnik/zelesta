#pragma once

#include <stdint.h>

// Initialize the keypad
void keypad_init(void);

// Keypad events. These macro define events for key #i
#define KEY_NO_EVENT            0
#define KEY_DOWN_EVENT(i)       (((i) << 2) + 1)
#define KEY_UP_EVENT(i)         (KEY_DOWN_EVENT(i) + 1)
#define KEY_DOWN_REPEAT(i)      (KEY_DOWN_EVENT(i) + 2)

// Check if the keypad state has changed, return event number.
uint8_t keypad_poll(void);
