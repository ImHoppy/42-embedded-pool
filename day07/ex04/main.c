#include <avr/io.h>
#include <util/delay.h>
#include <adc.h>
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
	DDRB |= (0b111 | (1 << PB4));
	init_rgb();

	adc_init(ADC_NORMAL);

	while (1)
	{
		// Get 8 bit value from ADC channel 0
		uint8_t adc = adc_read8(0);
		wheel(adc);
		PORTB = 0;
		if (adc < 64)
			PORTB = (1 << PB0);
		else if (adc < 128)
			PORTB = (1 << PB1);
		else if (adc < 192)
			PORTB = (1 << PB2);
		else
			PORTB = (1 << PB4);
		_delay_ms(20);
	}
}
