#include <avr/io.h>
#include <util/twi.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <i2c.h>
#include <uart.h>
#include "rush00.h"

extern volatile game_state_t game_state;

void master_loop()
{
	static game_state_t old_game_state = IDLE;
	if (old_game_state == game_state)
		return;
	old_game_state = game_state;
	uart_printstr("CHANGE_GAME_STATUS  ");
	uart_printhex(game_state);
	uart_printstr("\n\r");
	if (game_state == NOT_READY)
	{
		uint8_t is_ready = 0;
		i2c_start(0x10, I2C_READ);
		uart_printstr("AFTER START  ");
		while (is_ready == 0)
		{
			// uart_printstr("WAITING FOR SLAVE IM_READY IN LOOP    ");
			i2c_read(ACK);
			is_ready = TWDR;

			// uart_printstr("AFTER READ  ");
			// uart_printhex(TWDR);
			// uart_printstr(" \n\r");
		}
		i2c_read(NACK);
		i2c_stop();
		_delay_ms(10);
		if (is_ready)
		{
			uart_printstr("SLAVE IM_READY IN LOOP \n\r");
			game_state = INGAME;
			i2c_start(0x10, I2C_WRITE);
			i2c_write(CHANGE_GAME_STATUS | (INGAME << 4));
			i2c_stop();
		}
	}
	else if (game_state == INGAME)
	{
		uart_printstr("GAME STARTED \n\r");
		int sec = 0;
		while (sec < 4)
		{
			i2c_start(0x10, I2C_WRITE);
			i2c_write(TIMER | (sec << 4));
			i2c_stop();
			light_led(sec);
			_delay_ms(100);
			sec += 1;
		}
		// Turn off last leds
		i2c_start(0x10, I2C_WRITE);
		i2c_write(TIMER | (4 << 4));
		i2c_stop();
		light_led(4);

		// Checking if slave pressed button
		uint8_t pressed = 0;
		i2c_start(0x10, I2C_READ);
		while (pressed != 1 && game_state == INGAME)
		{
			i2c_read(ACK);
			pressed = TWDR;
		}
		i2c_read(NACK);
		i2c_stop();
		if (pressed)
		{
			uart_printhex(pressed);
			uart_printhex(game_state);
			uart_printstr("SLAVE PRESSED BUTTON \n\r");
			game_state = LOOSE;
			i2c_start(0x10, I2C_WRITE);
			i2c_write(CHANGE_GAME_STATUS | (WIN << 4));
			i2c_stop();
			light_rgb(LOOSE);
		}
		else
		{
			i2c_start(0x10, I2C_WRITE);
			i2c_write(CHANGE_GAME_STATUS | (LOOSE << 4));
			i2c_stop();
			light_rgb(WIN);
		}
		game_state = IDLE;
	}
}
