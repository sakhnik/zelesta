#include "app.h"
#include "charset.h"
#include "display.h"
#include "clock.h"
#include "alarm.h"
#include "beeper.h"
#include "logo.h"
#include "defs.h"

enum e_mode
{
    M_LOGO = 0,
    M_CLOCK,
    M_ADJUST_HR,
    M_ADJUST_MIN,
    M_ADJUST_SEC,
    M_ADJUST_ALARM,

    M_LAST
};

volatile enum e_mode app_mode;
volatile uint8_t app_running;

void app_init(void)
{
    app_mode = M_LOGO;
    app_running = 0;

    clock_init();
    alarm_init();
    logo_init();
}

void app_show(void)
{
    switch (app_mode)
    {
    case M_LOGO:
        if (!logo_show())
            app_key_mode();
        break;
    case M_CLOCK:
        if (app_running)
            clock_show_running();
        else
            clock_show_stopped();
        break;
    case M_ADJUST_HR:
        clock_show_adjust_hr();
        break;
    case M_ADJUST_MIN:
        clock_show_adjust_min();
        break;
    case M_ADJUST_SEC:
        clock_show_adjust_sec();
        break;
    case M_ADJUST_ALARM:
        alarm_show();
        break;
    default:;
    }
}

void app_second(void)
{
    if (!app_running)
        return;
    clock_process();
    alarm_process();
}

void app_key_mode(void)
{
    if (!app_running)
    {
        if (++app_mode == M_LAST)
            app_mode = M_CLOCK;
        return;
    }
    // Skip through clock adjustment when app_running
    if (app_mode == M_CLOCK)
        app_mode = M_ADJUST_ALARM;
    else if (app_mode == M_ADJUST_ALARM)
        app_mode = M_CLOCK;
}

void app_key_adjust(void)
{
    switch (app_mode)
    {
    case M_CLOCK:
        app_running = !app_running;
        //if (app_running)
        //    reset_clock_timer();
        beeper_set(880, 0x40);
        break;
    case M_ADJUST_HR:
        clock_add_hr();
        break;
    case M_ADJUST_MIN:
        clock_add_min();
        break;
    case M_ADJUST_SEC:
        clock_add_sec();
        break;
    case M_ADJUST_ALARM:
        alarm_adjust();
        break;
    default:;
    }
}

void app_key_adjust_repeat(void)
{
    switch (app_mode)
    {
    case M_ADJUST_HR:
        clock_add_hr();
        break;
    case M_ADJUST_MIN:
        clock_add_min();
        break;
    case M_ADJUST_SEC:
        clock_add_sec();
        break;
    default:;
    }
}
