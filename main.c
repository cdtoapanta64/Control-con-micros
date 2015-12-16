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
int valorluzretro,valortemperatura,encendido=0;
int contadorvelocidad,contadordistancia;
int velocidad=0,velocidadmaxima=0,distanciatotal=0,distanciarecorrido=0,distanciausuario=0,tiempo;
int aux,aux1,velocidadm,velocidadMm,distanciam,distanciarecorridom,distanciausuariom;


void configinterrupcion(void);
void iniciomicro(void);
void configtimmers(void);
//////Analogo Digital///////
void adtemperatura(void);
void adacelerador(void);
void adtanque(void);
void adfreno(void);


void EEPROM_write(unsigned int uiAddress, unsigned char var);
unsigned char EEPROM_read(unsigned int uiAddress);

////funciones//////
void gasolina(void);
void acelerador(void);
void tempambiente(void);
void focos(void);
void mostrardistanciatotal(void);
void mostrardistanciarecorrido(void);
void mostrardistanciausuario(void);
void freno(void);
void retro(void);
///Programa Principal///
int main(void)
{
	iniciomicro();
	distanciatotal=EEPROM_read(1);
	distanciausuario=EEPROM_read(2);
	lcd_init(LCD_DISP_ON) ;
	lcd_clrscr();
	configtimmers();
	configinterrupcion();
    while(1)
    {
		distanciarecorrido=0;
		
		while (((PIND & (1<<PD4))==0))
		{
			
			freno();
			retro();
			if (encendido==1)
			{
				acelerador();
			} 
			else
			{
				OCR2B=0;				
			}
			tempambiente();
			gasolina();
			
			mostrardistanciausuario();
			mostrardistanciarecorrido();
			mostrardistanciatotal();
		
			///////Velocidad////////////
			velocidadm=velocidad*60;
			lcd_gotoxy(0,0);
			lcd_write_value(velocidadm,4);
			lcd_gotoxy(4,0);
			lcd_puts("rpm");
			///////Velocidad////////////
			velocidadMm=velocidadmaxima*60;
			lcd_gotoxy(7,0);
			lcd_puts("Vm");
			lcd_gotoxy(9,0);
			lcd_write_value(velocidadMm,4);
			lcd_gotoxy(13,0);
			lcd_puts("rpm");	
		
		}
		
		lcd_clrscr();
		OCR2B=0;
	
	}
}

