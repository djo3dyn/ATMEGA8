/* Serial UART to TWI master transmitter ;
 * Input UART Frame [Start_Code] [Data_Size] [Slave_addrees] [data_1] ...[data_n] ; max 16 byte
 * 
*/

#include <avr/io.h>
#include <util/delay.h>

#include <avr/interrupt.h>
#include <util/twi.h>

#ifndef F_CPU
#define F_CPU 8000000UL  // 8 MHz
#endif

// TWI Frequency
#define	TW_FREQ_100K 0
#define	TW_FREQ_250K 1
#define	TW_FREQ_400K 2

// UART Status
#define UART_READY    0
#define UART_COMPLETE 1
#define UART_PROCCESS  2
#define UART_TIMEOUT  3
#define UART_INVALID  4
#define UART_OVERSIZE  5

// UART Status CODE
#define COMPLETE_CODE 250
#define INVALID_CODE 251
#define TIMEOUT_CODE 252
#define OVERSIZE_CODE 253

//UART Timeout time 65us x Val
#define UART_TIMEOUT_TIME  100

// Received , Decoded Data
const char start_code = 255 ;
char slave_address ;
char data_size ; 
char data_value[8] ;

// TWI Proccess status
uint8_t twi_status ;

// Buffer data on Interupt
volatile int UART_status ;
volatile int data_position ;
volatile char data_frame[16] ;
volatile uint16_t timeout_counter;

/* ============================= Two Wire Interface =========================*/
// TWI init
void MT_init(uint8_t twi_freq_mode)
{
  switch (twi_freq_mode)
	{
		case TW_FREQ_100K:
		/* Set bit rate register 72 and prescaler to 1 resulting in
		SCL_freq = 16MHz/(16 + 2*72*1) = 100KHz	*/
		TWBR = 72;
		break;
		
		case TW_FREQ_250K:
		/* Set bit rate register 24 and prescaler to 1 resulting in
		SCL_freq = 16MHz/(16 + 2*24*1) = 250KHz	*/
		TWBR = 24;
		break;
		
		case TW_FREQ_400K:
		/* Set bit rate register 12 and prescaler to 1 resulting in
		SCL_freq = 16MHz/(16 + 2*12*1) = 400KHz	*/
		TWBR = 12;
		break;
		
		default: break;
	}

  sei(); // enable global interrupt
}
// Start Transmit
uint8_t MT_start()
{

  // TWI-ENable , TWI Interrupt Enable & Start TWI
  TWCR = (1<<TWINT)|(1<<TWSTA)| (1<<TWEN) ;
  while (!(TWCR & (1<<TWINT))); //Wait TWINT enable
  if (TW_STATUS != TW_START && TW_STATUS != TW_REP_START) twi_status |= 0b00000001 ;

  return twi_status ;

}
// Transmitter Write Slave Address
uint8_t MT_write_sla(uint8_t _addr )
{
  // Write SLA
  TWDR = (_addr << 1) | 0x00; // Load Slave Address, write mode ;
  TWCR = (1<<TWINT) | (1<<TWEN);
  while (!(TWCR & (1 << TWINT))); //Wait TWINT enable
  if (TW_STATUS != TW_MT_SLA_ACK ) twi_status |= 0b00000010 ;

  return twi_status ;
}
// Transmiter Write function
uint8_t MT_write_data(uint8_t _value)
{
  // Write Data 1
  TWDR = _value ; // Load data
  TWCR = (1<<TWINT) | (1<<TWEN);
  while (!(TWCR & (1 << TWINT))); //Wait TWINT enable
  if (TW_STATUS != TW_MT_DATA_ACK ) twi_status |= 0b00000100 ;

  return twi_status ;
}
// Stop Transmit
void MT_stop()
{
  // Stop TWI
  TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
}

/* ============================== UART ===================================*/
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
  // Timeout Setup
  TCCR0 |= _BV(CS00); // set prescaler to 1 (CLK=8000000Hz/1/256=15kHz, 0.25ms)
	TIMSK |= _BV(TOIE0); // enable Timer Overflow interrupt
	// enable global interupt
	sei();
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

void UART_decode()
{
  // Decode  Data Size and SLA
  data_size = data_frame[1];
  slave_address = data_frame[2];
  // Decode data value
  for (int i = 0 ; i < data_size - 1 ; i++) {
    data_value[i] = data_frame[i+3];
  }

}

void UART_clear()
{
  // Clear Data Frame
  for (int i = 0 ; i < 16 ; i++) {
    data_frame[i] = 0 ;
  }
  data_position = 0;
}

void UART2TWI_Process()
{
  uint8_t status ;
  // TWI Proccess
  status = MT_start(); // Start TWI
  status = MT_write_sla(slave_address); // TWI Send Slave Address
  // TWI Send data
  for (int i = 0 ; i < (data_size-1) ; i++) {
    status = MT_write_data(data_value[i]);
  }
  MT_stop(); // Stop TWI
}

// Incoming UART Handle
ISR (USART_RXC_vect)
{
  char buffer = UDR ; // Read Char from buffer
  if (UART_status == UART_READY) // Check if loop program ready to receive data
  {
    data_frame[data_position] = buffer ; // Save Char to data frame array
    // Stop Receiveing data if valid condition
    if (data_position > 0 && data_frame[1] == data_position - 1) { // Valid condition
      UART_status = UART_PROCCESS ;
    }
    else if (data_position > 0 && data_frame[1] > 16 ) { // Data over size
      UART_status = UART_OVERSIZE ;
    }
    data_position ++ ; // Shift data frame position
    timeout_counter = 0 ; // Reset timeout if still reciving data
  }
}

ISR(TIMER0_OVF_vect)
{
  // If first data recived but position bellow data size, time out timer start counting 
  if (data_position > 0 && timeout_counter++ >= UART_TIMEOUT_TIME){
    UART_status = UART_TIMEOUT;
    timeout_counter = 0;
  }
}
int main(void) 
{
  //UART Setup
  UART_init(9600);
  //TWI setup
  MT_init(TW_FREQ_400K);
  DDRB = 0b00001111;
  while(1)
  {
    switch (UART_status)
    {
      case UART_PROCCESS :
      // Proccess valid Data received 
          PORTB |= (1<<PB1); //Debug
          if (data_frame[0] == start_code){
            UART_decode(); // Decode data frame
            UART_clear(); // Clear data frame
            UART2TWI_Process(); // Proccess decoded data and send to TWI bus
            UART_status = UART_COMPLETE; // UART Complete Procces
            UART_putc(COMPLETE_CODE); // send complete code
            PORTB |= (1<<PB2); //Debug
            }
      // Proccess Invalid Start Code
          else {
          UART_clear(); 
            UART_status = UART_INVALID;
            UART_putc(INVALID_CODE); // Send Invalid Code
            UART_status = UART_COMPLETE; //UART Complete Procces
          }
      break;
      case UART_COMPLETE :
      // UART Proccess Complete Handle
          UART_status = UART_READY ; // UART Idle , ready to receive data
          PORTB = 0x00 ; // Debug
      break;
      case UART_TIMEOUT :
      // Data Timeout handle
          UART_clear();
          UART_putc(TIMEOUT_CODE); // Send timeout code
          UART_status = UART_COMPLETE; //UART Complete Procces
      break;
      case UART_OVERSIZE :
      // Data oversize handle
          UART_clear();
          UART_putc(OVERSIZE_CODE);
          UART_status = UART_COMPLETE; //UART Complete Procces
      break;
    }
  }
}