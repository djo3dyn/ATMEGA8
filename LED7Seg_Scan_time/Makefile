# --
# Copyright (c) 2016, Lukasz Marcin Podkalicki <lpodkalicki@gmail.com>
# --

#Microcontroller Type
MCU=atmega8

#Fuse for Internal RC Clock 8MHz
FUSE_L=0xE4
FUSE_H=0xD9
F_CPU=8000000

#Fuse for External Crystal Clock 16MHz
# FUSE_L=0xFF
# FUSE_H=0xD9
# F_CPU=16000000

#Fuse for Internal RC Clock 12MHz
# FUSE_L=0xE4
# FUSE_H=0xD9
# F_CPU=12000000

CC=avr-gcc
LD=avr-ld
OBJCOPY=avr-objcopy
SIZE=avr-size
AVRDUDE=avrdude
CFLAGS=-std=c99 -Wall -g -Os -mmcu=${MCU} -DF_CPU=${F_CPU} -I.
TARGET=bin\7seg

SRCS = 7seg.cpp ds1307.cpp twimaster.cpp

all:
	${CC} ${CFLAGS} -o ${TARGET}.o ${SRCS}
	${LD} -o ${TARGET}.elf ${TARGET}.o
	${OBJCOPY} -j .text -j .data -O ihex ${TARGET}.o ${TARGET}.hex
	${SIZE} -C --mcu=${MCU} ${TARGET}.elf

flash:
	${AVRDUDE} -p ${MCU} -c usbasp -B10 -B10 -U flash:w:${TARGET}.hex:i -F -P usb

fuse:
	$(AVRDUDE) -p ${MCU} -c usbasp -B10 -U hfuse:w:${FUSE_H}:m -U lfuse:w:${FUSE_L}:m

clean:
	rm -f *.c~ *.h~ *.o *.elf *.hex