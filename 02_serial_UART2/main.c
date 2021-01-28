/**
 * Copyright (c) 2016, Lukasz Marcin Podkalicki <lpodkalicki@gmail.com>
 * ATmega8/002
 * Simple debug logging via UART.
 *
 * Simplified wiring diagram:
 * +------------+----------------+
 * |   ATMEGA8  | Serial Adapter |
 * +------------+----------------+
 * | PD1 (TXD)  |      RXD       |
 * +------------+----------------+
 * |     GND    |      GND       |
 * +------------+----------------+
 *
 */

#define CODE_MODE 0
#define VAL_MODE 1

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

static void UART_init(uint16_t baudrate);
static void UART_putc(char c);
static void UART_puts(const char *s);

volatile int rx_mode = CODE_MODE ;
volatile int add_code = 0 ;
volatile int add_value = 0 ;
volatile int array_value [16];
volatile char rx_data ;

unsigned char UART_Receive( void )
{
/* Wait for data to be received */
while ( !(UCSRA & (1<<RXC)) )
;
/* Get and return received data from buffer */
return UDR;
}

void UART_init(uint16_t baudrate)
{
 	uint16_t baudrate_calc = (F_CPU / 4 / baudrate - 1) / 2;

	/* Set PD1 (TXD) as output */
	DDRD |= _BV(PD1);

	/* Set calculated baud rate */
	UBRRH = baudrate_calc >> 8;
 	UBRRL = baudrate_calc;

	/* Double the USART transmition speed */
	UCSRA = _BV(U2X);

	/* Enable transmition & reception + interupt */
	UCSRB = _BV(TXEN) | _BV(RXEN) | _BV(RXCIE);

	/* Set format 8N1 */
	UCSRC = _BV(URSEL) | _BV(UCSZ1) | _BV(UCSZ0);

	// enable global interupt
	//sei();
}

void UART_putc(char c)
{
	/* Wait for empty transmit buffer */
	while (!(UCSRA & _BV(UDRE)));
	/* Put data into buffer, sends the data */
	UDR = c;
}

void UART_puts(const char *s)
{

    while (*s) UART_putc(*(s++));
}

ISR(USART_RXC_vect)
{
	rx_data = UDR ;
}

int
main(void)
{
	/* setup */
	UART_init(9600);
	DDRB = 0b11111111;

	/* loop */
	while (1) 
	{
		UART_putc( 255 ) ;
		_delay_ms(1000);
	}
		
}

