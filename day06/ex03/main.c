#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>
#include <rgb.h>
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

void read_hex(char line[8])
{
	char c;
	uint8_t i = 0;
	while (1)
	{
		c = uart_rx();
		// Handle backspace
		if (c == 0x7F)
		{
			if (i > 0)
			{
				line[i] = '\0';
				i--;
				uart_tx('\b');
				uart_tx(' ');
				uart_tx('\b');
			}
			continue;
		}
		// Handle newline
		if (c == '\r' && i == 7)
		{
			uart_tx('\n');
			uart_tx('\r');
			line[i] = '\0';
			break;
		}
		if (i > 6)
			continue;
		// Ignore first character if not '#'
		if (i == 0 && c != '#')
			continue;
		// Ignore characters that are not hex
		if (i > 0 && !is_hex(c))
			continue;
		uart_tx(c);
		line[i] = c;
		i++;
	}
}

int main()
{
	init_rgb();
	uart_init(UART_ALL);

	char hex_rgb_line[8];
	while (1)
	{
		read_hex(hex_rgb_line);

		set_rgb(
			(hextodec(hex_rgb_line[1]) << 4) + hextodec(hex_rgb_line[2]), // R
			(hextodec(hex_rgb_line[3]) << 4) + hextodec(hex_rgb_line[4]), // G
			(hextodec(hex_rgb_line[5]) << 4) + hextodec(hex_rgb_line[6])  // B
		);
	}
}
