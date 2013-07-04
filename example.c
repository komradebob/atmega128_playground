//
// Simple chaser light example for the atmega128 in an STK500 development board
//
// 11/8/2013 R.M. Bowness
//
//

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>

#include <string.h>
#include <util/delay.h>


//#define F_CPU 19660800UL	      // Sbts up the default speed for delay.h
//#define F_CPU 32000UL         // Sets up the default speed for delay.h 
#define F_CPU 16000000UL         // Sets up the default speed for delay.h 

#define USART_BAUDRATE 9600    // Baud Rate value
#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)

void usart_init() {

//Enable communication in duplex mode
UCSR1A = (1 << U2X1);
UCSR1B |= (1 << RXEN1) | (1 << TXEN1);   // Turn on the transmission and reception circuitry
UCSR1C &= ~(1 << UMSEL1);
UCSR1C |= (1<<USBS1) | (1 << UCSZ10) | (1 << UCSZ11);

UBRR1L = BAUD_PRESCALE;                 // Load lower 8-bits of the baud rate value into the low byte of the UBRR register
UBRR1H = (BAUD_PRESCALE >> 8);          // Load upper 8-bits of the baud rate value.. 
}

#define DEBOUNCE	100

#define TRUE 1
#define FALSE 0

#define INITIALIZE 0x0
#define SINGLE 0x1
#define SCAN_VOICE 0x2
#define SCAN_KEY 0x3

#define MAX_RADIOS 7


#define D_COUNT	2000
unsigned long global_count;

#include <util/delay.h>

unsigned char active_radio, radio_status[MAX_RADIOS], single_op(unsigned char), scan(unsigned char);

int main()
{
	unsigned char mode;

	mode = SINGLE;
	DDRC = 255;
	PORTC=255;
	DDRA=0;

	PORTC = 255;	// Light up all the LEDs
	PORTA = 0xff;	// set pullup resistors

	usart_init();

while(1) {

	switch (mode) {

		case SINGLE:
			mode = single_op(mode);
			break;
		case SCAN_VOICE:
		case SCAN_KEY:
			mode = scan(mode);
			break;
		default:
			break;
	}
	
    }

}


///////////////////////////////////////////////////////////////////////
// check_mode_switch() Check to see if the mode switch is depressed
// returns 0 id not, 1 if it is pushed.
///////////////////////////////////////////////////////////////////////
int check_mode_switch()
{

	if (( PINA & 0x1) == 0) 
		return(1);
	else
		return(0);
}
	

///////////////////////////////////////////////////////////////////////
// single_op() Single radio operation mode. Look for a mode switch, ptt, or cw key input to change and move on.
// Returns the mode of the mode switch chosen
///////////////////////////////////////////////////////////////////////
unsigned char single_op(unsigned char mode)
{

unsigned char result;

	result = PINA & 0xfe;
	if (result != 254) {
		PORTC = result;
		active_radio = result;
	}
	if (check_mode_switch() == 1)
		mode = SCAN_VOICE;
	return(mode);
}


///////////////////////////////////////////////////////////////////////
// scan() Scan through the activated radios, send the message contained in the voice keyer, wait for it to be over, then move on
// Returns the mode selected to get out
///////////////////////////////////////////////////////////////////////
unsigned char scan(unsigned char mode)
{

int i;
unsigned char result;

	active_radio++;
	if (active_radio > MAX_RADIOS)
		active_radio = 1;

	if(global_count > 188000) {
		PORTC = ~((unsigned char)1<<active_radio);
		global_count = 0;
	}
	else
		global_count++;

	result = PINA;

	if(result != 255) {
		mode = SINGLE;
		PORTC = result;
	} 

	return(mode);
	
}
