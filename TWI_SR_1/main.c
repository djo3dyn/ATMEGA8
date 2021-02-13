#include <avr/io.h>
#include <util/delay.h>

#include <avr/interrupt.h>
#include <util/twi.h>

#ifndef F_CPU
#define F_CPU 16000000UL  // 16 MHz
#endif

#define SLAVE_ADDRESS 1

// Define status data proccess
#define TWI_READY 0
#define TWI_PROCCESS 1

// Interupt variable
volatile uint8_t TWI_status ;
volatile char data_frame[16] ;
volatile uint8_t data_position ;

// Get Slave Address from Input
uint8_t get_SLA(void)
{
  uint8_t _sla ;
  _sla = PIND >> 3 ;
  return _sla ; 
}

void SR_init(uint8_t address)
{
  cli();
  // load address into TWI address register
  TWAR = address << 1;
  // set the TWCR to enable address matching and enable TWI, clear TWINT, enable TWI interrupt
  TWCR |= (1<<TWIE) | (1<<TWEA) | (1<<TWINT) | (1<<TWEN);
  sei();
}

void SR_stop(void)
{
  // clear acknowledge and enable bits
  cli();
  TWCR = 0;
  TWAR = 0;
  sei();
}

static void data_frame_clear()
{
  // Clear Data Frame
  for (int i = 0 ; i < 16 ; i++) {
    data_frame[i] = 0 ;
  }
}

ISR(TWI_vect)
{
  switch(TW_STATUS)
  {
    case TW_SR_DATA_ACK:
      // received data from master, write to data frame
      if (data_position < 16) data_frame[data_position] = TWDR ;
      data_position++;
      TWCR |= (1<<TWIE) | (1<<TWEA) | (1<<TWINT) ;
      break;
    case TW_BUS_ERROR:
      // some sort of erroneous state, prepare TWI to be readdressed
      TWCR = 0;
      TWCR = (1<<TWIE) | (1<<TWINT) | (1<<TWEA) | (1<<TWEN); 
      break;
    case TW_SR_STOP :
      TWCR = 0; // Clear Interupt
      TWI_status = TWI_PROCCESS ;
      data_position = 0; // Reset data position 
      TWCR |= (1<<TWIE) | (1<<TWEA) | (1<<TWINT) | (1<<TWEN); // Init again
    break;
    default:
      TWCR = (1<<TWIE) | (1<<TWINT) | (1<<TWEA) | (1<<TWEN);
      break;
  }
  PORTD ^= (1<<PD2); // Debug
} 
int main(void) 
{
  
  DDRB = 0xFF ;
  //Port D init
  DDRD |= (1<<DDD2);
  PORTD |= (1 << PD3) | (1 << PD4) | (1 << PD5) ;
  SR_init(get_SLA);
  while(1)
  {
    
    PORTB = data_frame[0];
    _delay_ms(500);
    PORTB = data_frame[1];
    _delay_ms(500);
    PORTB = data_frame[2];
    _delay_ms(500);
    PORTB = data_frame[3];
    _delay_ms(500);
    PORTB = data_frame[4];
    _delay_ms(500);
    
  }
 
}