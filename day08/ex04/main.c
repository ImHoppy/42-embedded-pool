#include <avr/io.h>
#include <avr/interrupt.h>
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

#define CONVERT_RGB(r, g, b) ((uint32_t)(r) << 16) | ((uint32_t)(g) << 8) | (b)

// Modified wheel function to return the color instead of setting it
uint32_t wheel(uint8_t pos)
{
	pos = 255 - pos;
	if (pos < 85)
	{
		return CONVERT_RGB(255 - pos * 3, 0, pos * 3);
	}
	else if (pos < 170)
	{
		pos = pos - 85;
		return CONVERT_RGB(0, pos * 3, 255 - pos * 3);
	}
	else
	{
		pos = pos - 170;
		return CONVERT_RGB(pos * 3, 255 - pos * 3, 0);
	}
}

void timer0_init()
{
	// Set Timer0 in CTC mode
	TCCR0A |= (1 << WGM01);

	// Set compare match register for 10ms intervals
	OCR0A = 10 * (F_CPU / 1024 / 1000);

	// Enable Timer0 compare match interrupt
	TIMSK0 |= (1 << OCIE0A);
}
void timer0_start()
{
	// Set prescaler to 1024
	TCCR0B |= (0b101 << CS00);
}

void timer0_stop()
{
	// Set prescaler to 0
	TCCR0B &= ~(0b111 << CS00);
}

ISR(TIMER0_COMPA_vect)
{
	static uint8_t i = 0;
	i++;
	uint32_t led0 = wheel((i + 50) % 0xFF);
	uint32_t led1 = wheel((i + 25) % 0xFF);
	uint32_t led2 = wheel((i + 00) % 0xFF);
	spi_set_leds(LOW_BRIGHTNESS | led0, LOW_BRIGHTNESS | led1, LOW_BRIGHTNESS | led2);
}

int main()
{
	spi_init(SPI_MASTER);
	uart_init(UART_ALL);

	timer0_init();
	sei();

	char line[LINE_LEN] = {0}; // Format is #RRGGBBDX\0

	uint32_t leds[3] = {0};

	// Clear the LEDs
	spi_set_leds(leds[0], leds[1], leds[2]);

	while (1)
	{
		read_line(line);
		if (strcasecmp(line, "fullrainbow") == 0)
		{
			timer0_start();
			memset(leds, 0, sizeof(leds));
			continue;
		}
		if (!check_line(line))
		{
			uart_printstr(ERROR_MSG);
			continue;
		}
		timer0_stop();
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
