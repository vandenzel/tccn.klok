PROJECT = klok
MCU = at90usb162
CC = avr-gcc
OBJECTS = klok.o systick.o usbserial.o
LINKONLYOBJECTS = 

## Options common to compile, link and assembly rules
COMMON = -mmcu=$(MCU) -I$(HOME)/Projects/lib/LUFA

## Compile options common for all C compilation units.
CFLAGS = $(COMMON)
CFLAGS += -Wall -gdwarf-2 -Os
CFLAGS += -MD -MP -MT $(*F).o -MF dep/$(@F).d 

## Assembly specific flags
ASMFLAGS = $(COMMON)
ASMFLAGS += $(CFLAGS)
ASMFLAGS += -x assembler-with-cpp -Wa,-gdwarf2

## Linker flags
LDFLAGS = $(COMMON)
LDFLAGS += 


## Intel Hex file production flags
HEX_FLASH_FLAGS = -R .eeprom

HEX_EEPROM_FLAGS = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0 --no-change-warnings


## Build
all: $(PROJECT).elf $(PROJECT).hex $(PROJECT).eep size

## Compile
%.o: %.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

##Link
$(PROJECT).elf: $(OBJECTS)
	 $(CC) $(LDFLAGS) $(OBJECTS) $(LINKONLYOBJECTS) $(LIBDIRS) $(LIBS) -o $(PROJECT).elf

%.hex: %.elf
	avr-objcopy -O ihex $(HEX_FLASH_FLAGS)  $< $@

%.eep: %.elf
	-avr-objcopy $(HEX_EEPROM_FLAGS) -O ihex $< $@ || exit 0

%.lss: %.elf
	avr-objdump -h -S $< > $@

size: $(PROJECT).elf
	@echo
	@avr-size -C --mcu=$(MCU) $(PROJECT).elf

## Clean target
.PHONY: clean
clean:
	-rm -rf $(OBJECTS) $(PROJECT).elf dep/* $(PROJECT).hex $(PROJECT).eep

## Other dependencies
-include $(shell mkdir dep 2>/dev/null) $(wildcard dep/*)

## Flash

flash:
	dfu-programmer $(MCU) erase
	dfu-programmer $(MCU) flash $(PROJECT).hex

