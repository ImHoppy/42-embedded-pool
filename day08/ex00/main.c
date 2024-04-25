#include <avr/io.h>
#include <util/delay.h>
#include <spi.h>

int main()
{
	spi_init(SPI_MASTER);

	// Start transmission
	spi_tx32(0);
	// Set LED 0 to RED
	spi_tx_led(0x1, 255, 0, 0);
	// Turn off LED 1
	spi_tx_led(0, 0, 0, 0);
	// Turn off LED 2
	spi_tx_led(0, 0, 0, 0);

	// Stop transmission
	spi_tx32(0xFFFFFFFF);
	spi_end();
}
