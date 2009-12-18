#include "display.h"
#include "logo.h"
#include "defs.h"
#include "clock.h"
#include "charset.h"
#include "beeper.h"
#include <avr/interrupt.h>
#include <util/delay.h>

#define TCNT0_START             (256 - 61)

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
    //if (!logo_done)
    //    logo_done = !logo_show();
    //else
    //    clock_show_running();
    display_process();
    beeper_process();
}

ISR(TIMER2_OVF_vect)
{
    //if (logo_done)
    //    clock_process();
}

int main(void)
{
    display_init();
    //logo_init();
    //clock_init();
    beeper_init();
    beeper_set(440, 255);

    TIMSK = 0;

    // TIMER0 is used to update the display
    TCCR0 = _BV(CS11) | _BV(CS10); // 64x prescaler
    TCNT0 = TCNT0_START;
    TIMSK |= _BV(TOIE0);

    // TIMER2 is used as real-time clock
    TCCR2 = _BV(CS22) | _BV(CS20); // 128x prescaler
    OCR2 = 0;
    ASSR = _BV(AS2);
    while ((ASSR & _BV(TCN2UB)) ||
           (ASSR & _BV(TCR2UB)) ||
           (ASSR & _BV(OCR2UB)))
        asm volatile("nop" ::);
    TIMSK |= _BV(TOIE2);

    sei();

    while (1)
    {
        asm volatile("nop" ::);
    }
}
