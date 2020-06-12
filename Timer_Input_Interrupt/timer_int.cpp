#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

ISR(INT0_vect)
{
	PORTC ^= _BV(PC1); 
}

ISR (TIMER1_COMPA_vect)
{
    PORTC ^= _BV(PC0); 
}

int main()
 {
	// I/O setup 
	DDRB = 0xFF ;
	DDRC = 0x03 ;
	PORTD |= _BV(PD2) ; //Pullup input PD2 - INT0
	 
	//Input Interrupt
	PORTD |= _BV(PD2);
	GICR |= _BV(INT0);		// Enable INT0
	MCUCR |= _BV(ISC01);	// Trigger INT0 on falling edge
	 
	//Timer Interrupt
	OCR1A = 3500 ;

    TCCR1B |= _BV(WGM12); // Mode 4, CTC on OCR1A
    TIMSK |= _BV(OCIE1A); //Set interrupt on compare match
    TCCR1B |= _BV(CS12) | _BV(CS10); // set prescaler to 1024 and start the timer
	
	//Enable Global Interrupt
	sei(); 
	
   while (1)
   {
	 //Simple Running LED Program with delay  
	   PORTB = 0b00000001 ;
	   _delay_ms(500);
	   PORTB = 0b00000010 ;
	   _delay_ms(500);
	   PORTB = 0b00000100 ;
	   _delay_ms(500);
	   PORTB = 0b00001000 ;
	   _delay_ms(500);
	   PORTB = 0b00010000 ;
	   _delay_ms(500);
	   PORTB = 0b00100000 ;
	   _delay_ms(500);
	   PORTB = 0b01000000 ;
	   _delay_ms(500);
	   PORTB = 0b10000000 ;
	   _delay_ms(500);
   }
	   
   return 0;
 }