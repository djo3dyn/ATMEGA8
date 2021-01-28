/*============================================
 * 
 * SPWM Generator for AC inverter
 * Expandable for 3 phase application
 * Use ATMega8 Chip for each phase
 * 
==============================================*/

#include <avr/io.h>
#include <avr/interrupt.h>
#include <math.h>

#define PIN_SYNC PC3
#define PORT_SYNC PORTC

// Register Variable
unsigned int myICR = 1776;

// Degree reference to create sine value
static int angle_degree = 0 ;

// Full 180 degree lookup table
static unsigned int lookup_180[180] ;

// Create lookup table
void create_lookup()
{
    float angle_rad ;
	for (int i = 0 ; i <= 180 ; i++)
	{
    	angle_rad = (float) i * 1000 / 57296; // Convert degree to radians
    	lookup_180[i] = (int) (sin(angle_rad) * myICR + 0.5 ); // Sine angle and convert to register value
	}
    
}

ISR (INT1_vect)
{	
	// Trigger to syncronize phase angle,
	if (angle_degree > 10) angle_degree = 0 ; // Tolerance max 10 degree
}


ISR(TIMER1_OVF_vect)
{
	angle_degree += 2 ; // Increment + 2 

	// Write lookup value to TC Register	
	if (angle_degree <= 180) OCR1A = lookup_180[angle_degree]; // High Register
	if (angle_degree >= 180 && angle_degree <= 360 ) OCR1B = lookup_180[angle_degree - 180]; //Low Register

	// Output pin to syncronize phase angle , Diference by 120 degree
	if (angle_degree == 120) 
	{
		PORT_SYNC &= ~_BV(PIN_SYNC);
	}
	else if (angle_degree >= 360) //reset angle to 0 ;
	{
		angle_degree = 0 ;
		PORT_SYNC |= _BV(PIN_SYNC);
	}

}

int main(void)
{

	//Set PB1 and PB2 to Output
	DDRB |= _BV(DDB1)| _BV(DDB2) | _BV(DDB3);
	DDRC |= _BV(PC3);

	//Startup create lookup
	create_lookup();

	//Connect PB1 and PB2 to OC1A and OC1B as PWM Output
	TCCR1A |= _BV(COM1A1) | _BV(COM1B1);

	//Set Fast PWM by ICR1 on top
	TCCR1A |= _BV(WGM11);
	TCCR1B |= _BV(WGM12) | _BV(WGM13);

	// Set prescaler to 1 and starts PWM
	TCCR1B |= _BV(CS10);

	//Set top of PWM by set ICR1
	ICR1 = myICR ; 

	// enable Timer Overflow interrupt
	TIMSK |= _BV(TOIE1); 

	// Trigger Interupt
	GICR |= _BV(INT1); // Pin INT1 interupt enable
	MCUCR |= _BV(ISC11) ; // Interupt enable on falling edge ;
	// enable global interrupts
	sei();

	// TC2 Output 8Mhz to drive other MCU
	TCCR2 |= _BV(COM20)|_BV(WGM21)|_BV(CS20); // set timer counter mode to CTC
    OCR2 = 0 ; // set Timer's counter max value 
   
   while (1)
   {
   }
}
