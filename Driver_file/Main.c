

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

volatile int count=0,digit1=0,digit2=0,digit3=0;


int main()
{
	SREG=(1<<7);
	DDRC=0XFF; //8 pins 4 for decoder 4 for enabling 4 screens
	PORTC|=(1<<4);
	MCUCR|=((1<<ISC11)|(1<<ISC01));  //Falling edge trigger INT0 and INT1
	GICR|=((1<<INT0)|(1<<INT1)); //INT0.INT1 enabled
	TCCR0|=(1<<CS00);
	TIMSK|=(1<<TOIE0);
	while(1)
	{

		if(count==1000&&PORTC<26)
		{
			PORTC++;
			count=0;
		}
		else if(PORTC==26)
		{
			digit1++;
			PORTC=digit1;
			PORTC|=(1<<5);
			_delay_ms(100);
			if(digit1==6)
					{
						digit1=0;
						digit2++;
						PORTC=digit2;
						PORTC|=(1<<6);
						_delay_ms(100);
						if(digit2==9)
							{
								digit2=0;
								digit3++;
								PORTC=digit3;
								PORTC|=(1<<7);
								_delay_ms(100);

							}
					}
			PORTC=0;
			PORTC|=(1<<4);
			count=0;
		}

	}




	return 0;
}
ISR(TIMER0_OVF_vect)
{
	count++;
}
