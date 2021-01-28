/*
    Analog to digital sample ;
*/
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

void ADC_setup()
{
    // Use AVCC pin voltage reference 
    ADMUX |= _BV(REFS0);
    ADMUX &= ~_BV(REFS1);
    // Left order 10 bit Resolution
    ADMUX &= ~_BV(ADLAR);
    // ADC enable
    ADCSRA |= _BV(ADEN) ;
    // ADC Clock Prescaller
    ADCSRA |= _BV(ADPS2) | _BV(ADPS0) ; // 101 :: Prescaller 32 of 16.000.000Mhz = 500kHz

}

int ADC_read(int _adc_pin)
{
    ADMUX &= 0xF0 ; // Clear Pin
    ADMUX |= (0x0F & _adc_pin) ; //Set Pin
    //Start Convertion
    ADCSRA |= _BV(ADSC);
    // Wait for it to finish
    while (ADCSRA & _BV(ADSC));
    // Save to variable
    return ADC ;
}

int main(void)
{
    DDRB |= _BV(PB2);
    int ADC1_value ;
    ADC_setup();
    while (1)
    {
        ADC1_value =  ADC_read(2);
        if (ADC1_value > 512)
        {
            PORTB |= _BV(PB2);
        }
        else
        {
            PORTB &= ~_BV(PB2);
        }
        _delay_ms(100);

    }
    

}