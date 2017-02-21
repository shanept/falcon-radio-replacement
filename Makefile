############################################################################
# TARGET:        Target build directory
# INSTALL_DIR:   Arduino IDE installation directory
# PORT:          Arduino board port
#
# Retrieve the following data from:
#   $(INSTALL_DIR)/hardware/arduino/avr/boards.txt
#
# UPLOAD_SPEED, UPLOAD_PROTOCOL, BUILD_MCU, BUILD_F_CPU, VARIANT
############################################################################
TARGET = $(notdir $(CURDIR))
# INSTALL_DIR = /Applications/Arduino.app/Contents/Resources/Java
INSTALL_DIR = /opt/arduino-1.6.9
PORT = /dev/ttyACM1

UPLOAD_SPEED = 115200
UPLOAD_PROTOCOL = arduino
BUILD_MCU = atmega328p
BUILD_F_CPU = 16000000L
VARIANT = standard

############################################################################
# Below here nothing should be changed...
############################################################################
AVR = $(INSTALL_DIR)/hardware/arduino/avr
ARDUINO = $(AVR)/cores/arduino
AVR_TOOLS_PATH = $(INSTALL_DIR)/hardware/tools/avr/bin
SRC = $(ARDUINO)/hooks.c $(ARDUINO)/wiring.c $(ARDUINO)/wiring_analog.c \
    $(ARDUINO)/wiring_digital.c $(ARDUINO)/wiring_pulse.c \
    $(ARDUINO)/wiring_shift.c $(ARDUINO)/WInterrupts.c
