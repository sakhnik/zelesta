#include "keypad.h"
#include "defs.h"

// Keyboard driver is a bit generic
// Tuning details:
#define KEY_PORT                PORTB
#define KEY_DDR                 DDRB
#define KEY_PIN                 PINB
#define DECISION_TIME           10     // In jiffies, i. e. 1/256 of a second

#define KEY_UP_LEVEL            0
#define KEY_DOWN_LEVEL          (1 - KEY_UP_LEVEL)

#define KEY_START_REPEAT        0xff // Almost a second
#define KEY_NEXT_REPEAT         0x20 // 1/8 of second

// Data associated to every key
const uint8_t key_pin[] = { PB4, PB5 };
uint8_t key_state[] = { KEY_UP_LEVEL, KEY_UP_LEVEL };
uint8_t key_time[] = { 0, 0 };   // jiffies of the last update
uint8_t key_repeat[] = { 0, 0 }; // jiffies when to generate repeated event

#define KEY_COUNT               ARRAY_SIZE(key_pin)

// Prepare key pins for reading
void keypad_init(void)
{
    int i;
    for (i = 0; i != KEY_COUNT; ++i)
    {
        KEY_DDR &= ~_BV(key_pin[i]);
        KEY_PORT &= ~_BV(key_pin[i]);
    }
}

// Detect press events on current key,
// debounce it
uint8_t keypad_poll(void)
{
    uint8_t prev_state;
    uint8_t cur_state;
    // Currently polled key
    static uint8_t cur_key = 0;

    if (++cur_key >= KEY_COUNT)
        cur_key = 0;

    prev_state = key_state[cur_key];
    cur_state = bit_is_clear(KEY_PIN, key_pin[cur_key]);

    if (cur_state == prev_state)
    {
        // Nothing changed
        key_time[cur_key] = jiffies;
        // Check if it's time to generate next repeated press
        if (cur_state == KEY_DOWN_LEVEL &&
            jiffies == key_repeat[cur_key])
        {
            // Repeat four time a second
            key_repeat[cur_key] = jiffies + KEY_NEXT_REPEAT;
            return KEY_DOWN_REPEAT(cur_key);
        }
        return KEY_NO_EVENT;
    }
    if (jiffies - key_time[cur_key] < DECISION_TIME)
        // Wait until the key stabilizes
        return KEY_NO_EVENT;
    // Thus, we do have an event
    key_state[cur_key] = cur_state;
    key_time[cur_key] = jiffies;
    // Somehow it doesn't work if this is placed below
    key_repeat[cur_key] = jiffies + KEY_START_REPEAT;
    if (cur_state == KEY_UP_LEVEL)
        return KEY_UP_EVENT(cur_key);
    // Generate first repeat after a second
    return KEY_DOWN_EVENT(cur_key);
}
