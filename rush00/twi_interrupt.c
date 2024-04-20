#include <avr/io.h>
#include <util/twi.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <i2c.h>
#include <uart.h>
#include "rush00.h"

extern volatile game_state_t game_state;
extern volatile uint8_t is_slave;
extern volatile uint8_t slave_button_state;

ISR(TWI_vect)
{
	// uart_printstr("TWI_vect Status: ");
	// uart_printhex(TW_STATUS);
	// uart_printstr(" TypeData : ");
	// uart_printhex(TWDR & 0x0F);
	// uart_printstr(" Data: ");
	// uart_printhex(TWDR >> 4);
	// uart_printstr("\n\r");

	// Handle TWI interrupt
	// Check status and take appropriate action
	switch (TW_STATUS)
	{
	case TW_ST_DATA_ACK:
	case TW_ST_SLA_ACK: // SLA+R received, ACK returned
						// Read data from TWDR
	{
		TWDR = slave_button_state;
		uart_printhex(slave_button_state);
		slave_button_state = 0;
		break;
	}
	case TW_SR_DATA_ACK: // Data received, ACK returned
		// Read data from TWDR
		{
			switch (TWDR & 0x0F)
			{
			case BE_A_SLAVE:
			{
				is_slave = 1;
				break;
			}
			case CHANGE_GAME_STATUS:
			{
				game_state = (game_state_t)(TWDR >> 4);
				if (game_state == WIN || game_state == LOOSE)
				{
					is_slave = 0;
					light_rgb(game_state);
					game_state = IDLE;
				}
				if (game_state == NOT_READY)
				{
					light_rgb(game_state);
				}
				break;
			}
			case TIMER:
			{
				const uint8_t timer_led = (TWDR >> 4);
				light_led(timer_led);
			}
			case IM_READY:
			{
				if (is_slave || game_state != NOT_READY)
					break;
				game_state = INGAME;
			}
			}
			break;
		}
	case TW_SR_STOP: // Stop or repeated start condition received while selected
					 // Prepare to receive next packet
		// uart_printstr("TW_SR_STOP");
		// uart_printstr("\n\r");
		break;
		// Add more cases for other status codes as needed
	}

	// Clear interrupt flag
	TWCR |= (1 << TWINT);
}
