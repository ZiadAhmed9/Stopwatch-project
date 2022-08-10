
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>


//from calculations we will use Timer0 with clk/1024 to prescale it giving
//976 ticks will be needed to complete one second, every overflow is 256 tick so we will count 4 overflows
//TCNT0 will be initialized with 6 to ensure 250*4=1000 ---> exactly 1 second
unsigned char second = 0;
unsigned char minute = 0;
unsigned char count=0;
volatile  int del=0;
//first we initialize the timer


ISR(TIMER0_OVF_vect)
{
	count++;
	if(count==4)
	{
		second++;
		if(second==60)
		{
			second=0;
			minute++;
		}
		if(minute==60)
		{
			minute=0;
			second=0;
		}
		count=0;
	}
}

void INT0_init_start(void)
{
	DDRD&=~(1<<PD2);
	PORTD|=(1<<PD2); //internal pull up enabled
	GICR|=(1<<INT0);
	MCUCR|=(1<<ISC01);  //FALLING EDGE
	SREG|=(1<<7);   //ENABLE GLOBAL INTERRUPT
}

ISR(INT0_vect)
{
	TCCR0|=((1<<CS00)|(1<<CS02));  //PRESCALER
	TCNT0=6;     //STARTING POINT
	SREG|=(1<<7);  //ENABLE GLOBAL INTERRUPT WHEN OVERFLOW HAPPENS
	TIMSK|=(1<<TOIE0);
}

void INT1_init_pause(void)
{
	DDRD&=~(1<<PD3);
	PORTD|=(1<<PD3); //internal pull up enabled
	GICR|=(1<<INT1);
	MCUCR|=(1<<ISC11);  //FALLING EDGE
	SREG|=(1<<7);   //ENABLE GLOBAL INTERRUPT

}
void init_reset(void)
{
	second=0;
	minute=0;
	TCNT0=6;
	PORTC=0xf0;
	_delay_ms(250);
	PORTC^=0xf0;
	_delay_ms(250);
	PORTC^=0xf0;
	_delay_ms(250);
	PORTC^=0xf0;
	_delay_ms(250);
	PORTC^=0xf0;
	_delay_ms(250);
	PORTC^=0xf0;
	_delay_ms(250);
	PORTC^=0xf0;
	_delay_ms(250);
	PORTC^=0xf0;
	_delay_ms(250);
}

ISR(INT1_vect)
{
	TCCR0&=~((1<<CS00)|(1<<CS02)); //DISABLES THE TIMER

}
int main()
{
	DDRC=0xFF;
	PORTC|=((1<<4)|(1<<5)|(1<<6)|(1<<7));
	PORTC&=~((1<<0)|(1<<1)|(1<<2)|(1<<3));
	INT0_init_start();
	INT1_init_pause();
	while(1)
	{
		del=0;

				PORTC = second % 10;
				PORTC|= (1<<4);

				_delay_ms(1);
				PORTC = second / 10;
				PORTC|= (1<<5);

				_delay_ms(1);
				PORTC = minute % 10;
				PORTC|= (1<<6);
				_delay_ms(1);
				PORTC = minute / 10;
				PORTC|= (1<<7);
				_delay_ms(1);
				//we check now if the stop button is on hold
				while(!(PIND&(1<<PD3)))
				{
						del++;
						if(del==30000)
						{
							init_reset();
						}

				}






	}
}
