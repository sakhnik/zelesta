#include "logo.h"
#include "display.h"
#include "defs.h"
#include "charset.h"
#include <avr/eeprom.h>

uint8_t EEMEM logo_enabled_addr;
volatile uint8_t logo_enabled;
uint8_t logo_progress;
uint8_t logo_step_done;

const uint8_t logo_text[] =
{
    CHAR_3,
    CHAR_E,
    CHAR_L,
    CHAR_E,
    CHAR_5,
    CHAR_T,
    CHAR_A
};

void logo_init(void)
{
    logo_enabled = eeprom_read_byte(&logo_enabled_addr);
    logo_progress = 0;
    logo_step_done = 0;
    if (!logo_enabled)
        logo_progress = 0x7f;  // Big number as if the logo has finished
}

static uint8_t logo_process(void)
{
    uint8_t step = jiffies & (1 << 7);
    if (step == logo_step_done)
        return 0;
    logo_step_done = step;
    ++logo_progress;
    return 1;
}

uint8_t logo_show(void)
{
    uint8_t digit;
    int8_t idx;

    if (!logo_process())
        return 1;

    // Tell the caller if the logo finished
    if (logo_progress > ARRAY_SIZE(logo_text) + MAX_ANODES)
        return 0;

    // Show the running logo
    for (digit = 0; digit != MAX_ANODES; ++digit)
    {
        idx = logo_progress + digit - MAX_ANODES;
        if (idx < 0 || idx >= ARRAY_SIZE(logo_text))
            display[digit] = CHAR_NONE;
        else
            display[digit] = logo_text[idx];
    }
    return 1;
}

void logo_show_adjust(void)
{
    if (logo_enabled)
    {
        display[0] = CHAR_L;
        display[1] = CHAR_NONE;
        display[2] = CHAR_NONE;
        display[3] = CHAR_0;
        display[4] = CHAR_N;
    }
    else
    {
        display[0] = CHAR_L;
        display[1] = CHAR_NONE;
        display[2] = CHAR_0;
        display[3] = CHAR_F;
        display[4] = CHAR_F;
    }
}

void logo_adjust(void)
{
    logo_enabled = !logo_enabled;
    eeprom_write_byte(&logo_enabled_addr, logo_enabled);
}
