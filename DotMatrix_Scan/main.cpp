#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

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
#define STEP3 300
#define STEP4 400
#define STEP5 500



uint8_t test_char_1[8] = 
{	
	0b00011000 ,
	0b00011000 ,
	0b00011000 ,
	0b00011000 ,
	0b00011000 ,
	0b01111110 ,
	0b00111100 ,
	0b00011000 
};

uint8_t test_char_2[8] = 
{	
	0b10000001 ,
	0b01000010 ,
	0b00100100 ,
	0b00011000 ,
	0b00011000 ,
	0b00100100 ,
	0b01000010 ,
	0b10000001 
};

uint8_t test_char_3[8] = 
{	
	0b11111111 ,
	0b10000001 ,
	0b10000001 ,
	0b10000001 ,
	0b10000001 ,
	0b10000001 ,
	0b10000001 ,
	0b11111111 
};

unsigned int us_count = 0 ;
	
void init()
{
	DDRB = 0xFF; // Output Positif -- Data
	DDRD = 0xFF; // Output Negatif -- Scan
	
	DATA = 0xFF ; //Data test
	SCAN = 0x00; // Scan test
	_delay_ms(500);
	SCAN |= ~_BV(PD0); // initial value scan
	DATA = 0x00 ;
}

void scan(int _rate , uint8_t _data[])  //scan rate in us
{
	uint8_t buff = 1 ;
	for (int i=0 ; i<=7 ; i++)
	{
		DATA = _data[i];
		SCAN = ~buff ;
		buff <<= 1 ;
		if (buff == 0) buff = 1 ;
		_delay_us(_rate);
		DATA = 0x00; //Clear Data
	}
	
}

int main()
{
	init();
	
	while(1)
	{
		us_count++;
		if (us_count <= STEP1 )
		{
			scan(100,test_char_1);
		}
		else if (us_count > STEP1 && us_count <= STEP2 )
		{
			scan(100,test_char_2);
		}
		else if (us_count > STEP2 && us_count <= STEP3 )
		{
			scan(100,test_char_3);
	
		}
		else if (us_count > STEP3 ) //Reset
		{
			us_count = 0;
		}
		
	}
}
