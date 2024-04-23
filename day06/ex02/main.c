#include <avr/io.h>
#include <util/delay.h>
#include <rgb.h>

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
	init_rgb();

	uint8_t i = 0;
	while (1)
	{
		wheel(i);
		_delay_ms(15);
		i++;
	}
}
