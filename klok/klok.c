// klok.c

#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "systick.h"

volatile unsigned int systick_counter;
volatile unsigned char seconds;
volatile unsigned char minutes;
volatile unsigned char hours;
volatile unsigned char do_display;
volatile unsigned char do_seconds;

volatile unsigned int speed;
const unsigned int normal  = 1000;
const unsigned int fast    = 100;
const unsigned int faster  = 10;
const unsigned int fastest = 1;

volatile unsigned char digit;

const unsigned char bcd[] = {
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 
	0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 
	0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 
	0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 
	0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 
	0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 
};

void hours_inc( void ) {
	if ( hours < 23 ) {
		hours++;
	} else {
		hours = 0;
	}
}

void minutes_inc( void ) {
	if ( minutes < 59 ) {
		minutes++;
	} else {
		minutes = 0;
		hours_inc();
	}
}

void seconds_inc( void ) {
	if ( seconds < 59 ) {
		seconds++;
	} else {
		seconds = 0;
		minutes_inc();
	}
}

void systick_handler( void ) {
	systick_counter++;

	if ( systick_counter >= speed ) {
		systick_counter = 0;
		do_seconds = 1;
		do_display = 1;
	}	
}

void systick_reset( void ) {
	systick_counter = 0;
}

#define DIGIT_PORT PORTD
#define DIGIT_DDR  DDRD
#define DIGIT_MASK 0xff
#define DIGIT_1    0b80
#define DIGIT_2    0b40
#define DIGIT_3    0b20
#define DIGIT_4    0b10
#define DIGIT_5    0b08
#define DIGIT_6    0b04
#define DIGIT_7    0b02
#define DIGIT_8    0b01

#define BCD_PORT   PORTB
#define BCD_DDR    DDRB
#define BCD_MASK   0x0f

#define BUTTON_PORT PORTC
#define BUTTON_PIN	PO
#define BUTTON_DDR  DDRC
#define BUTTON_MASK 0xe0
#define BUTTON_1    0x80
#define BUTTON_2    0x40
#define BUTTON_3    0x20

void setup( void ) {
	
	cli();
	
	DIGIT_DDR   |= DIGIT_MASK;
	DIGIT_PORT  &= DIGIT_MASK;

	BCD_DDR     |= BCD_MASK;
	BCD_PORT    &= BCD_MASK;
	
	BUTTON_DDR  &= ~BUTTON_MASK;
	BUTTON_PORT |=  BUTTON_MASK;
	
	seconds = 0;
	minutes = 0;
	hours = 0;

	do_display = 0;
	do_seconds = 0;
	
	speed = normal;
	
	systick_setup();
	
	sei();
	
}

void display( void ) {

	switch ( digit ) {
		case 0: 
			BCD_PORT = ( bcd[ hours ] >> 4 ) & BCD_MASK;
			_delay_ms( 1 );
			DIGIT_PORT = DIGIT_7;
			_delay_ms( 1 );
			DIGIT_PORT = 0x00;
			digit++;
			break;
		case 1: 
			BCD_PORT = ( bcd[ hours ] ) & BCD_MASK;
			_delay_ms( 1 );
			DIGIT_PORT = DIGIT_6;
			_delay_ms( 1 );
			DIGIT_PORT = 0x00;
			digit++;
			break;
		case 2: 
			BCD_PORT = ( bcd[ minutes ] >> 4 ) & BCD_MASK;
			_delay_ms( 1 );
			DIGIT_PORT = DIGIT_5;
			_delay_ms( 1 );
			DIGIT_PORT = 0x00;
			digit++;
			break;
		case 3: 
			BCD_PORT = ( bcd[ minutes ] ) & BCD_MASK;
			_delay_ms( 1 );
			DIGIT_PORT = DIGIT_4;
			_delay_ms( 1 );
			DIGIT_PORT = 0x00;
			digit++;
			break;
		case 4: 
			BCD_PORT = ( bcd[ seconds ] >> 4 ) & BCD_MASK;
			_delay_ms( 1 );
			DIGIT_PORT = DIGIT_3;
			_delay_ms( 1 );
			DIGIT_PORT = 0x00;
			digit++;
			break;
		case 5: 
			BCD_PORT = ( bcd[ seconds ] ) & BCD_MASK;
			_delay_ms( 1 );
			DIGIT_PORT = DIGIT_2;
			_delay_ms( 1 );
			DIGIT_PORT = 0x00;
			digit++;
			break;
		case 6: 
			BCD_PORT = 0x0e;
			_delay_ms( 1 );
			DIGIT_PORT = DIGIT_8;
			_delay_ms( 1 );
			DIGIT_PORT = 0x00;
			digit++;
			break;
		case 7: 
			BCD_PORT = ( seconds | 0x0e ) & BCD_MASK;
			_delay_ms( 1 );
			DIGIT_PORT = DIGIT_1;
			_delay_ms( 1 );
			DIGIT_PORT = 0x00;
			digit = 0;
			do_display = 0;
			break;
	}	
}

void loop( void  ) {

	if ( ~BUTTON_PORT & BUTTON_1 ) {
		speed = fast;
	} else {
		speed = normal;
	}

	if ( ~BUTTON_PORT & BUTTON_2 ) {
		speed = faster;
	} else {
		speed = normal;
	}
		
	if ( ~BUTTON_PORT & BUTTON_3 ) {
		speed = fastest;
	} else {
		speed = normal;
	}
		
	if ( do_seconds ) {
		if ( ~BUTTON_PORT & BUTTON_MASK ) {
			minutes_inc();
		} else {
			seconds_inc();
		}
		do_seconds = 0;
	}
	
	if ( do_display ) {
		display();
	}
	
}

int main( void ) {
	
	setup();
	
	while ( 1 ) {
		loop();
	}
	
	return 0;
	
}
