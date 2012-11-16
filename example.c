//
// Simple chaser light example for the atmega128 in an STK500 development board
//
// 11/8/2010 R.M. Bowness
//
//

#include <avr/io.h>
#include <avr/interrupt.h>

//#define F_CPU 100000UL	      // Sets up the default speed for delay.h
#define F_CPU 32000UL         // Sets up the default speed for delay.h 
#include <util/delay.h>

int main()
{
	char i,j;

	DDRC = 255;
	PORTC=255;

	while(1) {
		for(i=0;i<8;i++) {
		
			PORTC &= (~_BV(i));

			_delay_ms(2048);
			
			PORTC |= (_BV(i));

//			_delay_ms(2048);
	
		}
	}
}
