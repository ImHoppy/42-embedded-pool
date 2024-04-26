#pragma once
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

void timer1_init(uint16_t ms);
void timer1_OVF();
