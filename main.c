//#define F_CPU 4000000ul  // 4 MHz
#include "display.h"
#include "logo.h"
#include <avr/io.h>
#include <avr/interrupt.h>
//#include <util/delay.h>

#define TCNT0_START             (256 - 61)

volatile uint8_t jiffies = 0;

volatile uint8_t logo_done = 0;

ISR(TIMER0_OVF_vect)
{
    TCNT0 = TCNT0_START;
    ++jiffies;
    if (!logo_done)
        logo_done = !logo_show();
    if (logo_done && jiffies == 0x80)
        display[0] = 0;
    display_process();
}

ISR(TIMER2_OVF_vect)
{
    if (logo_done)
        display[0] = ~0;
}

int main(void)
{
    display_init();
    logo_init();

    TIMSK = 0;

    // TIMER0
    TCCR0 = _BV(CS11) | _BV(CS10); // 64x prescaler
    TCNT0 = TCNT0_START;
    TIMSK |= _BV(TOIE0);

    // TIMER2
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
        asm volatile("nop" ::);
}
