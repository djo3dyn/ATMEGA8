/* Aplikasi Jadwal Waktu Sholat
 * ATMEGA8 
 
 * by Judin
 
 *judincahsigerung@gmail.com

*/

// Initializations Internal Include Library
#include <stdlib.h>
#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>

// Initializations External Include Library
#include "ds1307/ds1307.h"
#include "lcd16024b\lcd.h"

// Initializations Time Variables
uint8_t year = 0;
uint16_t year_w = 2000;
uint8_t month = 0;
uint8_t day = 0;
uint8_t hour = 0;
uint8_t minute = 0;
uint8_t second = 0;

// Initializations Geo Variables
float lintang = 0 ;
float bujur = 0 ;
uint8_t timezone = 0;
uint8_t height = 0;

// Initializations Waktu Sholat
uint8_t dzuhur_h = 12;
uint8_t dzuhur_m = 0;
uint8_t ashar_h = 15;
uint8_t ashar_m = 10;
uint8_t maghrib_h = 17;
uint8_t maghrib_m = 48;
uint8_t isya_h = 19;
uint8_t isya_m = 2;
uint8_t imsyak_h = 4;
uint8_t imsyak_m = 30;
uint8_t subuh_h = 4;
uint8_t subuh_m = 40;
uint8_t terbit_h = 5;
uint8_t terbit_m = 58;


// Functions....

// Calculatin for geo varibale
void get_jwsvar(void)
{
	
}

// Kalkulasi waktu dzuhur
void get_dzuhur(void)
{
	
}

// Kalkulasi waktu Ashar
void get_ashar(void)
{
	
}

// Kalkulasi waktu maghrib
void get_maghrib(void)
{
	
}

// Kalkulasi waktu Isya
void get_isya(void)
{
	
}

// Kalkulasi waktu subuh
void get_subuh(void)
{
	
}

// Kalkulasi waktu imsyak
void get_imsyak(void)
{
	
}

// Kalkulasi waktu Terbit
void get_terbit(void)
{
	
}

// Update RTC, Refresh tampilan Jam dan Menit di LCD dilakukan berkala max tiap 1 detik
void rtc_update_lcd(void)
{
	//RTC get date
	ds1307_getdate(&year, &month, &day, &hour, &minute, &second);
		
	//RTC Covert string
	char jam[2];
	char menit[2];
		
	//LCD Print Jam
	lcd_xy(12,1);
		
	itoa(hour/10,jam,10);
	lcd_putstr(jam);
	itoa(hour%10,jam,10);
	lcd_putstr(jam);
		
	lcd_putstr(":");
		
	itoa(minute/10,menit,10);
	lcd_putstr(menit);
	itoa(minute%10,menit,10);
	lcd_putstr(menit);
	
	// Pada waktu 00:00, pada statup dan reset jadwal sholat di Uptade
	if (minute==0 && hour ==0)
	{
		daily_get();
	}
}

