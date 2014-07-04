/*
 * systick.c - Een millisecondeteller voor de MinimusUSB
 *
 * (C)2011 Alex van Denzel, CC-BY-SA
 */

#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>

#include "systick.h"

void systick_setup( void ) {
	
	// set timer0 (the 8-bit timer) to CTC mode, with TOP=249
	// WGM0 2:0 = 2
	// OCR0A = 249
	
	TCCR0A = 0x02;		// 0000 0010: COM0A = 0; COM0B = 0; WGM0 = 0x02;
	TCCR0B = 0x03;		// 0000 0011: FOC0A = 0; FOC0B = 0; WGM0 = 0x02; CS0 = 3; 
	TCNT0 = 0;
	OCR0A = 249;		// 250 ticks == 1 ms
	TIMSK0 = 0x02;		// 0000 0010: OCIE0B = 0; OCIE0A = 1; TOIE0 = 0;
	
	systick_reset();
}

ISR( TIMER0_COMPA_vect ) {
	systick_handler();
}
