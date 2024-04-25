#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>
#include <string.h>
#include <spi.h>
#include "history.h"
#include <uart.h>

bool is_hex(char c)
{
	if (c >= '0' && c <= '9')
		return true;
	if (c >= 'A' && c <= 'F')
		return true;
	if (c >= 'a' && c <= 'f')
		return true;
	return false;
}

uint8_t hextodec(char c)
{
	if (c >= '0' && c <= '9')
		return c - '0';
	if (c >= 'A' && c <= 'F')
		return c - 'A' + 10;
	if (c >= 'a' && c <= 'f')
		return c - 'a' + 10;
	return 0;
}

bool check_line(char line[LINE_LEN])
{
	if (strlen(line) != 8)
	{
		return false;
	}
	for (uint8_t i = 0; i < 6; i++)
	{
		if (!is_hex(line[i]))
		{
			return false;
		}
	}
	if ((line[7] != 'D' || line[7] != 'd') && line[8] >= '0' && line[8] <= '9')
	{
		return false;
	}
	return true;
}

#define UNKNOW_LED_MSG "Unknown LED\n\r"

int main()
{
	spi_init(SPI_MASTER);
	uart_init(UART_ALL);

	char line[LINE_LEN] = {0}; // Format is #RRGGBBDX\0

	uint32_t leds[3] = {0};

	while (1)
	{
		read_line(line);
		if (!check_line(line))
		{
			uart_printstr(ERROR_MSG);
			continue;
		}
		uint32_t color = 0;
		uint8_t r = hextodec(line[0]) << 4 | hextodec(line[1]);
		uint8_t g = hextodec(line[2]) << 4 | hextodec(line[3]);
		uint8_t b = hextodec(line[4]) << 4 | hextodec(line[5]);
		color = LOW_BRIGHTNESS | ((uint32_t)r << 16) | ((uint32_t)g << 8) | b;
		uint8_t led = hextodec(line[7]);

		if (led > 2)
		{
			uart_printstr(UNKNOW_LED_MSG);
			continue;
		}
		leds[led] = color;
		spi_set_leds(leds[0], leds[1], leds[2]);
	}

	spi_end();
}
