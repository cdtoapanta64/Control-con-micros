/*
 * Prueba_Control_con_Micros.c
 *
 * Created: 11/12/2015 10:41:27
 *  Author: USUARIO
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include "lcd.h"

void puertos(void);
void configtimmers(void);
void adacelerador(void);
void adtanque(void);
void EEPROM_write(unsigned int uiAddress, unsigned char var);
unsigned char EEPROM_read(unsigned int uiAddress);

int main(void)
{
    while(1)
    {
        
    }
}

void adacelerador(void)
{
	ADMUX=0B01000000;
	ADCSRA=0B11000011;
	ADCSRA |= (1<<ADSC);
	// wait until conversion complete ADSC=0 -> Complete
	while (ADCSRA & (1<<ADSC));
	// Get ADC the Result
	valoracelerador = ADCW;
}
void adtanque(void)
{
	ADMUX=0B01000001;
	ADCSRA=0B11000011;
	ADCSRA |= (1<<ADSC);
	// wait until conversion complete ADSC=0 -> Complete
	while (ADCSRA & (1<<ADSC));
	// Get ADC the Result
	valortanque = ADCW;
	
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
