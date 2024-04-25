#ifndef SPI_H
#define SPI_H

#include <avr/io.h>

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

void spi_init(spi_mode_t mode);

void spi_tx(uint8_t data);
void spi_tx16(uint16_t data);
void spi_tx32(uint32_t data);
void spi_tx_led(uint8_t bright, uint8_t r, uint8_t g, uint8_t b);

uint8_t spi_rx();

void spi_end();

#endif
