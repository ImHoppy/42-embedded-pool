#include <avr/io.h>
#include <util/delay.h>
#include <spi.h>

#define RED_LED 0xff0000
#define GREEN_LED 0x00ff00
#define BLUE_LED 0x0000ff

int main()
{

	uint8_t i = 0;

	spi_init(SPI_MASTER);

	while (1)
	{
		switch (i)
		{
		case 0:
			/* code */
			spi_set_leds(LOW_BRIGHTNESS | RED_LED, 0, 0);
			break;
		case 1:
			/* code */
			spi_set_leds(0, LOW_BRIGHTNESS | RED_LED, 0);
			break;
		case 2:
			/* code */
			spi_set_leds(0, 0, LOW_BRIGHTNESS | RED_LED);
			break;
		default:
			spi_set_leds(0, 0, 0);
			break;
		}
		i = (i + 1) % 4;

		_delay_ms(250);
	}

	spi_end();
}
