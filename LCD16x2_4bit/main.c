

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>

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
	lcd_putstr("Helo World...");
	
	//Number Test
	uint32_t numb = 1000000000;
	char numbstr [16];
	
	while(1){
		numb = numb++; // Number increcement
		ultoa(numb, numbstr, 10); // Unsigned long (32bit) to string by format number 10 (Decimal)
		lcd_xy(1,2); // set LCD Cursor Column 1 Row 2
		lcd_putstr(numbstr); // Print number which convert to string
		_delay_ms(500); // delay 500 ms
		
	}

}
