#include <avr/io.h>
#include <util/twi.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <i2c.h>
#include <uart.h>
#include "rush00.h"

volatile game_state_t game_state = IDLE;
volatile uint8_t is_slave = 0;
volatile uint8_t slave_button_state = 0;

void light_led(uint8_t index)
{
	const uint8_t led_index[4] = {PB0, PB1, PB2, PB4};
	PORTB = 0;
	if (index > 3)
		return;
	PORTB ^= (1 << led_index[index]);
}

void light_rgb(game_state_t status)
{

	if (status == WIN)
	{
		PORTD = (1 << PD3);
	}
	else if (status == LOOSE)
	{
		PORTD = (1 << PD5);
	}
}

ISR(PCINT2_vect)
{
	static uint8_t sw_pressed = 0;
	uart_printstr("SW2  ");
	uart_printhex(sw_pressed);
	uart_printstr(" | ");

	uart_printhex(is_slave);
	uart_printstr(" | ");
	uart_printhex(game_state);
	uart_printstr("\n\r");
	if (!sw_pressed)
	{

		sw_pressed = 1;
		if (is_slave)
		{
			slave_button_state = 1;
		}
		else
		{
			// Only on master OR if not in game

			// NOBODY IS SLAVE
			if (game_state == IDLE)
			{
				// Ask to be a slave
				i2c_start(0x10, I2C_WRITE);
				i2c_write(BE_A_SLAVE);
				i2c_stop();
				// Wait for slave to be ready
				i2c_start(0x10, I2C_WRITE);
				i2c_write((NOT_READY << 4) | CHANGE_GAME_STATUS);
				i2c_stop();
				game_state = NOT_READY;
			}
			else if (game_state == INGAME)
			{
				game_state = WIN;
			}
		}
	}
	else
	{
		sw_pressed = 0;
	}
}

int main()
{
	DDRB = (1 << PINB0) | (1 << PINB1) | (1 << PINB2) | (1 << PINB4);
	DDRD = (1 << PD3) | (1 << PD5);

	uart_init(UART_TX);
	i2c_init();
	i2c_init_slave(0x10);

	PCMSK2 |= (0b1 << PCINT20); // Set PCINT20 (SW2) to be interrupted
	PCICR |= (0b1 << PCIE2);	// Enable interrupt for PCINT[23:16] to PCINT2_vect

	sei();

	_delay_ms(200);
	while (1)
	{
		if (is_slave)
		{
			slave_loop();
		}
		else
		{
			master_loop();
		}
	}
}
