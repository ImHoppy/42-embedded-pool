#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <uart.h>
#include <stdbool.h>

#define MAX_INPUT_LENGTH 64
#define MAX_USERNAME_LENGTH (MAX_INPUT_LENGTH - 1)
#define MAX_PASSWORD_LENGTH (MAX_INPUT_LENGTH - 1)

int16_t compare_strings(const char *s1, const char *s2, uint8_t n)
{
	uint8_t i;

	i = 0;
	while (i < n && s1[i] == s2[i] && s1[i] && s2[i])
		i++;
	if (i != n)
		return ((uint8_t)s1[i] - (int8_t)s2[i]);
	return (0);
}

uint8_t is_ascii(const char c)
{
	return c >= 0x20 && c <= 0x7E;
}

uint8_t string_length(const char *s)
{
	if (!s)
		return 0;
	uint8_t i = 0;
	while (s[i] != 0)
		++i;
	return i;
}

char handle_input(char input[64], uint8_t input_len, uint8_t is_password)
{
	static uint8_t input_index = 0;
	const char c = uart_rx();
	if (c == 0x7F)
	{
		if (input_index > 0)
		{
			--input_index;
			uart_printstr("\b \b");
		}
		return 0;
	}
	if (c == '\r')
	{
		input[input_index] = 0;
		input_index = 0;
		uart_printstr("\n\r");
		return 1;
	}
	if (!is_ascii(c))
		return 0;
	if (input_index < input_len)
	{
		input[input_index] = c;
		++input_index;
		if (is_password)
			uart_tx('*');
		else
			uart_tx(c);
	}
	return 0;
}

void debug(const char *str)
{
	for (char c = *str; c != 0; ++str, c = *str)
		uart_printhex(c);
}

int login(const char *good_username, const char *good_password)
{
	char username[MAX_USERNAME_LENGTH + 1] = {0};
	char password[MAX_PASSWORD_LENGTH + 1] = {0};

	uart_printstr("Login as: ");
	while (!handle_input(username, MAX_USERNAME_LENGTH, false))
		;

	uart_printstr(username);
	uart_printstr("'s password: ");
	while (!handle_input(password, MAX_PASSWORD_LENGTH, true))
		;

	if (compare_strings(username, good_username, string_length(good_username)) != 0 ||
		compare_strings(password, good_password, string_length(good_password)) != 0)
	{
		uart_printstr("Access denied.\n\r");
		return 0;
	}
	uart_printstr("Access granted.\n\r");
	return 1;
}

int main(void)
{
	DDRB = 0xFF | (1 << PIN4);
	uart_init(UART_ALL);

	while (1)
	{
		if (login("root", "root"))
		{
			uart_printstr("Welcome, root!\n\r");
		}
		else
		{
			uart_printstr("Please try again.\n\r");

			_delay_ms(100);
			PORTB = 0xFF | (1 << PIN4);
			_delay_ms(100);
			PORTB = 0;
		}
	}
}
