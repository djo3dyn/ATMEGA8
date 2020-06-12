#include <avr/io.h>
#include <util/delay.h>

#define SCAN_PORT PORTC
#define DATA_PORT PORTD

#define CLOCK 	4
#define DATA 	5
#define LATCH 	6

uint8_t main_data_1[8] = {0,0,0,0, 0,0,0,0}; 
uint8_t main_data_2[8] = {0,0,0,0, 0,0,0,0}; 
uint8_t main_data_3[8] = {0,0,0,0, 0,0,0,0}; 
uint8_t main_data_4[8] = {0,0,0,0, 0,0,0,0}; 

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
	DDRD = 0xF0; // Output for Data
	DDRC = 0x0F; // Output for Scan

}
void shiftOut(uint8_t _data) 
{
	for (int i = 7 ; i >= 0 ; i-- )  
	{
		DATA_PORT &= ~_BV(CLOCK); // Clock LOW
		if ( _data & (1 << i ) ) DATA_PORT |= _BV(DATA);  //Data out 0000 0000 , Q7-Q6-Q5-Q4 Q3-Q2-Q1-Q0 
		else DATA_PORT &= ~_BV(DATA);
		DATA_PORT |= _BV(CLOCK); // Clock HIGH
	 }
	 
}

void scan(int _rate)  //scan rate in us
{
	uint8_t row_num = 0 ;
	for (int i=0 ; i<=7 ; i++)
	{
		DATA_PORT &= ~_BV(LATCH); // Latch off
		shiftOut(main_data_1[i]);
		shiftOut(main_data_2[i]);
		shiftOut(main_data_3[i]);
		DATA_PORT |= _BV(LATCH) ;// Latch on
		
		row_num = i ;
		SCAN_PORT = row_num & 0x0F ; 
		
		_delay_us(_rate);
		
		DATA_PORT &= ~_BV(LATCH); // Latch off
		shiftOut(0);
		shiftOut(0);
		shiftOut(0);
		DATA_PORT |= _BV(LATCH) ;// Latch on
		
	}
}

void set_data(uint8_t _data_1[] , uint8_t _data_2[] , uint8_t _data_3[])
{
	for (int i=0 ; i<=7 ; i++)
	{
		main_data_1[i] = _data_1[i];
		main_data_2[i] = _data_2[i];
		main_data_3[i] = _data_3[i];
	}
}

int main()
{
	init();
	set_data(test_char_1 , test_char_2  , test_char_3) ;
	
	while(1)
	{
		scan(50);
		us_count++;
		if (us_count >= 100 && us_count < 200) 
		{
			set_data(test_char_2 , test_char_1 , test_char_3) ;
		}
		else if (us_count >= 200 && us_count < 300)
		{
			set_data(test_char_1 , test_char_2 , test_char_3) ;
			us_count = 0 ;
		}
	}
}