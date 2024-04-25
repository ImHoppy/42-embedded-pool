#include <avr/io.h>
#include <util/delay.h>
#include <spi.h>
#include <adc.h>

#define RED_LED 0xff0000
#define GREEN_LED 0x00ff00
#define BLUE_LED 0x0000ff

int main()
{
	spi_init(SPI_MASTER);
	adc_init(ADC_NORMAL);

	while (1)
	{
		uint8_t percentage = adc_read8(0) / 2.55;
		uint32_t led0 = 0;
		uint32_t led1 = 0;
		uint32_t led2 = 0;
		PORTB = 0;
		if (percentage >= 33)
			led0 = LOW_BRIGHTNESS | 0xFF0000;
		if (percentage >= 66)
			led1 = LOW_BRIGHTNESS | 0xFF0000;
		if (percentage >= 100)
			led2 = LOW_BRIGHTNESS | 0xFF0000;
		spi_set_leds(led0, led1, led2);
		_delay_ms(20);
	}

	spi_end();
}
