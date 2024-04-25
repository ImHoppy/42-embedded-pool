#ifndef SPI_H
#define SPI_H

#include <avr/io.h>

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
void spi_set_leds(uint32_t led0, uint32_t led1, uint32_t led2);

uint8_t spi_rx();

void spi_end();

#endif
