#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "ds1307.h"

#define SCAN PORTD
#define DATA PORTB
 

#define NONE    0b00000000
#define ZERO    0b00111111
#define ONE     0b00000110
#define TWO     0b01011011 
#define THREE   0b01001111
#define FOUR    0b01100110
#define FIVE    0b01101101
#define SIX     0b01111101
#define SEVEN   0b00000111
#define EIGHT   0b01111111
#define NINE    0b01101111
#define CHR_A   0b01110111
#define CHR_B   0b01111100
#define CHR_C   0b00111001
#define CHR_D   0b01011110
#define CHR_E   0b01111001
#define CHR_F   0b01110001


//Define step timer

#define STEP1 100
#define STEP2 200
#define STEP3 600
#define STEP4 800
#define STEP5 1000

uint8_t segment[16] = {	ZERO,ONE,TWO,THREE,FOUR,
						FIVE,SIX,SEVEN,EIGHT,
						NINE,CHR_A,CHR_B,CHR_C,
						CHR_D,CHR_E,CHR_F };

volatile uint8_t digit[8] = {0,0,0,0, 0,0,0,0};					
volatile uint8_t data_digit[8] = {0,0,0,0, 0,0,0,0};
volatile uint8_t digit_count = 6 ;

uint8_t year = 0;
uint16_t year_w = 2000;
uint8_t month = 0;
uint8_t day = 0;
uint8_t hour = 0;
uint8_t minute = 0;
uint8_t second = 0;
	

unsigned int us_count = 0 ;
	
void init()
{
	DDRB = 0xFF; // Output Positif -- Data
	DDRD = 0xFF; // Output Negatif -- Scan
	
	DDRC = 0x01 ;// Add second colon/dot

	//Timer Interrupt
	TCCR1B |= _BV(WGM12); // Mode 4, CTC on OCR1A
    TIMSK |= _BV(OCIE1A); //Set interrupt on compare match 

	DATA = 0xFF ; //Data test
	SCAN = 0x00; // Scan test
	_delay_ms(500);
	SCAN |= ~_BV(PD0); // initial value scan
	DATA = 0x00 ;
	
}

void start_scan(int _rate) //Start scan rate in milisecond
{
	//Timer Interrupt
	OCR1A = 20; // OCRn =  [ (clock_speed / Prescaler_value) * Desired_time_in_Seconds ] - 1
    TCCR1B |= _BV(CS11); // set prescaler to 8 and start the timer
	//Enable Global Interrupt
	sei(); 
}

void stop_scan(int _rate)
{
	//Timer Interrupt
    TCCR1B &= ~_BV(CS12) & ~_BV(CS11) & ~_BV(CS10) ; // set prescaler to 0 and stop the timer
	//Disable Global Interrupt
	cli(); 
}

void scan_segment()  //scan rate in us
{
	volatile uint8_t buff = 1 ;
	for (int i=0 ; i < digit_count ; i++)
	{
		DATA = data_digit[i];
		SCAN = ~buff ;
		buff <<= 1 ;
		if (buff == 0) buff = 1 ;
		DATA = 0x00; // Clear Data <---- Optional with real hardware
	}
	
	
}

unsigned int dec2bcd(unsigned int number)
{
  
    unsigned char i=0;
    unsigned int k = 0;
    while(number)
    {
        k = ( k ) | ((number%10) << i*4);
        number = number / 10;
        i++;
    }
    return(k);
 
}

void generate_digit(unsigned int _numL , unsigned int _numH)
{
	digit[0] = dec2bcd(_numL) & 0x0F ;
	digit[1] = (dec2bcd(_numL) >> 4) & 0x0F ;
	digit[2] = (dec2bcd(_numL) >> 8) & 0x0F ;
	digit[3] = (dec2bcd(_numL) >> 12) & 0x0F ;

	digit[4] = dec2bcd(_numH) & 0x0F ;
	digit[5] = (dec2bcd(_numH) >> 4) & 0x0F ;
	digit[6] = (dec2bcd(_numH) >> 8) & 0x0F ;
	digit[7] = (dec2bcd(_numH) >> 12) & 0x0F ;
	
	data_digit[0] = segment[digit[0]];
	data_digit[1] = segment[digit[1]];
	data_digit[2] = segment[digit[2]];
	data_digit[3] = segment[digit[3]];
	data_digit[4] = segment[digit[4]];
	data_digit[5] = segment[digit[5]];
	data_digit[6] = segment[digit[6]];
	data_digit[7] = segment[digit[7]];
	
}

void update_time()
{
	unsigned int sec_min = 0 ;

	//RTC get date
	ds1307_getdate(&year, &month, &day, &hour, &minute, &second);
	sec_min = (minute *100) + second ;
	generate_digit(sec_min , hour);
}

ISR (TIMER1_COMPA_vect)
{
    scan_segment(); 
}
	
int main()
{
	init();
	update_time();
	start_scan(100);

	while(1)
	{
		update_time();
		PORTC ^= _BV(PC0);
		_delay_ms(50);
		
		//us_count++;
		
	}
}
