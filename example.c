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

#define DEBOUNCE	100

#define TRUE 1
#define FALSE 0

#define INITIALIZE 0x0
#define SINGLE 0x1
#define SCAN_VOICE 0x2
#define SCAN_KEY 0x3
#define PROGRAM 0x4

#define MAX_RADIOS 7

#define RADIO_SWITCH_PORT PORTC
#define XMIT_PORT PORTD
#define XMIT_BIT 0x01
#define START_STOP_PORT PORTB
#define START_STOP_BIT 0x01
#define PROGRAM_PORT PORTB
#define PROGRAM_BIT 0x02

#define D_COUNT	2000

#include <util/delay.h>

void read_switches();

unsigned char xmit_status, radio_switches, start_stop, program_switch, active_radio, radio_status[MAX_RADIOS], program(unsigned char), single_op(unsigned char), scan(unsigned char);

int main()
{
	unsigned char result, mode;

			// Ports - pin high is off or inactive, low is active or selected.
	mode = SINGLE;
	DDRA=0;		// PORT A is input from radio switches
	DDRB=0;		// PORT B is input 
	DDRC = 0xff;	// set port C for output
	DDRD=0;		// PORT D is input from voice keyer PIND.0
	DDRE = 0xff;		// PORT E is input from random test switches

	PORTA = 0xff;	// set pullup resistors on port A - switches
	PORTB = 0xff;	// set pullup resistors on port E - switches
	PORTC = 0xff;	// PORT C is output for LEDs, Turn them all off.
	PORTD = 0xff;	// set pullup resistors on port D - Input from voice keyer on PIN.0
	PORTE = 0xff;	// set pullup resistors on port E 


while(1) {

	switch (mode) {

		case SINGLE:
			mode = single_op(mode);
			break;
		case SCAN_VOICE:
		case SCAN_KEY:
			mode = scan(mode);
			break;
		case PROGRAM:
			mode = program(mode);
			break;
		default:
			break;
	}
	
//	read_switches();
//	if(program_switch == TRUE)
//		mode = PROGRAM;
//	if((start_stop == TRUE) && (mode == SCAN_VOICE))
//		mode = SINGLE;
	
    }

}

///////////////////////////////////////////////////////////////////////
// void read_switches()
// always returns - Sets xmit_status,radio_switches, start_stop, program globals
///////////////////////////////////////////////////////////////////////
void read_switches(void)

{

	radio_switches = RADIO_SWITCH_PORT;

	start_stop = PORTB  & START_STOP_BIT;

	program_switch = PORTB  & PROGRAM_BIT;

	xmit_status = PORTD & XMIT_BIT;
}

///////////////////////////////////////////////////////////////////////
// program() Set the active radios
// returns SINGLE if complete, otherwise returns the mode it was called with
///////////////////////////////////////////////////////////////////////
unsigned char program(unsigned char mode)
{

unsigned char result;

	result = ~PORTC;

	PINA = result;
	mode = PROGRAM;

	return(mode);
}

///////////////////////////////////////////////////////////////////////
// status_xmit() returns the status of the xmit bit
// returns TRUE if transmitting, FALSE if not
///////////////////////////////////////////////////////////////////////
int status_xmit()
{
	if((PIND & 0x01) == 0)
		return(TRUE);
	else
		return(FALSE);
}

///////////////////////////////////////////////////////////////////////
// check_mode_switch() Check to see if the mode switch is depressed
// returns 0 id not, 1 if it is pushed.
///////////////////////////////////////////////////////////////////////
int check_mode_switch()
{

	if (( PINA & 0x1) == 0) 
		return(TRUE);
	else
		return(FALSE);
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
unsigned char radio_switches, start_stop, txmit, result;

//	Basic logic/flow
//	increment the active radio, if it is more than the MAX_RADIOS, reset to 1
//
// 	read the radio switches & start/stop
//	if start/stop == stop, then stop, set mode = SINGLE and return else
//	if pind.0 is low and pina is all high, then return else
//	if pind.0 is high and pina is all high, move to next radio and return else
//	if pina is not all high, stop, set active_radio from pina, set mode = SINGLE, and return
//	start/stop = start, pina = all high, pind = low, return Playing msg now.


	

	
	active_radio++;
	if (active_radio > MAX_RADIOS)
		active_radio = 1;

	PORTC = ~((unsigned char)1<<active_radio);
	_delay_ms(200);

	while((PIND & 0x1) == 1) 
		result = PINA;

	if(result != 255) {
		mode = SINGLE;
		PORTC = result;
	} 

	return(mode);
	
}

