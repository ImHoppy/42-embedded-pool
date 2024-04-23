#include <avr/io.h>
#include <util/delay.h>

#define RED_LED (1 << PD5)
#define GREEN_LED (1 << PD6)
#define BLUE_LED (1 << PD3)

int main()
{
	DDRD |= RED_LED | GREEN_LED | BLUE_LED;

	PORTD = RED_LED;
	while (1)
	{
		_delay_ms(1000);
		switch (PORTD)
		{
		case RED_LED:
			PORTD = GREEN_LED;
			break;
		case GREEN_LED:
			PORTD = BLUE_LED;
			break;
		case BLUE_LED:
			PORTD = RED_LED;
			break;
		}
	}
}
