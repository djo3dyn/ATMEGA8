#include <avr/io.h>
#include <avr/interrupt.h>
#include <math.h>


// Register Variable
volatile unsigned int myOCRA ;
volatile unsigned int myOCRB ;
volatile unsigned int myICR = 2000;

const float angle_inc = 20 ;

// Deggree Variable
volatile float angle_degree ;
volatile float angle_rad ;
volatile float sin_value ;

ISR (INT1_vect)
{	
	
	if (angle_degree > 10) // Tolerance max 10 degree
	{
		angle_degree = 0 ;
	}
	
}


ISR(TIMER1_OVF_vect)
{
	angle_degree += angle_inc ; // Increment + 2 
	
	if (angle_degree <= 180)
	{
		//TCCR1A = 0b10000010 ; // Disconnect OC1B
		angle_rad = angle_degree * 1000 / 57296;
		sin_value = sin(angle_rad); // Calculate sin wave
		myOCRA = (int) (sin_value * myICR + 0.5) ; // convert to value
		OCR1A = myOCRA ; // add to register
	}
	else if (angle_degree >= 180)
	{
		//TCCR1A = 0b00100010 ; // Disconnect OC1A
		angle_rad = (angle_degree - 180) * 1000 / 57296;
		sin_value = sin(angle_rad); // Calculate sin wave
		myOCRB = (int) (sin_value * myICR + 0.5) ; // convert to value
		OCR1B = myOCRB ; // add to register
	}

	if (angle_degree == 120) //reset to 0 ;
	{
		PORTC &= ~_BV(PC3);
	}
	else if (angle_degree >= 360) //reset to 0 ;
	{
		angle_degree = 0 ;
		PORTC |= _BV(PC3);
	}

	/* Test Purpose only
	myOCRA ++;
	OCR1A = myOCRA;
	OCR1B = myOCRA;
	if (myOCRA >= myICR) 
	{
		myOCRA = 0 ;
	}
	*/

}

int main(void)
{

	//Set PB1 and PB2 to Output
	DDRB |= _BV(DDB1)| _BV(DDB2);
	DDRC |= _BV(PC3);

	//Connect PB1 and PB2 to OC1A and OC1B as PWM Output
	TCCR1A |= _BV(COM1A1) | _BV(COM1B1);

	//Set Fast PWM by ICR1 on top
	TCCR1A |= _BV(WGM11);
	TCCR1B |= _BV(WGM12) | _BV(WGM13);

	// Set prescaler to 1 and starts PWM
	TCCR1B |= _BV(CS10);

	//Set top of PWM by set ICR1
	ICR1 = myICR ; 

	// OCR1A = 0;
	// OCR1B = 0;

	// enable Timer Overflow interrupt
	TIMSK |= _BV(TOIE1); 

	// Trigger Interupt
	GICR |= _BV(INT1); // Pin INT1 interupt enable
	MCUCR |= _BV(ISC11) ; // Interupt enable on falling edge ;
	// enable global interrupts
	sei(); 
   
   while (1)
   {
   }
}
