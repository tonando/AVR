DEVICE  = atmega328p
F_CPU   = 20000000	
FUSE_L  = 0xff 
FUSE_H  = 0xdd
TOOL 	= stk500v2
PORT	= COM8

OBJCOPY = avr-objcopy
OUT     = main
SRC     = main.cpp

OBJFLAGS = -j .text -j .data -O ihex
AVRDUDE = avrdude -c $(TOOL) -P $(PORT) -p $(DEVICE) 
COMPILE = avr-gcc -Wall -Os -DF_CPU=$(F_CPU) -mmcu=$(DEVICE) 
#-Iusbdrv
#OBJECTS = $(SRC:.c=.o) $(CHSRC:.ch=.o)
OBJECTS = main.o Sequencer.o Arpeggiator.o 
#$(OBJECTS): usbdrv/usbconfig.h

hex: $(OUT).hex

# rule for uploading firmware:
flash: $(OUT).hex
	$(AVRDUDE) -U flash:w:$(OUT).hex:i

# rule for deleting dependent files (those which can be built by Make):
clean:
	rm -f *.hex *.lst *.obj *.cof *.list *.map *.eep.hex *.elf *.o

# Generic rule for compiling C files:
%.o: %.cpp	
	$(COMPILE) -c $< -o $@

# Generic rule for compiling combined C and header files:
.ch.o:
	$(COMPILE) $(CHFLAGS) -c $< -o $@

# Generic rule for assembling Assembler source files:
%.o: %.S
	$(COMPILE) -x assembler-with-cpp -c $< -o $@
# "-x assembler-with-cpp" should not be necessary since this is the default
# file type for the .S (with capital S) extension. However, upper case
# characters are not always preserved on Windows. To ensure WinAVR
# compatibility define the file type manually.

# Generic rule for compiling C to assembler, used for debugging only.
#.c.s:
#	$(COMPILE) -S $< -o $@

# file targets:
$(OUT).elf: $(OBJECTS)
	$(COMPILE) $(OBJECTS) -o $@
%.hex: %.elf
	rm -f *.hex *.eep.hex
	$(OBJCOPY) $(OBJFLAGS) $< $@
	#avr-objcopy -j .text -j .data -O ihex $(OUT).elf $(OUT).hex
	avr-size $(OUT).hex

# debugging targets:

disasm:	$(OUT).elf
	avr-objdump -d $(OUT).elf

cpp:
	$(COMPILE) -E $(SRC)