void mostrardistanciatotal(void)
{
	///////Distancia Total////////////
	distanciam=distanciatotal*0.1;
	lcd_gotoxy(16,0);
	lcd_puts("Dt");
	lcd_gotoxy(18,0);
	lcd_write_value(distanciam,5);
	lcd_gotoxy(23,0);
	lcd_puts("km");
}
void mostrardistanciarecorrido(void)
{
	/////Distancia  Recorrido////////////
	distanciarecorridom=distanciarecorrido*0.1;
	lcd_gotoxy(0,1);
	lcd_puts("Dr");
	lcd_gotoxy(2,1);
	lcd_write_value(distanciarecorridom,4);
	lcd_gotoxy(6,1);
	lcd_puts("km");
}
void mostrardistanciausuario(void)
{
	/////Distancia  Usuario////////////
	distanciausuariom=distanciausuario*0.1;
	lcd_gotoxy(8,1);
	lcd_puts("Du");
	lcd_gotoxy(10,1);
	lcd_write_value(distanciausuariom,4);
	lcd_gotoxy(14,1);
	lcd_puts("km");
}
void focos(void)
{
	adfreno();
	if(valorfreno<1024)///////
	{
		if (valorluzfreno<500)/////////
		{
			
		} 
		else
		{
		}
		
	}
	else
	{
		
	}
	
	
	
}/////////////////
void tempambiente(void)
{
	int temp;
	adtemperatura();
	temp=valortemperatura*0.107526;
	lcd_gotoxy(25,0);
	lcd_puts("Tem:");
	lcd_gotoxy(29,0);
	lcd_write_value(temp,2);
	lcd_gotoxy(31,0);
	lcd_puts("C");
	
}
void gasolina(void)
{
	int aux;
	adtanque();
	aux=(valortanque-512)*(0.1953125);
	lcd_gotoxy(23,1);
	lcd_puts("Fuel:");
	lcd_gotoxy(28,1);
	lcd_write_value(aux,3);
	lcd_gotoxy(31,1);
	lcd_puts("%");
	PORTD&=~(1<<PD0);
	
	if(aux<=25)
	{
		PORTD|=(1<<PD0);
		lcd_gotoxy(16,1);
		lcd_puts("T.Bajo");		
	} 
	else
	{
		lcd_gotoxy(16,1);
		lcd_puts("      ");
	}
}
void acelerador(void)
{
	int aux;
	adacelerador();
	
	aux=-0.1953125*(valoracelerador-1024);
	OCR2B=aux;
	
}
void freno(void)
{
	int aux0;
	adfreno();
	aux0=0.1953125*(valorfreno-512);
	if (aux0<80)
	{
		PORTB|=(1<<PB2);
		if ((PINB & (1<<PB4))==0)
		{
			PORTD|=(1<<PD1);
		}
		else
		{
			PORTD&=~(1<<PD1);
		}
	} 
	else
	{
		PORTB&=~(1<<PB2);
		PORTD&=~(1<<PD1);
	}
}
void retro(void)
{
	if (PINB & (1<<PB6) )
	{
		PORTB|=(1<<PB1);
		if ((PINB & (1<<PB5))==0)
		{
			PORTD|=(1<<PD5);
		}
		else
		{
			PORTD&=~(1<<PD5);
		}
	} 
	else
	{
		PORTB&=~(1<<PB1);
		PORTD&=~(1<<PD5);
	}
}
///velocidad///
ISR(TIMER0_COMPA_vect)
{
    aux++;
	if (aux>=4)
	{
		contadorvelocidad++;
		aux=0;
	}
	TCNT0=0;
}
///distancia///
ISR(TIMER0_COMPB_vect)
{
	contadordistancia++;
	if (contadordistancia>=12)
	{
		distanciatotal++;
		distanciarecorrido++;
		distanciausuario++;
		contadordistancia=0;
		EEPROM_write(distanciatotal,1);
		EEPROM_write(distanciausuario,2);
		
	} 

}
ISR(TIMER1_COMPA_vect)
{
	
	aux1=velocidad;
	velocidad=contadorvelocidad;
	
	contadorvelocidad=0;
	if (aux1>velocidadmaxima)
	{
		velocidadmaxima=aux1;

	}
	
		
}
ISR(INT0_vect)
{
	encendido++;
	if (encendido==2)
	{
		encendido=0;
	}
}
ISR(INT1_vect)
{
 distanciausuario=0;
}

void iniciomicro(void)
{
	
	DDRA=0xff;
	DIDR0=0b00111111;
	
	 DDRB=0b00000110;
	PORTB=0b11111000;
	
	DDRC=0xFF;

	
	DDRD=0b01100011;
	PORTD|=(1<<2);
	PORTD|=(1<<3);
	PORTD|=(1<<4);
	

}
void configinterrupcion(void)
{
 //interrupciones
 EICRA=0b00001010;
 EIMSK=0b00000011;
 sei();

}
void configtimmers(void)
{
	//timer 0
	TCCR0A=0b00000000;
	TCCR0B=0b00000110;
	TIMSK0=0b00000110;
	OCR0A=1;
	OCR0B=1;
	TCNT0=0;
	
	//timer 1
	TCCR1A=0b00000000;
	TCCR1B=0b00001100;
	TCCR1C=0b00000000;
	TIMSK1=0b00000010;
	OCR1A=31250;
	TCNT1=0;
	
	//timer 2
	TCCR2A=0b00100011;
	TCCR2B=0b00001011;//TCCR2B=0b00001011
	OCR2A=100;
	OCR2B=0;
	ASSR=0b00000000;
}
void adtemperatura(void)
{
	ADMUX=0B10000101;
	ADCSRA=0B11000011;
	ADCSRA |= (1<<ADSC);
	// wait until conversion complete ADSC=0 -> Complete
	while (ADCSRA & (1<<ADSC));
	// Get ADC the Result
	valortemperatura= ADCW;
}

void adfreno(void)
{
	ADMUX=0b01000010;
	ADCSRA=0b11000011;
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