CXXSRC = $(ARDUINO)/HardwareSerial.cpp $(ARDUINO)/HardwareSerial0.cpp \
    $(ARDUINO)/WMath.cpp $(ARDUINO)/Print.cpp $(ARDUINO)/abi.cpp \
    $(ARDUINO)/new.cpp $(wildcard IO/*.cpp) $(wildcard Models/*.cpp) \
    $(wildcard Controls/SN74HC165N/*.cpp) \
    $(wildcard $(INSTALL_DIR)/libraries/CAN_BUS_Shield/*.cpp) \
    $(wildcard $(AVR)/libraries/SPI/src/*.cpp)

FORMAT = ihex

# Name of this Makefile (used for "make depend").
MAKEFILE = Makefile

# Debugging format.
# Native formats for AVR-GCC's -g are stabs [default], or dwarf-2.
# AVR (extended) COFF requires stabs, plus an avr-objcopy run.
DEBUG = stabs

OPT = s

# Place -D or -U options here
CDEFS = -DF_CPU=$(BUILD_F_CPU)
CXXDEFS = -DF_CPU=$(BUILD_F_CPU)

# Place -I options here
CINCS = -I$(ARDUINO) -I$(AVR)/variants/$(VARIANT) -I$(abspath Models) \
    -I$(abspath IO) -I$(INSTALL_DIR)/libraries/CAN_BUS_Shield \
    -I$(AVR)/libraries/SPI/src
#    $(addprefix -I, $(addsuffix /src/, $(wildcard $(AVR)/libraries/*)))

CXXINCS = $(CINCS)

# Compiler flag to set the C Standard level.
# c89 - "ANSI" C
# gnu89 - c89 plus GCC extensions
# c99 - ISO C99 standard (not yet fully implemented)
# gnu99 - c99 plus GCC extensions
CSTANDARD = -std=gnu++11
CXXSTANDARD = -std=gnu++11
CDEBUG = -g$(DEBUG)
CWARN = -Wall -Wstrict-prototypes
CTUNING = -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
#CEXTRA = -Wa,-adhlns=$(<:.c=.lst)

CFLAGS = $(CDEBUG) $(CDEFS) $(CINCS) -O$(OPT) $(CWARN) $(CSTANDARD) $(CEXTRA)
CXXFLAGS = $(CXXDEFS) $(CINCS) -O$(OPT) $(CXXSTANDARD)
#ASFLAGS = -Wa,-adhlns=$(<:.S=.lst),-gstabs
LDFLAGS = -lm

# Programming support using avrdude. Settings and variables.
AVRDUDE_PORT = $(PORT)
AVRDUDE_WRITE_FLASH = -U flash:w:applet/$(TARGET).hex
AVRDUDE_FLAGS = -V -F \
    -p $(BUILD_MCU) -P $(AVRDUDE_PORT) -c $(UPLOAD_PROTOCOL) \
    -b $(UPLOAD_SPEED) -C $(INSTALL_DIR)/hardware/tools/avr/etc/avrdude.conf

# Program settings
CC = $(AVR_TOOLS_PATH)/avr-gcc
CXX = $(AVR_TOOLS_PATH)/avr-g++
OBJCOPY = $(AVR_TOOLS_PATH)/avr-objcopy
OBJDUMP = $(AVR_TOOLS_PATH)/avr-objdump
AR = $(AVR_TOOLS_PATH)/avr-ar
SIZE = $(AVR_TOOLS_PATH)/avr-size
NM = $(AVR_TOOLS_PATH)/avr-nm
AVRDUDE = $(AVR_TOOLS_PATH)/avrdude
REMOVE = rm -f
MV = mv -f

# Define all object files.
OBJ = $(SRC:.c=.o) $(CXXSRC:.cpp=.o) $(ASRC:.S=.o)

# Define all listing files.
LST = $(ASRC:.S=.lst) $(CXXSRC:.cpp=.lst) $(SRC:.c=.lst)

# Combine all necessary flags and optional flags.
# Add target processor to flags.
ALL_CFLAGS = -mmcu=$(BUILD_MCU) -I. $(CFLAGS)
ALL_CXXFLAGS = -mmcu=$(BUILD_MCU) -I. $(CXXFLAGS)
ALL_ASFLAGS = -mmcu=$(BUILD_MCU) -I. -x assembler-with-cpp $(ASFLAGS)

# Default target.
all: applet_files build sizeafter

build: elf hex

applet_files: $(TARGET).ino
	test -d applet || mkdir applet
	echo '#include "Arduino.h"' > applet/$(TARGET).cpp
	cat $(TARGET).ino >> applet/$(TARGET).cpp
	cat $(ARDUINO)/main.cpp >> applet/$(TARGET).cpp

elf: applet/$(TARGET).elf
hex: applet/$(TARGET).hex
eep: applet/$(TARGET).eep
lss: applet/$(TARGET).lss
sym: applet/$(TARGET).sym

# Program the device.
upload: applet/$(TARGET).hex
	$(AVRDUDE) $(AVRDUDE_FLAGS) $(AVRDUDE_WRITE_FLASH)

# Display size of file.
HEXSIZE = $(SIZE) --target=$(FORMAT) applet/$(TARGET).hex
ELFSIZE = $(SIZE) applet/$(TARGET).elf
sizebefore:
	@if [ -f applet/$(TARGET).elf ]; then echo; echo $(MSG_SIZE_BEFORE); $(HEXSIZE); echo; fi

sizeafter:
	@if [ -f applet/$(TARGET).elf ]; then echo; echo $(MSG_SIZE_AFTER); $(HEXSIZE); echo; fi

# Convert ELF to COFF for use in debugging / simulating in AVR Studio or VMLAB.
COFFCONVERT=$(OBJCOPY) --debugging \
    --change-section-address .data-0x800000 \
    --change-section-address .bss-0x800000 \
    --change-section-address .noinit-0x800000 \
    --change-section-address .eeprom-0x810000

coff: applet/$(TARGET).elf
	$(COFFCONVERT) -O coff-avr applet/$(TARGET).elf $(TARGET).cof

extcoff: $(TARGET).elf
	$(COFFCONVERT) -O coff-ext-avr applet/$(TARGET).elf $(TARGET).cof

.SUFFIXES: .elf .hex .eep .lss .sym

.elf.hex:
	$(OBJCOPY) -O $(FORMAT) -R .eeprom $< $@

.elf.eep:
	-$(OBJCOPY) -j .eeprom --set-section-flags=.eeprom="alloc,load" \
    --change-section-lma .eeprom=0 -O $(FORMAT) $< $@

# Create extended listing file from ELF output file.
.elf.lss:
	$(OBJDUMP) -h -S $< > $@

# Create a symbol table from ELF output file.
.elf.sym:
	$(NM) -n $< > $@

# Link: create ELF output file from library.
applet/$(TARGET).elf: applet/$(TARGET).cpp applet/core.a
	$(CC) $(ALL_CFLAGS) -o $@ applet/$(TARGET).cpp -L. applet/core.a $(LDFLAGS)

applet/core.a: $(OBJ)
	@for i in $(OBJ); do echo $(AR) rcs applet/core.a $$i; $(AR) rcs applet/core.a $$i; done

# Compile: create object files from C++ source files.
%.o: %.cpp
	$(CXX) -c $(ALL_CXXFLAGS) $< -o $@

# Compile: create object files from C source files.
%.o: %.c
	$(CC) -c $(ALL_CFLAGS) $< -o $@

# Compile: create assembler files from C source files.
.c.s:
	$(CC) -S $(ALL_CFLAGS) $< -o $@

# Assemble: create object files from assembler source files.
.S.o:
	$(CC) -c $(ALL_ASFLAGS) $< -o $@

# Automatic dependencies
%.d: %.c
	$(CC) -M $(ALL_CFLAGS) $< | sed "s;$(notdir $*).o:;$*.o $*.d:;" > $@

%.d: %.cpp
	$(CXX) -M $(ALL_CXXFLAGS) $< | sed "s;$(notdir $*).o:;$*.o $*.d:;" > $@

# Target: clean project.
clean:
	@$(REMOVE) applet/$(TARGET).cpp applet/$(TARGET).hex applet/$(TARGET).eep applet/$(TARGET).cof \
	    applet/$(TARGET).elf applet/$(TARGET).map applet/$(TARGET).sym applet/$(TARGET).lss \
	    applet/core.a $(TARGET).o $(TARGET).d \
	    $(OBJ) $(LST) $(SRC:.c=.s) $(SRC:.c=.d) $(CXXSRC:.cpp=.s) $(CXXSRC:.cpp=.d) \
	@$(REMOVE) -r applet
	@echo "All clean!"

.PHONY: all build elf hex eep lss sym program coff extcoff clean applet_files sizebefore sizeafter
