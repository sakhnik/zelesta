#include "clock.h"
#include "defs.h"
#include "display.h"
#include "charset.h"

uint8_t clock_h0;
uint8_t clock_m0; // low
uint8_t clock_m1; // high
uint8_t clock_s0; // low
uint8_t clock_s1; // high

// Update timestamp helps to keep digits visible when adjusting
uint8_t clock_adjust_time;

void clock_init(void)
{
    // Start the clock from 9:59:00
    clock_h0 = 9;
    clock_m1 = 5;
    clock_m0 = 9;
    clock_s1 = 0;
    clock_s0 = 0;

    clock_adjust_time = 0;
}

void clock_process(void)
{
    if (!clock_add_sec())
        return;
    if (!clock_add_min())
        return;
    clock_add_hr();
}

void clock_show_stopped(void)
{
    display[4] = charset[clock_s0];
    display[3] = charset[clock_s1];
    display[2] = charset[clock_m0];
    display[1] = charset[clock_m1];
    display[0] = charset[clock_h0];
    display_dots = 1;
}

void clock_show_running(void)
{
    // Must blink with dots once a second
    uint8_t hidden = ((jiffies - clock_adjust_time) >> 7) & 1;
    display[4] = charset[clock_s0];
    display[3] = charset[clock_s1];
    display[2] = charset[clock_m0];
    display[1] = charset[clock_m1];
    display[0] = charset[clock_h0];
    display_dots = 1 - hidden;
}

void clock_show_adjust_hr(void)
{
    // Must blink with hr digit twice a second
    uint8_t hidden = ((jiffies - clock_adjust_time) >> 6) & 1;
    display[4] = charset[clock_s0];
    display[3] = charset[clock_s1];
    display[2] = charset[clock_m0];
    display[1] = charset[clock_m1];
    display[0] = hidden ? CHAR_NONE : charset[clock_h0];
    display_dots = 1;
}

void clock_show_adjust_min(void)
{
    // Must blink with min digits twice a second
    uint8_t hidden = ((jiffies - clock_adjust_time) >> 6) & 1;
    display[4] = charset[clock_s0];
    display[3] = charset[clock_s1];
    display[2] = hidden ? CHAR_NONE : charset[clock_m0];
    display[1] = hidden ? CHAR_NONE : charset[clock_m1];
    display[0] = charset[clock_h0];
    display_dots = 1;
}

void clock_show_adjust_sec(void)
{
    // Must blink with sec digits twice a second
    uint8_t hidden = ((jiffies - clock_adjust_time) >> 6) & 1;
    display[4] = hidden ? CHAR_NONE : charset[clock_s0];
    display[3] = hidden ? CHAR_NONE : charset[clock_s1];
    display[2] = charset[clock_m0];
    display[1] = charset[clock_m1];
    display[0] = charset[clock_h0];
    display_dots = 1;
}

uint8_t clock_get_sec(void)
{
    return clock_s0 | (clock_s1 << 4);
}

uint8_t clock_get_min(void)
{
    return clock_m0 | (clock_m1 << 4);
}

uint8_t clock_add_hr(void)
{
    clock_adjust_time = jiffies;
    if (++clock_h0 < 10)
        return 0;
    clock_h0 = 0;
    return 1;
}

uint8_t clock_add_min(void)
{
    clock_adjust_time = jiffies;
    if (++clock_m0 < 10)
        return 0;
    clock_m0 = 0;
    if (++clock_m1 < 6)
        return 0;
    clock_m1 = 0;
    return 1;
}

uint8_t clock_add_sec(void)
{
    clock_adjust_time = jiffies;
    if (++clock_s0 < 10)
        return 0;
    clock_s0 = 0;
    if (++clock_s1 < 6)
        return 0;
    clock_s1 = 0;
    return 1;
}
