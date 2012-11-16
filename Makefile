#  
#
#  Copyright (c) 2012 Bob Bownes
#
#  This is free software: you can redistribute it and/or modify
#  it under the terms of FastBob's General Public License as published by
#  Robert M. Bownes III.
#
#  This is distributed in the hope that it will be useful,
#  but with no warrantee whatsoever, for any purpose, public, private or otherwise.
#  If you use it and something happesn, it's your own darn fault.
#

#
# This is a prototype Makefile. Modify it according to your needs.
# You should at least check the settings for
# DEVICE ....... The AVR device you compile for
# CLOCK ........ Target AVR clock rate in Hertz
# OBJECTS ...... The object files created from your source files. This list is
#                usually the same as the list of source files with suffix ".o".
# PROGRAMMER ... Options to avrdude which define the hardware you use for
#                uploading to the AVR and the interface where this hardware
#                is connected.
# FUSES ........ Parameters for avrdude to flash the fuses appropriately.

DEVICE     ?= atmega128
CLOCK      = 32000
PROGRAMMER ?= -c buspirate -P /dev/tty.usbserial-AH00MQKI
OBJECTS    = example.o
# FUSES      = -U hfuse:w:0xd9:m -U lfuse:w:0x24:m
FUSES      = -U hfuse:w:0xd2:m -U lfuse:w:0xff:m

AVRDUDE = avrdude $(PROGRAMMER) -p $(DEVICE) -B 9600 -F
COMPILE = avr-gcc -Wall -Os -DF_CPU=$(CLOCK) -mmcu=$(DEVICE) -I. -ffunction-sections

# symbolic targets:
all:	example.hex

.c.o:
	$(COMPILE) -c $< -o $@

.S.o:
	$(COMPILE) -x assembler-with-cpp -c $< -o $@

# "-x assembler-with-cpp" should not be necessary since this is the default
# file type for the .S (with capital S) extension. However, upper case
# characters are not always preserved on Windows. To ensure WinAVR
# compatibility define the file type manually.

.c.s:
	$(COMPILE) -S $< -o $@

flash:	all
	$(AVRDUDE) -U flash:w:example.hex:i

fuse:
	$(AVRDUDE) $(FUSES)

# Xcode uses the Makefile targets "", "clean" and "install"

install: flash fuse

clean:
	rm -f example.hex example.elf $(OBJECTS)

# file targets:

example.elf: $(OBJECTS)
	$(COMPILE) -o example.elf $(OBJECTS) -lm -Wl,--gc-sections

example.hex: example.elf
	rm -f example.hex
	avr-objcopy -j .text -j .data -O ihex example.elf example.hex
	avr-size -C --mcu=$(DEVICE) example.elf
cpp:
	$(COMPILE) -E example.c
