#define F_CPU 4000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>

//FOR LCD
#define RS 2
#define E 3
#define ctrl PORTB

int writedata(char t){
	
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


int writecmd(char z)
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

void putstring(char const *str){
	
	unsigned char k=0;
	//Sending character by character to LCD
	while (str[k]!=0){
		writedata(str[k]);
		k++;
	}
}

void LCD_XY(unsigned char x, unsigned char y){
	
	unsigned char firstcharadrs[] = {0x80, 0xC0,0x94,0xD4};
	writecmd(firstcharadrs[y-1] + x - 1);
	_delay_us(100);
	
}

void lcd_iniit(void){
	writecmd(0x02);// Return Home
	writecmd(0x28);//4 bit initialization-2line display-
	writecmd(0x0C);//Display on-Cursor off-Blink off
	writecmd(0x01);//Clear All Display Data
	writecmd(0x06);//after every character cursor will move to right
}

void lcd_clear(void){
	writecmd(0x01);//Clear All Display Data
}


int main(void)
{
	// Input/Output Ports initialization
	DDRC = 0x00;
	DDRB = 0xFF;
	//Other Initializations
	lcd_iniit();
	int numb = 0;
	char numbs[16];
// 	putstring("4-Bit Lcd");
// 	LCD_XY(1,2);//to move the cursor to second line
// 	putstring("Interfacing");
	putstring("Heloo...");
	_delay_ms(3000);
	lcd_clear();
	putstring("Run for Second:");
	while(1){
		LCD_XY(1,2);
		numb = numb++;
		itoa(numb, numbs, 10);
		putstring(numbs);
		_delay_ms(1000);
	}
}