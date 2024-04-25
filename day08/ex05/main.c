#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <spi.h>
#include <adc.h>

#define RED_LED (1 << PD5)
#define GREEN_LED (1 << PD6)
#define BLUE_LED (1 << PD3)

static uint8_t led_index;
static int8_t rgb_color_index = 0;

volatile uint8_t sw1_state = 0;

/* SW1 button will change primary color index */
ISR(INT0_vect)
{
	sw1_state = 1;
}

/* SW2 button will change LED */
ISR(PCINT2_vect)
{
	static uint8_t is_pressed = 0;

	is_pressed = !is_pressed;
	if (!is_pressed)
	{
		led_index = (led_index + 1) % 3;
#ifdef DEBUG
		PORTB &= ~((1 << PIN2) | (1 << PIN1) | (1 << PIN0));
		PORTB |= (led_index == 0) ? (1 << PIN2) : 0;
		PORTB |= (led_index == 1) ? (1 << PIN1) : 0;
		PORTB |= (led_index == 2) ? (1 << PIN0) : 0;
#endif
	}
	// Neutralizing bounce effect
	_delay_ms(20);
	// Clear interrupt flag if one occured while ISR was executing
	PCIFR |= (1 << PCIF2);
}

#define ColorToHex(color) (((uint32_t)(color[0]) << 16) | ((uint32_t)(color[1]) << 8) | (color[2]))

int main()
{
	spi_init(SPI_MASTER);
	adc_init(ADC_NORMAL);

	EICRA |= (0b00 << ISC00); // Low level --> interrupt request for INT0
	EIMSK |= (0b1 << INT0);	  // Set INTO (SW1) to be interrupted

	PCMSK2 |= (0b1 << PCINT20); // Set PCINT20 (SW2) to be interrupted
	PCICR |= (0b1 << PCIE2);	// Enable interrupt for PCINT[23:16] to PCINT2_vect
	sei();

#ifdef DEBUG
	// Enable output for LED D5
	DDRD |= RED_LED | GREEN_LED | BLUE_LED;
	DDRB |= (1 << PIN2) | (1 << PIN1) | (1 << PIN0);
#endif

	uint8_t colors[3][3] = {0};

	while (1)
	{
		if (sw1_state)
		{
			rgb_color_index = (rgb_color_index + 1) % 3;
			_delay_ms(20);
			sw1_state = 0;
#ifdef DEBUG
			PORTD &= ~(RED_LED | GREEN_LED | BLUE_LED);
			PORTD |= (rgb_color_index == 0) ? RED_LED : 0;
			PORTD |= (rgb_color_index == 1) ? GREEN_LED : 0;
			PORTD |= (rgb_color_index == 2) ? BLUE_LED : 0;
#endif
		}

		uint8_t adc = adc_read8(0);

		colors[led_index][rgb_color_index] = adc;

		uint32_t led0 = ColorToHex(colors[0]) | LOW_BRIGHTNESS;
		uint32_t led1 = ColorToHex(colors[1]) | LOW_BRIGHTNESS;
		uint32_t led2 = ColorToHex(colors[2]) | LOW_BRIGHTNESS;

		spi_set_leds(led0, led1, led2);
		_delay_ms(10);
	}

	spi_end();
}
