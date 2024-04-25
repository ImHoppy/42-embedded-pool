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
	spi_tx_led(0x1, 0, 255, 0);
	spi_tx_led(0x1, 0, 0, 255);

	// Stop transmission
	spi_tx32(0xFFFFFFFF);
	spi_end();
}
