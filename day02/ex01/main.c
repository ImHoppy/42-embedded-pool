#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

volatile uint8_t percentage = 1;

ISR(TIMER0_OVF_vect)
{
	static uint8_t way = 1;

	if (way)
	{
		percentage += 1;
	}
	else
	{
		percentage -= 1;
	}
	if (percentage == 0 || percentage == 100)
	{
		way ^= 1;
	}
}

int main()
{
	DDRB |= (1 << DDB1) | (1 << DDB2) | (1 << DDB0);

	TCCR0A |= (0b11 << WGM00); // Partie 1 de TOP sur OCR0A (Mode 7)

	TCCR0B |= (0b1 << WGM02);  // Partie 2 de TOP sur OCR0A (Mode 7)
	TCCR0B |= (0b101 << CS00); // Division par 1024
	// Fait une boucle 200 fois par seconde pour incrementer le % de 1
	// et fait 0->100 et 100->0 en une seconde
	OCR0A = (F_CPU / 200) / 1024;

	/// Set interrupt when timer0 overflow occured.
	TIMSK0 |= (0b1 << TOIE0);
	// Enable interrupt routine
	sei();

	// Setup timer1 pour allumer la led OC1A
	TCCR1A |= (0b10 << COM1A0); // Quand compteur == OCR1A --> LED D2 changement
	TCCR1A |= (0b10 << WGM10);	// Partie 1 de TOP sur OCR1A (Mode 14)

	TCCR1B |= (0b11 << WGM12); // Partie 2 de TOP sur OCR1A (Mode 14)
	TCCR1B |= (0b001 << CS10); // Pas de divison

	ICR1 = 32767U / 2;
	OCR1A = ICR1;
	const uint8_t ICR_1Percent = (ICR1 / 100);
	//   uint8_t poubelle = 0;
	while (1)
	{
		// OCR1A = ICR1 * poubelle / 100;
		// poubelle += 10;
		// _delay_ms(1000);
		// OCR1A = (ICR1 / percentage * 100);
		OCR1A = (ICR_1Percent * percentage);
	}
}
