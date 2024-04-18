#pragma once

#define UART_BAUDRATE 115200
#define BAUD ((F_CPU / 16.0 / UART_BAUDRATE) + 0.5) - 1

void uart_init(void);

void uart_tx(char c);
void uart_printstr(const char *str);
