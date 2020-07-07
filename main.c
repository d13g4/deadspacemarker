//infos
#define F_CPU 500000		//processorspeed (only for calibration) 
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <avr/eeprom.h>		//eeprom read/write
#include <avr/io.h>
#include <avr/sleep.h>		//sleep-mode
#include <avr/interrupt.h>	//interrupts
#include <avr/pgmspace.h>
#include <util/delay.h>		//wait
#include <util/twi.h>		//twi-communication
#include <inttypes.h>		//integer
#include <util/atomic.h>
#include <math.h>		//math
#include <avr/wdt.h>		//watchdog-timer


//global variables
int ledcounter=0;
int istrue;
//setup at startup
void setup(){
//energy-awareness
    ACSR = 0x80;	// disable analog comparator
    wdt_disable();	// disable watchdog-timer
//timer0
    TCCR0A = 0x02;      // Clear Timer on Compare Match (CTC) mode
    TCCR0B = 0x05;
    TIFR |= 0x01;       // clear interrupt flag
    TIMSK = 0x01;       // TC0 compare match A interrupt enable
    OCR0A  = 255;      // number to count up to (in Zyklen)
//timer1
    TCCR1A  &= (0<<COM1A0)|(0<<COM1A1);  // CTC Modus
    TCCR1B |= (1<<WGM12)|(1<<CS10)|(1<<CS12);   // Prescaler 1024
    OCR1A = 240*125;	// number to count up to (in 1/125 seconds)
    TIMSK |= (1<<OCIE1A);      //Compare Interrupt Enable

//external interrupt 0
    GIMSK |= (1<<INT0);
    MCUCR |= (0<<ISC01)|(0<<ISC00);
    PORTD |= (1<<PD2);	// PD2 in PORTD set to 1 (pullup activation)
//pin declaration
    DDRB = 0b11111111; // LED on PB0 - BP7
    DDRD &= ~(1 << PD2);  // PD2 in DDRD to 0

// 
    sei();              // global interrupt enable
}
int main(){
setup();
while(1){
    switch(ledcounter){
	case 0:
	    PORTB = 0b00000001;
	    break;
	case 1:
	    PORTB = 0b00000011;
	    break;
	case 2:
	    PORTB = 0b00000010;
	    break;
	case 3:
	    PORTB = 0b00000110;
	    break;
	case 4:
	    PORTB = 0b00000100;
	    break;
	case 5:
	    PORTB = 0b00001100;
	    break; 
    	case 6:
	    PORTB = 0b00001000;
	    break; 	
    	case 7:
	    PORTB = 0b00011000;
	    break; 
   	case 8:
	    PORTB = 0b00010000;
	    break; 
   	case 9:
	    PORTB = 0b00110000;
	    break; 
    	case 10:
	    PORTB = 0b00100000;
	    break; 
    	case 11:
	    PORTB = 0b01100000;
	    break; 
    	case 12:
	    PORTB = 0b01000000;
	    break; 
    	case 13:
	    PORTB = 0b11000000;
	    break; 
    	case 14:
	    PORTB = 0b10000000;
	    break; 
    	case 15:
	    PORTB = 0b10000001;
	    break; 
	case 16:
	    ledcounter=0;
	    break;   
    }
}
}
// interrupt service routine (ISR) for timer 0 A compare match
ISR (TIMER0_COMPA_vect){
if (istrue==1){
    ledcounter++;
    istrue=0;
}
else{
    istrue=1;
    }
}

