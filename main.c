/*
 * Prueba_Control_con_Micros.c
 *
 * Created: 11/12/2015 10:41:27
 *  Author: USUARIO
 */ 

#define F_CPU 8000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include "lcd.h"

int valoracelerador,valortanque,valorfreno,valorluzfreno;
int valorluzretro;

void configinterrupcion(void);
void puertos(void);
void configtimmers(void);
void adacelerador(void);
void adtanque(void);
void adfreno(void);
void adluzfreno(void);
void adluzretro(void);

void EEPROM_write(unsigned int uiAddress, unsigned char var);
unsigned char EEPROM_read(unsigned int uiAddress);

int main(void)
{
	lcd_init(LCD_DISP_ON);
	lcd_clrscr();
	lcd_gotoxy(0,0);
	
    while(1)
    {
        
    }
}

void configinterrupcion(void)
{
 //interrupciones
 
}
void configtimmers(void)
{
	//timer 0
	TCCR0A=0b00000000;
	TCCR0B=0b00000110;
	TIMSK0=0b00000010;
	
	//timer 1
	TCCR1A=0b00000000;
	TCCR1B=0b00001100;
	TCCR1C=0b00000000;
	TIMSK1=0b00000010;
	OCR1A=31250;
	TCNT1=0;
	
	//timer 2
	TCCR2A=0b00100011;
	TCCR2B=0b00001011;
	OCR2A=100;
	OCR2B=0;
	ASSR=0b00000000;
}

ISR(TIMER0_COMPA_vect)
{
	
}

ISR(TIMER1_COMPA_vect)
{
	
}


void adluzfreno(void)
{
	ADMUX=0B01000010;
	ADCSRA=0B11000011;
	ADCSRA |= (1<<ADSC);
	// wait until conversion complete ADSC=0 -> Complete
	while (ADCSRA & (1<<ADSC));
	// Get ADC the Result
	valorluzfreno= ADCW;
}
void adluzretro(void)
{
	ADMUX=0B01000100;
	ADCSRA=0B11000011;
	ADCSRA |= (1<<ADSC);
	// wait until conversion complete ADSC=0 -> Complete
	while (ADCSRA & (1<<ADSC));
	// Get ADC the Result
	valorluzretro= ADCW;
}
void adfreno(void)
{
	ADMUX=0B01000011;
	ADCSRA=0B11000011;
	ADCSRA |= (1<<ADSC);
	// wait until conversion complete ADSC=0 -> Complete
	while (ADCSRA & (1<<ADSC));
	// Get ADC the Result
	valorfreno= ADCW;
}
void adacelerador(void)
{
	ADMUX=0B01000000;
	ADCSRA=0B11000011;
	ADCSRA |= (1<<ADSC);
	// wait until conversion complete ADSC=0 -> Complete
	while (ADCSRA & (1<<ADSC));
	// Get ADC the Result
	valoracelerador= ADCW;
}
void adtanque(void)
{
	ADMUX=0B01000001;
	ADCSRA=0B11000011;
	ADCSRA |= (1<<ADSC);
	// wait until conversion complete ADSC=0 -> Complete
	while (ADCSRA & (1<<ADSC));
	// Get ADC the Result
	valortanque= ADCW;
	
}
void EEPROM_write(unsigned int uiAddress, unsigned char var)
{
	/* Wait for completion of previous write */
	while(EECR & (1<<EEPE));
	
	EEAR = uiAddress;
	EEDR = var;
	/* Write logical one to EEMPE */
	EECR |= (1<<EEMPE);
	/* Start eeprom write by setting EEPE */
	EECR |= (1<<EEPE);
}
unsigned char EEPROM_read(unsigned int uiAddress)
{
	/* Wait for completion of previous write */
	while(EECR & (1<<EEPE))
	;
	/* Set up address register */
	EEAR = uiAddress;
	/* Start eeprom read by writing EERE */
	EECR |= (1<<EERE);
	/* Return data from Data Register */
	return EEDR;
}
