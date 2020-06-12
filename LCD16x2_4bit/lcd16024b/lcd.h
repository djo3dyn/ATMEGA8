/* 	LCD 16x2 4 Bit .h Header
 * 	Created: 11/28/2015 11:12:42 PM
 *  Author: Ismail
 *
 *	EngineerExperiences.com
*/

//Define F_CPU and Pin and PORTB

#ifndef F_CPU
#define F_CPU 4000000UL
#endif

#ifndef RS
#define RS 2
#endif

#ifndef E
#define E 3
#endif

#ifndef ctrl
#define ctrl PORTB
#endif

//Define Function

extern int lcd_writedata(char t);

extern int lcd_writecmd(char z);

extern void lcd_putstr(char const *str);

extern void lcd_xy(unsigned char x, unsigned char y);

extern void lcd_iniit(void);

extern void lcd_clear(void);