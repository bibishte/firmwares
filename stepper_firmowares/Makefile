##### AVR Makefile
##### Supports C, C++ and Assembly source files.

##### make - compile project
##### make disasm - disassembly
##### make stats - status
##### make hex - create hex output
##### make writeflash - write to flash (only with connected programmer)
##### make gdbinit - create gdbinit file
##### make simulavr - start simulavr
##### make ddd - start ddd for debugging
##### or make clean - clean target
#####
##### See the http://electrons.psychogenic.com/ 
##### website for detailed instructions


# Name of target controller 
# (e.g. 'at90s8515', see the available avr-gcc mmcu 
# options for possible values)
MCU=atmega8 #atmega162

# Processor frequency.
#     This will define a symbol, F_CPU, in all source code files equal to the 
#     processor frequency. You can then use this symbol in your source code to 
#     calculate timings. Do NOT tack on a 'UL' at the end, this will be done
#     automatically to create a 32-bit value in your source code.
F_CPU = 11059200

# Name of our project
# (use a single word, e.g. 'myproject')
PROJECTNAME=stepper

# Source files
# List C/C++/Assembly source files:
# (list all files to compile, eg 'a.c b.cpp as.S')
# Use .cc, .cpp or .C suffix for C++ files, use .S 
# (NOT .s !!!) for assembly source code files.
PRJSRC=main.c usart.c interpreter.c drv_8825.c utils.c messages.c

# additional includes (e.g. -I/path/to/mydir)
INC=-I./

# libraries to link in (e.g. -lmylib)
LIBS= -B/usr/avr/lib/ -B/usr/avr/lib/avr4/ -B/usr/i686-pc-linux-gnu/avr/lib -B/usr/avr/lib/avr5/ 

# Optimization level, 
# use s (size opt), 1, 2, 3 or 0 (off)
OPTLEVEL=2

# Place -D or -U options here
CDEFS = -DF_CPU=$(F_CPU)UL 

#####      AVR Dude 'writeflash' options       #####
#####  If you are using the avrdude program
#####  (http://www.bsdhome.com/avrdude/) to write
#####  to the MCU, you can set the following config
#####  options and use 'make writeflash' to program
#####  the device.


# programmer id--check the avrdude for complete list
# of available opts.  These should include stk500,
# avr910, avrisp, bsd, pony and more.  Set this to
# one of the valid "-c PROGRAMMER-ID" values 
# described in the avrdude info page.
# 
#AVRDUDE_PROGRAMMERID=siprog
AVRDUDE_PROGRAMMERID=usbasp
# port--serial or parallel port to which your 
# hardware programmer is attached
#
AVRDUDE_PORT=/dev/ttyS0

# Fuse atmega8 high byte HFUSE:
# 0xc9 = 1 1 0 0   1 0 0 1 <-- BOOTRST (boot reset vector at 0x0000)
#        ^ ^ ^ ^   ^ ^ ^------ BOOTSZ0
#        | | | |   | +-------- BOOTSZ1
#        | | | |   + --------- EESAVE (don't preserve EEPROM over chip erase)
#        | | | +-------------- CKOPT (full output swing)
#        | | +---------------- SPIEN (allow serial programming)
#        | +------------------ WDTON (WDT not always on)
#        +-------------------- RSTDISBL (reset pin is enabled)
# Fuse atmega8 low byte LFUSE:
# 0x9f = 1 0 0 1   1 1 1 1
#        ^ ^ \ /   \--+--/
#        | |  |       +------- CKSEL 3..0 (external >8M crystal)
#        | |  +--------------- SUT 1..0 (crystal osc, BOD enabled)
#        | +------------------ BODEN (BrownOut Detector enabled)
#        +-------------------- BODLEVEL (2.7V)
# Lock atmega8 LOCK:
# 0x3f = 1 1   1 1 1 1 <------ LB1		Lock bit
#        ^ ^   ^ ^ ^---------- LB2		Lock bit
#        | |   | +------------ BLB01	Boot lock bit
#        | |   +-------------- BLB02 	Boot lock bit
#        | +------------------ BLB11	Boot lock bit
#        +-------------------- BLB12	Boot lock bit
HFUSE=0xD9
LFUSE=0xCE
LOCK=0x3E

####################################################
#####                Config Done               #####
#####                                          #####
##### You shouldn't need to edit anything      #####
##### below to use the makefile but may wish   #####
##### to override a few of the flags           #####
##### nonetheless                              #####
#####                                          #####
####################################################


##### Flags ####

# HEXFORMAT -- format for .hex file output
HEXFORMAT=ihex

# compiler
CFLAGS=-I. $(INC) -mmcu=$(MCU) -std=c99 -O$(OPTLEVEL) \
	-fpack-struct -fshort-enums            \
	-funsigned-bitfields -funsigned-char    \
	-Wall -Wstrict-prototypes               \
	-Wa,-ahlms=$(firstword                  \
	$(filter %.lst, $(<:.c=.lst)))

CFLAGS += $(CDEFS)

# c++ specific flags
CPPFLAGS=-fno-exceptions               \
	-Wa,-ahlms=$(firstword         \
	$(filter %.lst, $(<:.cpp=.lst))\
	$(filter %.lst, $(<:.cc=.lst)) \
	$(filter %.lst, $(<:.C=.lst)))

# assembler
ASMFLAGS =-I. $(INC) -mmcu=$(MCU)        \
	-x assembler-with-cpp            \
	-Wa,-gstabs,-ahlms=$(firstword   \
		$(<:.S=.lst) $(<.s=.lst))


# linker
LDFLAGS=-Wl,-Map,$(TRG).map -lm $(LIBS) -mmcu=$(MCU)

