#include "beeper.h"
#include "defs.h"

// Duration counter
uint16_t beeper_left = 0;

static void beeper_switch(uint8_t on)
{
    if (on)
        TCCR1A |= _BV(COM1A1);
    else
    {
        TCCR1A &= ~_BV(COM1A1);
        PORTB |= _BV(PB1); // Let no current flow
    }
}

void beeper_init(void)
{
    // TIMER1 is for PWM
    DDRB |= _BV(PB1);
    TCCR1A = _BV(WGM11) | _BV(COM1A1);
    TCCR1B = _BV(WGM13) | _BV(WGM12) | _BV(CS10); // no prescaler
    TCNT1 = 0;
    ICR1 = 0x800;
    OCR1A = ICR1;
    TIMSK &= ~_BV(TOIE1);

    beeper_switch(0);
}

void beeper_set(uint16_t frequency,
                uint16_t duration)
{
    // PWM period
    ICR1 = F_CPU / 2 / frequency;
    OCR1A = ICR1 >> 1; // 1/2 duty cycle for maximal volume

    // Let the symphony start!
    beeper_switch(1);
    beeper_left = duration;
}

void beeper_process(void)
{
    if (!beeper_left)
        return;
    if (--beeper_left)
        return;
    // Turn off PWM
    beeper_switch(0);
}
