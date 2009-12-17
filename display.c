#include "display.h"
#include <avr/io.h>

// In the circuitry, the low logic level means the diode is lit.
// Change these macros if otherwise.
#define ANODE_LEVEL(x)          (~(x))
#define CATHODE_LEVEL(x)        (~(x))

// Initially no digits are displayed.
volatile uint8_t display[MAX_ANODES] = { 0, 0, 0, 0, 0 };
volatile uint8_t display_dots = 0;

void display_init(void)
{
    // Port D for output
    DDRD |= 0xff;
    // Port C for output
    DDRC |= 0xff;
    // Initially nothing is displayed
    PORTC = ANODE_LEVEL(0);
    PORTD = CATHODE_LEVEL(0);
    //DDRC |= _BV(PC0);
}

void display_process(void)
{
    // Currently exposed anode
    static uint8_t cur_anode = 0;
    // Select next anode
    if (++cur_anode >= MAX_ANODES)
        cur_anode = 0;
    // Switch previous off
    PORTC = ANODE_LEVEL(0);
    // Prepare next character
    PORTD = CATHODE_LEVEL(display[cur_anode] | (display_dots ? (1<<7) : 0));
    // Switch next on
    PORTC = ANODE_LEVEL(1 << cur_anode);
}
