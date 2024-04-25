#include <avr/io.h>
#include <util/delay.h>
#include <spi.h>

#define RED_LED 0xff0000
#define GREEN_LED 0x00ff00
#define BLUE_LED 0x0000ff

#define YELLOW_LED (RED_LED | GREEN_LED)
#define CYAN_LED (GREEN_LED | BLUE_LED)
#define MAGENTA_LED (RED_LED | BLUE_LED)
#define WHITE_LED (RED_LED | GREEN_LED | BLUE_LED)

int main()
{
	const uint32_t leds_cycles[] = {
		RED_LED,
		GREEN_LED,
		BLUE_LED,
		YELLOW_LED,
		CYAN_LED,
		MAGENTA_LED,
		WHITE_LED,
	};
	uint8_t i = 0;

	spi_init(SPI_MASTER);

	while (1)
	{
		spi_set_leds(((uint32_t)0x01 << 24) | leds_cycles[i], 0, 0);
		i = (i + 1) % (sizeof(leds_cycles) / sizeof(leds_cycles[0]));

		_delay_ms(1000);
	}

	spi_end();
}
