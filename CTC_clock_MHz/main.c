
#include <avr/io.h>


int
main(void)
{

	/* setup */
	DDRB = 0b00001000; // set LED pin as OUTPUT
	PORTB = 0b00000000; // set all pins to LOW
    TCCR2 |= _BV(COM20)|_BV(WGM21)|_BV(CS20); // set timer counter mode to CTC
    OCR2 = 0; // set Timer's counter max value

	/* loop */
	while (1);
}
