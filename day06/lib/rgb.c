#include <avr/io.h>
#include "rgb.h"
#define RED_LED (1 << PD5)
#define GREEN_LED (1 << PD6)
#define BLUE_LED (1 << PD3)

void init_rgb()
{
	// Set R G B as output
	DDRD |= RED_LED | GREEN_LED | BLUE_LED;
	// Initialisation of Timer 0 for LED RED & GREEN
	TCNT0 = 0;
	OCR0A = 0;									  // Reset Compare Match OCR0A value to 0; LED GREEN
	OCR0B = 0;									  // Reset Compare Match OCR0B value to 0; LED RED
	TCCR0A = (0b10 << COM0A0) | (0b10 << COM0B0); // Set Compare Output on "Clear OC0X on Compare Match, set OC0X at BOTTOM"
	TCCR0A |= (0b11 << WGM00);					  // Set TC0 mode to Fast PWM, TOP = OCRA (Mode 7)
	TCCR0B = (0b001 << CS00);					  // Set up TC0 to no pre-scaling

	// Initialisation of Timer 2 for LED BLUE
	TCNT2 = 0;
	OCR2B = 0;				   // Reset Compare Match OCR2B value to 0
	TCCR2A = (0b10 << COM2B0); // Clear OC2A on Compare Match, set OC2A at BOTTOM
	TCCR2A |= (0b11 << WGM20); // Set TC2 mode to Fast PWM 0xFF (WGM22:0 = 3)
	TCCR2B = (0b001 << CS20);  // Set up TC2 to no pre-scaling
}
void set_rgb(uint8_t r, uint8_t g, uint8_t b)
{
	OCR0A = g;
	OCR0B = r;
	OCR2B = b;
}