##### executables ####
CC=avr-gcc
OBJCOPY=avr-objcopy
OBJDUMP=avr-objdump
SIZE=avr-size
AVRDUDE=avrdude
REMOVE=rm -f

##### automatic target names ####
TRG=$(PROJECTNAME).out
DUMPTRG=$(PROJECTNAME).s

HEXROMTRG=$(PROJECTNAME).hex 
HEXTRG=$(HEXROMTRG) $(PROJECTNAME).ee.hex
GDBINITFILE=gdbinit-$(PROJECTNAME)

# Define all object files.

# Start by splitting source files by type
#  C++
CPPFILES=$(filter %.cpp, $(PRJSRC))
CCFILES=$(filter %.cc, $(PRJSRC))
BIGCFILES=$(filter %.C, $(PRJSRC))
#  C
CFILES=$(filter %.c, $(PRJSRC))
#  Assembly
ASMFILES=$(filter %.S, $(PRJSRC))


# List all object files we need to create
OBJDEPS=$(CFILES:.c=.o)    \
	$(CPPFILES:.cpp=.o)\
	$(BIGCFILES:.C=.o) \
	$(CCFILES:.cc=.o)  \
	$(ASMFILES:.S=.o)

# Define all lst files.
LST=$(filter %.lst, $(OBJDEPS:.o=.lst))

# All the possible generated assembly 
# files (.s files)
GENASMFILES=$(filter %.s, $(OBJDEPS:.o=.s)) 


.SUFFIXES : .c .cc .cpp .C .o .out .s .S \
	.hex .ee.hex .h .hh .hpp


.PHONY: writeflash clean stats gdbinit stats

# Make targets:
# all, disasm, stats, hex, writeflash/install, clean
all: $(TRG) hex

disasm: $(DUMPTRG) stats

stats: $(TRG)
	$(OBJDUMP) -h $(TRG)
	$(SIZE) $(TRG) 

hex: $(HEXTRG)


writeflash: hex
	@echo "====="
	@echo "===== writing hex to device flash ====="
	@echo "====="
	$(AVRDUDE) -c $(AVRDUDE_PROGRAMMERID)   \
	 -p $(MCU) -P $(AVRDUDE_PORT) -e        \
	 -U flash:w:$(HEXROMTRG)

setfuses:
	@echo "====="
	@echo "===== setting device fuses ====="
	@echo "====="
	$(AVRDUDE) -c $(AVRDUDE_PROGRAMMERID)   \
	 -p $(MCU) -P $(AVRDUDE_PORT) -u        \
	 -U hfuse:w:$(HFUSE):m -U lfuse:w:$(LFUSE):m

setlocks:
	@echo "====="
	@echo "===== setting device locks ====="
	@echo "====="
	$(AVRDUDE) -c $(AVRDUDE_PROGRAMMERID)   \
	 -p $(MCU) -P $(AVRDUDE_PORT)        \
	 -U lock:w:$(LOCK):m
	 
avrdude:
	$(AVRDUDE) -c $(AVRDUDE_PROGRAMMERID)   \
	 -p $(MCU) -P $(AVRDUDE_PORT) -e        \
	 -v

install: writeflash setfuses

$(DUMPTRG): $(TRG) 
	$(OBJDUMP) -S  $< > $@


$(TRG): $(OBJDEPS) 
	$(CC) $(LDFLAGS) -o $(TRG) $(OBJDEPS)


#### Generating assembly ####
# asm from C
%.s: %.c
	$(CC) -S $(CFLAGS) $< -o $@

# asm from (hand coded) asm
%.s: %.S
	$(CC) -S $(ASMFLAGS) $< > $@


# asm from C++
.cpp.s .cc.s .C.s :
	$(CC) -S $(CFLAGS) $(CPPFLAGS) $< -o $@



#### Generating object files ####
# object from C
.c.o: 
	$(CC) $(CFLAGS) -c $<


# object from C++ (.cc, .cpp, .C files)
.cc.o .cpp.o .C.o :
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $<

# object from asm
.S.o :
	$(CC) $(ASMFLAGS) -c $< -o $@


#### Generating hex files ####
# hex files from elf
#####  Generating a gdb initialisation file    #####
.out.hex:
	$(OBJCOPY) -j .text                    \
		-j .data                       \
		-O $(HEXFORMAT) $< $@

.out.ee.hex:
	$(OBJCOPY) -j .eeprom                  \
		--change-section-lma .eeprom=0 \
		-O $(HEXFORMAT) $< $@


#####  Generating a gdb initialisation file    #####
##### Use by launching simulavr and avr-gdb:   #####
#####   avr-gdb -x gdbinit-myproject           #####
# debugging-related targets

ddd: gdbinit
	ddd --debugger "avr-gdb -x $(GDBINITFILE)"

gdbserver: gdbinit
	simulavr --gdbserver --device $(MCU)

gdbinit: $(GDBINITFILE)

$(GDBINITFILE): $(TRG)
	@echo "file $(TRG)" > $(GDBINITFILE)
	
	@echo "target remote localhost:1212" \
		                >> $(GDBINITFILE)
	
	@echo "load"        >> $(GDBINITFILE) 
	@echo "break main"  >> $(GDBINITFILE)
	@echo "continue"    >> $(GDBINITFILE)
	@echo
	@echo "Use 'avr-gdb -x $(GDBINITFILE)'"


#### Cleanup ####
clean:
	$(REMOVE) $(TRG) $(TRG).map $(DUMPTRG)
	$(REMOVE) $(OBJDEPS)
	$(REMOVE) $(LST) $(GDBINITFILE)
	$(REMOVE) $(GENASMFILES)
	$(REMOVE) $(HEXTRG)
	$(REMOVE) *.map
	


#####                    EOF                   #####
