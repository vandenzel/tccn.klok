/*
 * systick.h - Een millisecondeteller voor de MinimusUSB
 *
 * (C)2011 Alex van Denzel, CC-BY-SA
 */

#ifndef minimus_systick_h
#define minimus_systick_h

void systick_setup( void );
void systick_handler( void );
void systick_reset( void );

#endif // minimus_systick_h
