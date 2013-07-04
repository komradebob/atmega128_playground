//
// Simple chaser light example for the atmega128 in an STK500 development board
//
// 11/8/2010 R.M. Bowness
//
//

#include <avr/io.h>
#include <avr/interrupt.h>

//#define F_CPU 19660800UL	      // Sbts up the default speed for delay.h
//#define F_CPU 32000UL         // Sets up the default speed for delay.h 

#define DEBOUNCE	100

#define TRUE 1
#define FALSE 0

#include <util/delay.h>

int main()
{
	char i;
	unsigned char SCAN,result=0,check_switches();
	

	DDRC = 255;
	PORTC=255;
	DDRA=0;
	SCAN=TRUE;	
while(1) {

	i = PINA;

	PORTC = i;

	_delay_ms(10);

}
}
