

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

int count=0;


int main()
{
	SREG=(1<<7);
	DDRC=0XFF; //8 pins 4 for decoder 4 for enabling 4 screens
	PORTC=((1<<4)|(1<<5)|(1<<6)|(1<<7));
	MCUCR|=((1<<ISC11)|(1<<ISC01));  //Falling edge trigger INT0 and INT1
	GICR|=((1<<INT0)|(1<<INT1)); //INT0.INT1 enabled
	TCCR0|=(1<<CS00);
	TIMSK|=(1<<TOIE0);
	while(1)
	{
		if(count==4000&&PORTC<255)
		{
			PORTC++;
			count=0;
		}
		else if(PORTC==250)
			PORTC&=~((1<<0)|(1<<1)|(1<<2)|(1<<3));

	}




	return 0;
}
ISR(TIMER0_OVF_vect)
{
	count++;
}