// Slide Show tampilan waktu sholat di LCD
void jws_print_lcd(int slc )
{
	// JWS Character Variable
	char dzuhur_hc[2]="00";
	char dzuhur_mc[2]="00";
	
	char ashar_hc[2]="00";
	char ashar_mc[2]="00";
	
	char maghrib_hc[2]="00";
	char maghrib_mc[2]="00";
	
	char isya_hc[2]="00";
	char isya_mc[2]="00";
	
	char imsyak_hc[2]="00";
	char imsyak_mc[2]="00";
	
	char subuh_hc[2]="00";
	char subuh_mc[2]="00";
	
	char terbit_hc[2]="00";
	char terbit_mc[2]="00";
	
	// Posisi kursor LCD di row 2
	lcd_xy(1,2);
	
	//Memilih jadwal yg ditampilkan
	switch (slc)
	{
		case 1 :
		// Print Dzuhur
		lcd_putstr("DZUHUR  ");
		lcd_xy(12,2);
		itoa(dzuhur_h/10,dzuhur_hc,10);
		lcd_putstr(dzuhur_hc);
		itoa(dzuhur_h%10,dzuhur_hc,10);
		lcd_putstr(dzuhur_hc);
		
		lcd_putstr(":");
		
		itoa(dzuhur_m/10,dzuhur_mc,10);
		lcd_putstr(dzuhur_mc);
		itoa(dzuhur_m%10,dzuhur_mc,10);
		lcd_putstr(dzuhur_mc);
		
		break;
		
		case 2 :
		// Print Ashar
		lcd_putstr("ASHAR  ");
		lcd_xy(12,2);
		itoa(ashar_h/10,ashar_hc,10);
		lcd_putstr(ashar_hc);
		itoa(ashar_h%10,ashar_hc,10);
		lcd_putstr(ashar_hc);
		
		lcd_putstr(":");
		
		itoa(ashar_m/10,ashar_mc,10);
		lcd_putstr(ashar_mc);
		itoa(ashar_m%10,ashar_mc,10);
		lcd_putstr(ashar_mc);
		
		break;
		
		case 3 :
		// Print maghrib
		lcd_putstr("MAGHRIB ");
		lcd_xy(12,2);
		itoa(maghrib_h/10,maghrib_hc,10);
		lcd_putstr(maghrib_hc);
		itoa(maghrib_h%10,maghrib_hc,10);
		lcd_putstr(maghrib_hc);
		
		lcd_putstr(":");
		
		itoa(maghrib_m/10,maghrib_mc,10);
		lcd_putstr(maghrib_mc);
		itoa(maghrib_m%10,maghrib_mc,10);
		lcd_putstr(maghrib_mc);
		
		break;
		
		case 4 :
		// Print isya
		lcd_putstr("ISYA   ");
		lcd_xy(12,2);
		itoa(isya_h/10,isya_hc,10);
		lcd_putstr(isya_hc);
		itoa(isya_h%10,isya_hc,10);
		lcd_putstr(isya_hc);
		
		lcd_putstr(":");
		
		itoa(isya_m/10,isya_mc,10);
		lcd_putstr(isya_mc);
		itoa(isya_m%10,isya_mc,10);
		lcd_putstr(isya_mc);
		
		break;
		
		case 5 :
		// Print imsyak
		lcd_putstr("IMSYAK  ");
		lcd_xy(12,2);
		itoa(imsyak_h/10,imsyak_hc,10);
		lcd_putstr(imsyak_hc);
		itoa(imsyak_h%10,imsyak_hc,10);
		lcd_putstr(imsyak_hc);
		
		lcd_putstr(":");
		
		itoa(imsyak_m/10,imsyak_mc,10);
		lcd_putstr(imsyak_mc);
		itoa(imsyak_m%10,imsyak_mc,10);
		lcd_putstr(imsyak_mc);
		
		break;
		
		case 6 :
		// Print subuh
		lcd_putstr("SUBUH  ");
		lcd_xy(12,2);
		itoa(subuh_h/10,subuh_hc,10);
		lcd_putstr(subuh_hc);
		itoa(subuh_h%10,subuh_hc,10);
		lcd_putstr(subuh_hc);
		
		lcd_putstr(":");
		
		itoa(subuh_m/10,subuh_mc,10);
		lcd_putstr(subuh_mc);
		itoa(subuh_m%10,subuh_mc,10);
		lcd_putstr(subuh_mc);
		
		break;
		
		case 7 :
		// Print terbit
		lcd_putstr("TERBIT  ");
		lcd_xy(12,2);
		itoa(terbit_h/10,terbit_hc,10);
		lcd_putstr(terbit_hc);
		itoa(terbit_h%10,terbit_hc,10);
		lcd_putstr(terbit_hc);
		
		lcd_putstr(":");
		
		itoa(terbit_m/10,terbit_mc,10);
		lcd_putstr(terbit_mc);
		itoa(terbit_m%10,terbit_mc,10);
		lcd_putstr(terbit_mc);
		
		break;
	}
}
	
// Update Jadwal Waktu Sholat, Refresh tampilan Tanggal, Bulan dan Tahun di LCD dilakukan harian
void daily_get(void)
{
	//tahun ditambah 2000, di RTC hanya nunjuk 19
	year_w = 2000+year;
	
	//get Waktu Sholat
	get_jwsvar();
	get_dzuhur();
	get_ashar();
	get_maghrib();
	get_isya();
	get_subuh();
	get_imsyak();
	get_terbit();
	
	//Character hari bulan tahun
	char hari[2];
	char bulan[2];
	char tahun[4];
	
	//Posisi Kursor di baris 1 
	lcd_xy(1,1);
	
	//LCD Print Tanggal	
	itoa(day/10,hari,10);
	lcd_putstr(hari);
	itoa(day%10,hari,10);
	lcd_putstr(hari);
		
	lcd_putstr("/");
		
	itoa(month/10,bulan,10);
	lcd_putstr(bulan);
	itoa(month%10,bulan,10);
	lcd_putstr(bulan);
		
	lcd_putstr("/");
		
	itoa(year_w,tahun,10);
	lcd_putstr(tahun);
	
}


// Fungsi Utama...
int main(void) {

	// Input/Output Ports initialization
	DDRC = 0x00;
	DDRD = 0xFF;
	DDRB = 0xFF;
	
	//LCD Initializations
	lcd_iniit();
	
	//RTC Initializations
	ds1307_init();
	sei();
	
	//Other Initializations
	rtc_update_lcd();
	daily_get();
	int slc_int = 1 ;
	
	// Fungsi yg looping...
	while(1)
	{
		//RTC update and print LCD
		rtc_update_lcd();
		PORTB = (0<<0);
		
		_delay_ms(500);
		
		//Efek kedip
		lcd_xy(14,1);
		lcd_putstr(" ");
		
		//Print jadwal sholat di LCD
		jws_print_lcd(slc_int);
		slc_int++;
		if (slc_int > 7 ) { slc_int = 1 ;}
		_delay_ms(500);
		
	}

}
