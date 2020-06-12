/* 	LCD 16x2 4 Bit .c Header
 *
 * 	Created: 11/28/2015 11:12:42 PM
 *  Author: Ismail
 *
 *	EngineerExperiences.com

*/

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>

#include "lcd.h"

int lcd_writedata(char t){
	
	ctrl |= (1 << RS);// To write Data
	char t1 = t << 4;// To shift 4 bits
	t &= 0b11110000;// for masking the lower 4 bits
	ctrl = (ctrl & 0x0F)|t;//sending data

	ctrl |= (1<<E); //send falling edge to send data to LCD
	_delay_ms(1);
	ctrl &= ~(1<<E);
	_delay_ms(1);
	
	t1 &= 0b11110000;//Again masking lower 4 bits
	ctrl = (ctrl & 0x0F)|t1;// Sending remaining data
	
	ctrl |= (1<<E);//send falling edge to send data to LCD
	_delay_ms(1);
	ctrl &= ~(1<<E);
	_delay_ms(1);
	return 0;
}


int lcd_writecmd(char z)
{	ctrl &= ~(1<<RS);// To write Commands
	char z1 = z << 4;// To shift 4 bits
	z &= 0b11110000; //masking lower 4 bits
	ctrl = (ctrl & 0x0F)|z; // sending data to LCD
	
	ctrl |= (1<<E);//send falling edge to send data to LCD
	_delay_ms(1);
	ctrl &= ~(1<<3);
	_delay_ms(1);
	z1 &= 0b11110000;//Again masking lower 4 bits
	ctrl = (ctrl & 0x0F)|z1;//Sending Remaining Data to LCD
	
	ctrl |= (1<<E);//send falling edge to send data to LCD
	_delay_ms(1);
	ctrl &= ~(1<<E);
	_delay_ms(1);
	return 0;
}

void lcd_putstr(char const *str){
	
	unsigned char k=0;
	//Sending character by character to LCD
	while (str[k]!=0){
		lcd_writedata(str[k]);
		k++;
	}
}

void lcd_xy(unsigned char x, unsigned char y){
	
	unsigned char firstcharadrs[] = {0x80, 0xC0,0x94,0xD4};
	lcd_writecmd(firstcharadrs[y-1] + x - 1);
	_delay_us(100);
	
}

void lcd_iniit(void){
	lcd_writecmd(0x02);// Return Home
	lcd_writecmd(0x28);//4 bit initialization-2line display-
	lcd_writecmd(0x0C);//Display on-Cursor off-Blink off
	lcd_writecmd(0x01);//Clear All Display Data
	lcd_writecmd(0x06);//after every character cursor will move to right
}

void lcd_clear(void){
	lcd_writecmd(0x01);//Clear All Display Data
}