#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

volatile uint8_t pressed = 0;

ISR(INT0_vect)
{
	if (!pressed)
	{
		PORTB ^= (1 << PB0);
		pressed = 1;
		EICRA = (0b11 << ISC00);
	}
	else
	{
		pressed = 0;
		EICRA = (0b10 << ISC00);
	}
}

int main(void)
{
	DDRB = 1 << PB0;

	EIMSK = 1 << INT0;
	EICRA = (0b10 << ISC00);
	sei();

	while (1)
	{
	}
	return 0;
}
