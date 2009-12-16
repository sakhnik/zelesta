#include "logo.h"
#include "defs.h"
#include "display.h"

uint8_t logo_progress;
uint8_t logo_step_done;

const uint8_t logo_text[] =
{
    0x4f, // 3
    0x79, // E
    0x38, // L
    0x79, // E
    0x6d, // 5
    0x78, // t
    0x77  // A
};

void logo_init(void)
{
    logo_progress = 0;
    logo_step_done = 0;
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

    // Show the runningn logo
    for (digit = 0; digit != MAX_ANODES; ++digit)
    {
        idx = logo_progress + digit - MAX_ANODES;
        if (idx < 0 || idx >= ARRAY_SIZE(logo_text))
            display[digit] = 0;
        else
            display[digit] = logo_text[idx];
    }
    // Tell the caller if the logo finished
    return logo_progress <= ARRAY_SIZE(logo_text) + MAX_ANODES;
}
