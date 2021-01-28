#include <avr/io.h>
#define F_CPU 8000000UL  // 8 MHz
#include <util/delay.h>

#include <avr/interrupt.h>
#include <util/twi.h>

#define SLAVE_ADDRESS 0b00100000

#define	TW_FREQ_100K 0
#define	TW_FREQ_250K 1
#define	TW_FREQ_400K 2

uint8_t value; // contains the received value


#define	TW_FREQ_100K 0
#define	TW_FREQ_250K 1
#define	TW_FREQ_400K 2


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

int MT_start()
{
  uint8_t _status ;

  // TWI-ENable , TWI Interrupt Enable & Start TWI
  TWCR = (1<<TWINT)|(1<<TWSTA)| (1<<TWEN) ;
  while (!(TWCR & (1<<TWINT))); //Wait TWINT enable
  if (TW_STATUS != TW_START && TW_STATUS != TW_REP_START) _status |= 0b00000001 ;

  return _status ;

}

int MT_write_sla(uint8_t _addr )
{
  uint8_t _status ;
  // Write SLA
  TWDR = (_addr << 1) | 0x00; // Load Slave Address, write mode ;
  TWCR = (1<<TWINT) | (1<<TWEN);
  while (!(TWCR & (1 << TWINT))); //Wait TWINT enable
  if (TW_STATUS != TW_MT_SLA_ACK ) _status |= 0b00000010 ;

  return _status ;
}
// Transmiter Write function
int MT_write_data(uint8_t _value)
{
  uint8_t _status ;

  // Write Data 1
  TWDR = _value ; // Load data
  TWCR = (1<<TWINT) | (1<<TWEN);
  while (!(TWCR & (1 << TWINT))); //Wait TWINT enable
  if (TW_STATUS != TW_MT_DATA_ACK ) _status |= 0b00000100 ;

  return _status ;
}

void MT_stop()
{
  // Stop TWI
  TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
}

int main(void) 
{
  // TWI setup
  MT_init(TW_FREQ_400K);
  DDRB |= (1 << DDB2) | (1 << DDB1) | (1 << DDB0) ;
  value = 0 ;
  while(1)
  {
    PORTB |= MT_start();
    PORTB |= MT_write_sla(SLAVE_ADDRESS);
    PORTB |= MT_write_data(value);
    MT_stop();
    value++;
    if (value >= 255) value = 0;
    _delay_ms(500);
  }
 
}