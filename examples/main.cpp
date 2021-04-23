#include <stdbool.h>
#include <util/delay.h>
#include <avr/io.h>
#include <stdpins.h>


// define some pins

#define led     B,1,ACTIVE_HIGH
#define button  D,6,ACTIVE_LOW
#define lux     C,3,ACTIVE_HIGH

int main()
{
    // configure as input and output
    AS_INPUT_PU(button);
    AS_OUTPUT(led);
    AS_INPUT(lux);

    // flash the LED
    ASSERT(led);
    _delay_ms(100);
    NEGATE(led);

    // read analog value
    DIDR0 |= BV(lux);                       // disable digital circuitry
    ADMUX = (3 << REFS0) | portBIT(lux);    // select 1.1V reference, our channel
    //... do the analog conversion    

    // make LED follow button
    bool state;
    for (;;) {
        state = IS_TRUE(button);
        SET_PA(led,state);
    }
}
