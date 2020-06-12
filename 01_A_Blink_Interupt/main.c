/**
 * Copyright (c) 2016, Lukasz Marcin Podkalicki <lpodkalicki@gmail.com>
 * ATtiny13/001
 * Simple blinky with delay function.
 */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define	LED_PIN	PB0

void C_Setup();

int
main(void)
{

	C_Setup();
	/* setup */
	DDRB = 0b00000001; // set LED pin as OUTPUT
	PORTB = 0b00000000; // set all pins to LOW

	/* loop */
	while (1) {
		PORTB = (0<<PORTB0); // toggle LED pin
	
	}
}

void C_Setup() {
		GICR = 1 << INT0; // Enable the interrupt pin ( at PD2)
		MCUCR = (1 << ISC00); // Set interrupt configuration in PB1 (see page 46 for any change mode)
		sei();
}

ISR(INT0_vect) {

		PORTB = (1<<PORTB0);
		_delay_ms(2000);
}