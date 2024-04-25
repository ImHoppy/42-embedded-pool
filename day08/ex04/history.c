#include "history.h"
#include <avr/io.h>
#include <stdbool.h>
#include <string.h>
#include "history.h"
#include <uart.h>

void add_to_history(char history[4][12], char line[LINE_LEN])
{
	for (uint8_t i = 3; i > 0; i--)
	{
		for (uint8_t j = 0; j < 12; j++)
		{
			history[i][j] = history[i - 1][j];
		}
	}
	for (uint8_t i = 0; i < 12; i++)
	{
		history[0][i] = line[i];
	}
}

static void update_current_line(char line[LINE_LEN], history_data_t *history_data, uint8_t *len)
{
	strcpy(line, history_data->history[history_data->history_index]);
	*len = strlen(line);
	history_data->cursor_position = *len;
	uart_printstr("\33[2K\r#");
	uart_printstr(line);
}

static void handle_esc_sequence(char line[LINE_LEN], history_data_t *history_data, uint8_t *len)
{
	uint8_t c = uart_rx(); // Second character of escape sequence
	if (c == '[')
	{
		c = uart_rx(); // Third character of escape sequence
		if (c == 'A')  // Up arrow
		{
			if (history_data->history_index < 3 && history_data->history[history_data->history_index + 1][0] != '\0')
			{
				history_data->history_index++;
				update_current_line(line, history_data, len);
			}
		}
		else if (c == 'B') // Down arrow
		{
			if (history_data->history_index > 0)
			{
				history_data->history_index--;
				update_current_line(line, history_data, len);
			}
		}
		else if (c == 'C') // Right arrow
		{
			if (history_data->cursor_position < *len)
			{
				uart_tx('\033');
				uart_tx('[');
				uart_tx('C');
				history_data->cursor_position++;
			}
		}
		else if (c == 'D') // Left arrow
		{
			if (history_data->cursor_position > 0)
			{
				uart_tx('\033');
				uart_tx('[');
				uart_tx('D');
				history_data->cursor_position--;
			}
		}
	}
}

void read_line(char line[LINE_LEN])
{
	uint8_t len = 0;
	static history_data_t history_data = {0};
	history_data.history_index = 0;
	history_data.cursor_position = 0;

	uart_printstr("#");
	while (1)
	{
		char c = uart_rx();
		uart_printhex(c);
		uart_tx(' ');
		if (c == ' ' || c == '\t' || c == '#')
			continue;
		if (c == 0x7f) // backspace
		{
			if (history_data.cursor_position > 0 && len > 0)
			{
				for (int i = history_data.cursor_position; i <= len; i++)
					line[i - 1] = line[i];
				len--;
				history_data.cursor_position--;
				uart_tx('\b'); // move cursor back
				// Print the rest of the line
				for (int i = history_data.cursor_position; i < len; i++)
					uart_tx(line[i]);
				uart_tx(' ');
				uart_tx('\b');
				for (int i = history_data.cursor_position; i < len; i++)
				{
					uart_tx('\b');
				}
				// uart_printstr("\33[D");
			}
			continue;
		}
		if (c == '\r') // Enter
		{
			if (len < 8)
			{
				uart_printnl();
				uart_printstr(ERROR_MSG);
				len = 0;
				history_data.cursor_position = 0;
				uart_printstr("#");
				continue;
			}
			uart_tx('\n');
			uart_tx('\r');
			line[len] = '\0';
			add_to_history(history_data.history, line);
			break;
		}
		if (c == 27) // Arrow key or escape sequence
		{
			handle_esc_sequence(line, &history_data, &len);
			continue;
		}
		// Insert character
		if (len < 11 && history_data.cursor_position < 11)
		{
			if (history_data.cursor_position < len)
			{
				for (int i = len; i > history_data.cursor_position; i--)
					line[i] = line[i - 1];
				line[history_data.cursor_position] = c;
				uart_tx(c);
				for (int i = history_data.cursor_position + 1; i <= len; i++)
					uart_tx(line[i]);
				for (int i = history_data.cursor_position + 1; i <= len; i++)
					uart_tx('\b');
			}
			else
			{
				line[len] = c;
				uart_tx(c);
			}
			len++;
			history_data.cursor_position++;
		}
	}
}
