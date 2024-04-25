#include <avr/io.h>
#include <util/delay.h>
#include <spi.h>

#define DDR_SPI DDRB
#define DD_SS PB2
#define DD_MOSI PB3
#define DD_MISO PB4
#define DD_SCK PB5

typedef enum
{
	SPI_MASTER,
	SPI_SLAVE
} spi_mode_t;

void spi_init(spi_mode_t mode)
{
	// SPCR: SPI Control Register (19.5.1)
	// SPE: SPI Enable
	// MSTR: Master/Slave Select
	// SPR0: SPI Clock Rate Select 0
	SPCR = 0;
	if (mode == SPI_MASTER)
	{
		// Set as master
		SPCR |= (1 << MSTR);
		// Set clock rate fck/16
		SPCR |= (1 << SPR0);
		// Set MOSI and SCK output, all others input
		DDR_SPI = (1 << DD_MOSI) | (1 << DD_SCK) | (1 << DD_SS);
		// SPE: SPI Enable
		SPCR |= (1 << SPE);
	}
	else if (mode == SPI_SLAVE)
	{
		// Set MISO output, all others input
		DDR_SPI = (1 << DD_MISO);
		// SPE: SPI Enable
		SPCR |= (1 << SPE);
	}
	// else SPI off

	// SPSR - SPI Status Register (19.5.2)
}

void spi_tx(uint8_t data)
{
	// Start transmission
	// SPDR - Spi Data Register (19.5.3)
	SPDR = data;
	// Wait for transmission complete
	while (!(SPSR & (1 << SPIF)))
		;
}

void spi_tx16(uint16_t data)
{
	spi_tx((uint8_t)(data >> 8));
	spi_tx((uint8_t)data);
}

void spi_tx32(uint32_t data)
{
	spi_tx((uint8_t)(data >> 24));
	spi_tx((uint8_t)(data >> 16));
	spi_tx((uint8_t)(data >> 8));
	spi_tx((uint8_t)data);
}

void spi_tx_led(uint8_t bright, uint8_t r, uint8_t g, uint8_t b)
{
	// 3 Bit of padding and 5 Bit of brightness
	spi_tx(0b11100000 | bright);
	// Blue
	spi_tx(b);
	// Green
	spi_tx(g);
	// Red
	spi_tx(r);
}

uint8_t spi_rx()
{
	// Wait for reception complete
	while (!(SPSR & (1 << SPIF)))
		;
	// Return data register
	return SPDR;
}

void spi_end()
{
	SPCR &= ~_BV(SPE);
}
