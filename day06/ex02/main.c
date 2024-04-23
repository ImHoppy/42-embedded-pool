#include <avr/io.h>
#include <util/delay.h>
#include <rgb.h>

#define RED_LED (1 << PD5)
#define GREEN_LED (1 << PD6)
#define BLUE_LED (1 << PD3)

#define YELLOW_LED (RED_LED | GREEN_LED)
#define CYAN_LED (GREEN_LED | BLUE_LED)
#define MAGENTA_LED (RED_LED | BLUE_LED)
#define WHITE_LED (RED_LED | GREEN_LED | BLUE_LED)

void wheel(uint8_t pos)
{
	pos = 255 - pos;
	if (pos < 85)
	{
		set_rgb(255 - pos * 3, 0, pos * 3);
	}
	else if (pos < 170)
	{
		pos = pos - 85;
		set_rgb(0, pos * 3, 255 - pos * 3);
	}
	else
	{
		pos = pos - 170;
		set_rgb(pos * 3, 255 - pos * 3, 0);
	}
}

int main()
{
	DDRD |= RED_LED | GREEN_LED | BLUE_LED;

	uint8_t i = 0;
	init_rgb();
	while (1)
	{
		wheel(i);
		_delay_ms(15);
		i++;
	}
}
