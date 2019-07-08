
#include <stdlib.h>
#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>

#include "ds1307/ds1307.h"
#include "lcd16024b\lcd.h"

//FOR LCD
#define RS 2
#define E 3
#define ctrl PORTB

int main(void) {

	// Input/Output Ports initialization
	DDRC = 0x00;
	DDRB = 0xFF;
	
	//LCD Initializations
	lcd_iniit();
	
	//RTC Initializations
	ds1307_init();
	lcd_putstr("Startup...");
	sei();
	uint8_t year = 0;
	uint8_t month = 0;
	uint8_t day = 0;
	uint8_t hour = 0;
	uint8_t minute = 0;
	uint8_t second = 0;
	
	lcd_clear();
	
	while(1){
		//RTC get date
		ds1307_getdate(&year, &month, &day, &hour, &minute, &second);
		
		//RTC Covert string
		char tanggal[16];
		char jam[16];
		sprintf(tanggal, "DATE :%d/%d/%d  ", day, month, year);
		sprintf(jam, "CLOCK:%d:%d:%d  ", hour, minute, second);
		
		//LCD Print
		lcd_xy(1,1);
		lcd_putstr(tanggal);
		lcd_xy(1,2);
		lcd_putstr(jam);
		
		//delay
		_delay_ms(1000);
		
	}

}
