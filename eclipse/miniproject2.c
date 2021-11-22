/*
 * miniproject2.c
 *
 *  Created on: Sep 18, 2021
 *      Author: Mohamed Elshafie
 */
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
/*
 * sec1 for the units part of the seconds number and sec2 for the tens part
 * Same applies for the minutes and hours
 */
char sec1=0,sec2=0;
char min1=0,min2=0;
char hour1=0,hour2=0;

void Timer1_CTC_init(void){         //initialization function for timer 1

	TCNT1 = 0;              //initialize timer

	OCR1A = 1000;          //store compare value to count 1 second

	TIMSK |= (1<<OCIE1A);      //enable interrupt
	TCCR1A = (1<<FOC1A);        //bit set when non PWM mode is used
	TCCR1B = (1<<WGM12) | (1<<CS12) | (1<<CS10);    //CTC mode, prescalar = 1024
}
void INT0_Init(void){               //initialization function for external interrupt 0
	SREG  &= ~(1<<7);                  //disable interrupts
	GICR  |= (1<<INT0);                 // Enable external interrupt
	MCUCR |= (1<<ISC00);                //make interrupt with falling edge
	SREG  |= (1<<7);                    //enable interrupts
}
void INT1_Init(void){             //initialization function for external interrupt 1
	SREG  &= ~(1<<7);                   //disable interrupts
	GICR  |= (1<<INT1);                 // Enable external interrupt
	MCUCR |= (1<<ISC10) | (1<<ISC11);   //make interrupt with raising edge
	SREG  |= (1<<7);                    // Enable interrupts by setting I-bit
}
void INT2_Init(void){            //initialization function for external interrupt 2
	SREG  &= ~(1<<7);                   //disable interrupts
	GICR  |= (1<<INT2);                 // Enable external interrupt
	MCUCR &= ~(1<<ISC2);                //make interrupt with falling edge
	SREG  |= (1<<7);                    // Enable interrupts by setting I-bit
}

int main(void){
	INT0_Init();
	INT1_Init();
	INT2_Init();
	Timer1_CTC_init();

	SREG |=(1<<7);        //enable global interrupts

	DDRA = 0xff;        //make port A output
	DDRB = 0x00;        //make port B input
	DDRC = 0xff;        //make port C output
	DDRD = 0x00;        //make port D input

	PORTA = 0;         //initialize for segments to be turned off
	PORTC = 0;         //initialize for value of stop watch to be zero

	PORTB |= (1<<PB2);   //enable internal pull up
	PORTD |= (1<<PD2);   //enable internal pull up

	while(1){

		PORTA &= ~(1<<PA0);      // disable other unneeded 7 segment that was enabled before
		PORTA |= (1<<PA5);       // enable 7 segment needed to print the number on
		PORTC = sec1;            //print on the first 7 segment on the right.
		_delay_ms(2);
                       // same goes again with the rest 7 segments
		PORTA &= ~(1<<PA5);
		PORTA |= (1<<PA4);
		PORTC = sec2;
		_delay_ms(2);

		PORTA &= ~(1<<PA4);
		PORTA |= (1<<PA3);
		PORTC = min1;
		_delay_ms(2);

		PORTA &= ~(1<<PA3);
		PORTA |= (1<<PA2);
		PORTC = min2;
		_delay_ms(2);

		PORTA &= ~(1<<PA2);
		PORTA |= (1<<PA1);
		PORTC = hour1;
		_delay_ms(2);

		PORTA &= ~(1<<PA1);
		PORTA |= (1<<PA0);
		PORTC = hour2;
		_delay_ms(2);

	}

}
ISR(TIMER1_COMPA_vect){     //Interrupt service routine for timer 1
	sec1++;         // increment seconds with every interrupt
	if(sec1==10){       //check to increment tens of seconds and reset the units
		sec2++;
		sec1=0;
	}
	if(sec2==6){      //check to increment units of minutes and reset the tens of seconds
		min1++;
		sec2=0;
	}
	if(min1==10){      //check to increment tens of minutes and reset the units
			min2++;
			min1=0;
		}
	if(min2==6){      //check to increment units of hours and reset the tens of minutes
			hour1++;
			min2=0;
		}
	if(hour1==10){      //check to increment tens of hours and reset the units
			hour2++;
			hour1=0;
		}
	if(hour2==6){       //check to reset the tens of hours
			hour2=0;
		}
}
ISR(INT0_vect){             //Interrupt service routine for external interrupt 0
	//reset the stop watch by making the seconds, minutes and hours variable equal zero to start over
	sec1=0;
	sec2=0;
	min1=0;
	min2=0;
	hour1=0;
	hour2=0;
}
ISR(INT1_vect){           //Interrupt service routine for external interrupt 1
	// pause the stop watch by stopping clock source of timer 1
	TCCR1B &= ~(1<<CS12);
	TCCR1B &= ~(1<<CS10);
}
ISR(INT2_vect){           //Interrupt service routine for external interrupt 2
	// start the stop watch again by enabling the clock source again
	TCCR1B |= (1<<CS12);
	TCCR1B |= (1<<CS10);
}
