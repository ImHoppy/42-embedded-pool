BIN = target/main.bin
SRCS = ./main.c

CC = avr-gcc
OBJCPY = avr-objcopy
CFLAGS = -mmcu=atmega328p -D F_CPU=16000000 -Wall -Wextra -Werror -Os -I../lib -MMD -MP

PORT = /dev/ttyUSB0
BAUD_RATE = 115200

OBJS = ${SRCS:%.c=%.o}
DEPS = ${OBJS:.o=.d}

all: hex flash

hex: ${BIN}.hex

%.o: %.c
	${CC} ${CFLAGS} -o $@ -c $<

${BIN}: ${OBJS}
	mkdir -p $(@D)
	${CC} ${CFLAGS} -o $@ $^

${BIN}.hex: ${BIN}
	${OBJCPY} -O ihex $< ${BIN}.hex

flash: ${BIN}.hex
	avrdude -c arduino -p m328p -P ${PORT} -b ${BAUD_RATE} -U flash:w:${BIN}.hex:i

clean:
	rm -rf ./target $(OBJS) $(DEPS)


screen:
	screen ${PORT} ${BAUD_RATE}

-include $(DEPS)

.PHONY: all hex flash clean screen
