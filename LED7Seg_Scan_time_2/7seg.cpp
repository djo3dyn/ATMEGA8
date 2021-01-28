#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "ds1307.h"

<<<<<<< HEAD
#include "segment.h"

#define SCAN PORTD
#define DATA PORTB

#define RUN_MODE 0
#define SET_MODE 1

uint8_t op_mode = 0 ;
uint8_t flash , flash_en ;
uint8_t digit_flash = 0 ;
uint64_t two_millis , button_millis ; 

uint8_t segment[17] = {	ZERO,ONE,TWO,THREE,FOUR,
						FIVE,SIX,SEVEN,EIGHT,
						NINE,CHR_A,CHR_B,CHR_C,
						CHR_D,CHR_E,CHR_F ,NONE};

volatile uint8_t digit[8] = {0,0,0,0, 0,0,0,0};					
volatile uint8_t data_digit[8] = {0,0,0,0, 0,0,0,0};  //main scan data
=======
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
>>>>>>> 069d63ec83c3dc1dae89cf2f842bda93b21da030
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
	
<<<<<<< HEAD
	DDRC = 0b00001001 ;// Add second colon/dot
	PORTC |= _BV(PC1) | _BV(PC2) ; //Input Pullup for button

	//Timer Interrupt for scan - High speed
=======
	DDRC = 0x01 ;// Add second colon/dot
	PORTC |= _BV(PC1) | _BV(PC2) ; //Input Pullup for button

	//Timer Interrupt
>>>>>>> 069d63ec83c3dc1dae89cf2f842bda93b21da030
	TCCR1B |= _BV(WGM12); // Mode 4, CTC on OCR1A
    TIMSK |= _BV(OCIE1A); //Set interrupt on compare match 

	DATA = 0xFF ; //Data test
	SCAN = 0x00; // Scan test
	_delay_ms(500);
	SCAN |= ~_BV(PD0); // initial value scan
	DATA = 0x00 ;
<<<<<<< HEAD

	// Timer interupt for flash - Low speed
	TIMSK |= _BV(TOIE0) ; //Enable Timer0 overflow interrupt
	TCCR0 |= _BV(CS01)|_BV(CS00); // set prescaler to 64 (CLK=8000000Hz/64/256=488Hz, 2ms)
=======
>>>>>>> 069d63ec83c3dc1dae89cf2f842bda93b21da030
	
}

void start_scan() //Start scan rate in milisecond
{
	//Timer Interrupt
	OCR1A = 2000 ; // OCRn =  [ (clock_speed / Prescaler_value) * Desired_time_in_Seconds ] - 1
    //TCCR1B |= _BV(CS11) | _BV(CS10); // set prescaler to 64 and start the timer
	TCCR1B |= _BV(CS11) ; // set prescaler to 8 and start the timer
	//Enable Global Interrupt
	sei(); 
}

void stop_scan()
{
	//Timer Interrupt
	TCCR1B &= ~_BV(CS12) & ~_BV(CS11) & ~_BV(CS10) ;  // set prescaler to 0 and stop the timer
	//Disable Global Interrupt
	cli(); 
}

<<<<<<< HEAD
/*
void start_flash(int _stat)
{
	if (_stat) TCCR0 |= _BV(CS02)|_BV(CS00); // set prescaler to 1024 (CLK=8000000Hz/1024/256=30Hz, 0.03s)
	else TCCR0 &= ~_BV(CS02) & ~_BV(CS01) & ~_BV(CS00) ; // set prescaler to 0 and stop the timer
	
}
*/

=======
>>>>>>> 069d63ec83c3dc1dae89cf2f842bda93b21da030
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

<<<<<<< HEAD
void flash_digit(unsigned int _digit , unsigned int on_off)
{
	if (on_off) data_digit[_digit] = segment[digit[_digit]];
	else data_digit[_digit] = segment[16];
}

void update_time()
{
	unsigned int sec_min = 0 ;
=======
void update_time()
{
	unsigned int sec_min = 0 ;

>>>>>>> 069d63ec83c3dc1dae89cf2f842bda93b21da030
	//RTC get date
	ds1307_getdate(&year, &month, &day, &hour, &minute, &second);
	sec_min = (minute *100) + second ;
	generate_digit(sec_min , hour);
}

ISR (TIMER1_COMPA_vect)
{
    scan_segment(); 
<<<<<<< HEAD
}

ISR(TIMER0_OVF_vect) // always ON
{
	two_millis++; // Count overflow
	if(two_millis >= 250 ) //every ~0,5 sec
	{
		PORTC ^= _BV(PC3); //Test toogle LED
		if (flash_en)
		{
			flash ^= 1 ;
			flash_digit(digit_flash , flash);
		}
		if (!(PINC&_BV(PC1)))
		{
			button_millis++;
			if (button_millis>=6)
			{
				op_mode ^= 1 ;
				flash_en ^= 1 ;
				button_millis = 0 ;
			}
		}
		else
		{
			button_millis = 0 ; 
		}
		two_millis = 0; //reset milis
	}
=======
	//PORTC ^= _BV(PC0);
>>>>>>> 069d63ec83c3dc1dae89cf2f842bda93b21da030
}
	
int main()
{
	init();
	update_time();
	start_scan();

	while(1)
	{
<<<<<<< HEAD
		if (op_mode==RUN_MODE)
		{
			update_time();
			PORTC ^= _BV(PC0);
			_delay_ms(500);
		}
		else if (op_mode==SET_MODE)
		{
			
	
		}
	
=======
		update_time();
		PORTC ^= _BV(PC0);
		_delay_ms(500);

>>>>>>> 069d63ec83c3dc1dae89cf2f842bda93b21da030
	}
}
