#include <avr/io.h>
#include <util/delay.h>

#define RED_LED (1 << PD5)
#define GREEN_LED (1 << PD6)
#define BLUE_LED (1 << PD3)

#define YELLOW_LED (RED_LED | GREEN_LED)
#define CYAN_LED (GREEN_LED | BLUE_LED)
#define MAGENTA_LED (RED_LED | BLUE_LED)
#define WHITE_LED (RED_LED | GREEN_LED | BLUE_LED)

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
			PORTD = YELLOW_LED;
			break;
		case YELLOW_LED:
			PORTD = CYAN_LED;
			break;
		case CYAN_LED:
			PORTD = MAGENTA_LED;
			break;
		case MAGENTA_LED:
			PORTD = WHITE_LED;
			break;
		case WHITE_LED:
			PORTD = RED_LED;
			break;
		}
	}
}
