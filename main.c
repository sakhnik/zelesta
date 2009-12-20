#include "display.h"
#include "defs.h"
#include "beeper.h"
#include "keypad.h"
#include "app.h"
#include <avr/interrupt.h>
#include <util/delay.h>

#define TCNT0_START             (256 - 56)

#ifdef ARTIFICIAL_JIFFIES
volatile uint8_t jiffies = 0;
#endif

volatile uint8_t logo_done = 0;

ISR(TIMER0_OVF_vect)
{
    TCNT0 = TCNT0_START;
#ifdef ARTIFICIAL_JIFFIES
    ++jiffies;
#endif
    app_show();
    display_process();
    beeper_process();
}

ISR(TIMER2_OVF_vect)
{
    app_second();
}

void reset_clock_timer(void)
{
    TCNT2 = 0;
}

int main(void)
{
    display_init();
    beeper_init();
    keypad_init();
    app_init();

    TIMSK = 0;

    // TIMER0 is used to update the display
    TCCR0 = _BV(CS11) | _BV(CS10); // 64x prescaler
    TCNT0 = TCNT0_START;
    TIMSK |= _BV(TOIE0);

    // TIMER2 is used as real-time clock
    TCCR2 = _BV(CS22) | _BV(CS20); // 128x prescaler
    OCR2 = 0;
    ASSR = _BV(AS2);
    TCNT2 = 0;
    while ((ASSR & _BV(TCN2UB)) ||
           (ASSR & _BV(TCR2UB)) ||
           (ASSR & _BV(OCR2UB)))
        asm volatile("nop" ::);
    TIMSK |= _BV(TOIE2);

    sei();

    // Just beep
    beeper_set(440, 0xff);

    while (1)
    {
        uint8_t key = keypad_poll();
        switch (key)
        {
        case KEY_UP_EVENT(0):
            app_key_mode();
            break;
        case KEY_DOWN_EVENT(1):
            app_key_adjust();
            break;
        case KEY_DOWN_REPEAT(1):
            app_key_adjust_repeat();
            break;
        }
    }
}
