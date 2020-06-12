/*
 Segment digit move here

*/

#if COM_ANODA

#define NONE    0b11111111
#define ZERO    0b11000000
#define ONE     0b11111001 //0b00000110
#define TWO     0b10100100 //0b01011011 
#define THREE   0b10110000 //0b01001111
#define FOUR    0b10011001 //0b01100110
#define FIVE    0b10010010 //0b01101101
#define SIX     0b10000010 //0b01111101
#define SEVEN   0b11111000 //0b00000111
#define EIGHT   0b10000000 //0b01111111
#define NINE    0b10010000 //0b01101111
#define CHR_A   0b10001000 //0b01110111
#define CHR_B   0b10000011 //0b01111100
#define CHR_C   0b11000110 //0b00111001
#define CHR_D   0b10100001 //0b01011110
#define CHR_E   0b10000110 //0b01111001
#define CHR_F   0b10001110 //0b01110001

#else

#define NONE    0b00000000
#define ZERO    0b00111111
#define ONE     0b00000110
#define TWO     0b01011011 
#define THREE   0b01001111
#define FOUR    0b01100110
#define FIVE    0b01101101
#define SIX     0b01111101
#define SEVEN   0b00000111
#define EIGHT   0b01111111
#define NINE    0b01101111
#define CHR_A   0b01110111
#define CHR_B   0b01111100
#define CHR_C   0b00111001
#define CHR_D   0b01011110
#define CHR_E   0b01111001
#define CHR_F   0b01110001

#endif
