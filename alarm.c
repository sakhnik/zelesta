#include "alarm.h"
#include "clock.h"
#include "beeper.h"
#include "display.h"
#include "charset.h"
#include <avr/eeprom.h>

#define ALARM_CFG_ADDR          0x13

enum e_alarm_config
{
    AC_NO_ALARM = 0,
    AC_30_SECS,
    AC_60_SECS,
    AC_120_SECS,

    AC_LAST
} alarm_config;

uint8_t EEMEM alarm_config_addr = AC_60_SECS;

void alarm_init(void)
{
    // Initialize the config from EEPROM
    alarm_config = eeprom_read_byte(&alarm_config_addr);
    if (alarm_config >= AC_LAST)
        alarm_config = AC_60_SECS;
}

#define SHORT_BEEP              beeper_set(220, 0x3f)
#define LONG_BEEP               beeper_set(440, 0xff)

void alarm_process(void)
{
    uint8_t secs = clock_get_sec();
    uint8_t mins = clock_get_min();

    // Depending on the current mode, we normalize seconds
    // as though the period were 60 seconds
    switch (alarm_config)
    {
    case AC_NO_ALARM:
        return;
    case AC_30_SECS:
        if (secs == 0x30 || !secs)
            secs = 0;
        else if (secs < 0x30)
            secs += 0x30;
        break;
    case AC_60_SECS:
        break;
    case AC_120_SECS:
        if (!(mins & 1) && !secs)
            break;
        if (!(mins & 1) || !secs)
            return;
        break;
    default:;
    }

    // Now we process all alarms as with period 60 seconds
    if (!secs)
        LONG_BEEP;
    else if (secs >= 0x55)
        SHORT_BEEP;
}

void alarm_show(void)
{
    display[0] = charset[0xa];
    display[1] = 0;
    display[2] = 0;
    display_dots = 0;
    switch (alarm_config)
    {
    case AC_NO_ALARM:
        display[3] = 0;
        display[4] = 0;
        return;
    case AC_30_SECS:
        display[3] = charset[3];
        display[4] = charset[0];
        return;
    case AC_60_SECS:
        display[3] = charset[6];
        display[4] = charset[0];
        return;
    case AC_120_SECS:
        display[2] = charset[1];
        display[3] = charset[2];
        display[4] = charset[0];
        return;
    default:;
    }
}

void alarm_adjust(void)
{
    if (++alarm_config == AC_LAST)
        alarm_config = AC_NO_ALARM;
    // Store config to EEPROM
    eeprom_write_byte(&alarm_config_addr, alarm_config);
}
