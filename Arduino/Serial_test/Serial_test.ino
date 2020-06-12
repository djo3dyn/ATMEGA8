#include <avr\io.h>
#include <util\delay.h>

#define DELAY 2

unsigned long current_milis;
unsigned char data[5];
int shift = 1;
int i = 0 ;

void data_1()
{
  data[4] = 0b00000001;
  data[3] = 0b00000001;
  data[2] = 0b00000001;
  data[1] = 0b00000001;
  data[0] = 0b00000001;
}
void data_2()
{
  data[4] = 0b00001111;
  data[3] = 0b00000001;
  data[2] = 0b00001111;
  data[1] = 0b00001000;
  data[0] = 0b00001111;
}

void data_3()
{
  data[4] = 0b00001111;
  data[3] = 0b00000001;
  data[2] = 0b00001111;
  data[1] = 0b00000001;
  data[0] = 0b00001111;
}

void data_4()
{
  data[4] = 0b00001001;
  data[3] = 0b00001001;
  data[2] = 0b00001111;
  data[1] = 0b00000001;
  data[0] = 0b00000001;
}
void data_5()
{
  data[4] = 0b00001111;
  data[3] = 0b00001000;
  data[2] = 0b00001111;
  data[1] = 0b00000001;
  data[0] = 0b00001111;
}
void data_6()
{
  data[4] = 0b00001111;
  data[3] = 0b00001000;
  data[2] = 0b00001111;
  data[1] = 0b00001001;
  data[0] = 0b00001111;
}

void data_7()
{
  data[4] = 0b00001111;
  data[3] = 0b00000001;
  data[2] = 0b00000001;
  data[1] = 0b00000001;
  data[0] = 0b00000001;
}
void data_8()
{
  data[4] = 0b00001111;
  data[3] = 0b00001001;
  data[2] = 0b00001111;
  data[1] = 0b00001001;
  data[0] = 0b00001111;
}
void data_9()
{
  data[4] = 0b00001111;
  data[3] = 0b00001001;
  data[2] = 0b00001111;
  data[1] = 0b00000001;
  data[0] = 0b00001111;
}
void data_0()
{
  data[4] = 0b00001111;
  data[3] = 0b00001001;
  data[2] = 0b00001001;
  data[1] = 0b00001001;
  data[0] = 0b00001111;
}

void scan()
{
  PORTB = ~data[i];
  PORTD = shift;
  shift = (shift >= 0x10) ? 1 : shift <<=1;
  i++ ;
  if (i>4) i = 0 ;
  _delay_ms(DELAY); 
}
int main(void)
{
  DDRB = 0xFF; // Anoda LED
  DDRD = 0xFF; // Katoda LED
  PORTD = 0xFE;
  

  while(1)
  {
    while(current_milis < 1000)
    {
      data_1();
      scan();
      current_milis ++;
    }
    while(current_milis >= 1000 && current_milis < 2000)
    {
      data_2();
      scan();
      current_milis ++;
    }
    while(current_milis >= 2000 && current_milis < 3000)
    {
      data_3();
      scan();
      current_milis ++;
    }
    while(current_milis >= 3000 && current_milis < 4000)
    {
      data_4();
      scan();
      current_milis ++;
    }
    while(current_milis >= 4000 && current_milis < 5000)
    {
      data_5();
      scan();
      current_milis ++;
    }
    while(current_milis >= 5000 && current_milis < 6000)
    {
      data_6();
      scan();
      current_milis ++;
    }
    while(current_milis >= 6000 && current_milis < 7000)
    {
      data_7();
      scan();
      current_milis ++;
    }
    while(current_milis >= 7000 && current_milis < 8000)
    {
      data_8();
      scan();
      current_milis ++;
    }
    while(current_milis >= 8000 && current_milis < 9000)
    {
      data_9();
      scan();
      current_milis ++;
    }
    while(current_milis >= 9000 && current_milis < 10000)
    {
      data_0();
      scan();
      current_milis ++;
    }
    while(current_milis >= 10000)
    {
      current_milis = 0;
    }
    
  }
}
