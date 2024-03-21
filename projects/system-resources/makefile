PATH = /bin:$(DEVKITARM)/bin:../../bin:/opt/devkitpro/tools/bin

PATH_LIB_GCC = $DEVKITARM/arm-none-eabi/lib/thumb

# name of application
NAME = "System Resources"
TARGET = main

# convert graphics
GFX2GBA  = gfx2gba
B2X      = b2x

# assemble and link
PREFIX  := arm-none-eabi-
GCC      = $(PREFIX)gcc
AS       = $(PREFIX)as
LD       = $(PREFIX)ld
OBJCOPY  = $(PREFIX)objcopy

# e-reader tools
NEVPK    = nevpk
NEDCMAKE = nedcmake
NEDCENC  = nedcenc
NEDCBMP  = raw2bmp
NESAV    = neflmake

all : sav bmp

sav : $(TARGET).sav

# E-Reader Dot Code Strip

bmp : build/$(TARGET).01.bin.raw
	$(eval RAW := $(shell ls build/$(TARGET).*.raw))
	$(foreach var,$(RAW),$(NEDCBMP) -i $(var) -o "$(var)";)
	mv build/*.bmp ./

build/$(TARGET).01.bin : build/$(TARGET).vpk
	$(NEDCMAKE) -i $< -o build/"$(TARGET)" -type 2 -bin -fill 1

build/$(TARGET).01.bin.raw : build/$(TARGET).01.bin 
	$(eval BIN := $(shell ls build/$(TARGET).*.bin))
	$(foreach var,$(BIN),$(NEDCENC) -i $(var) -o $(var).raw;)


# E-Reader Save Data
$(TARGET).sav : build/$(TARGET).vpk
	$(NESAV) -i $< -o "$@" -type 2 -name $(NAME)


# GBA Binary
build/$(TARGET).vpk : build/$(TARGET).bin
	$(NEVPK) -i "$<" -o "$@" -c -level 3


build/$(TARGET).bin : build/$(TARGET).elf
	$(OBJCOPY) -O binary "$<" "$@"

build/$(TARGET).elf : build/crt0.o build/$(TARGET).o ../../lib/ereader.ld
	$(LD) build/crt0.o build/$(TARGET).o  -L $(PATH_LIB_GCC) -T ../../lib/ereader.ld -O3 -o "$@"

build/$(TARGET).o : $(TARGET).c gfx/gfx.c
	$(GCC) -I ../../lib -mthumb -c -O2 -o "$@" "$<"

build/crt0.o : ../../lib/crt0.s
	$(AS) -o "$@" "$<"


# Graphics
gfx/gfx.c :
	grit gfx/*.tga -gu8 -gB4 -ftc -fh! -pS -fa -ogfx/gfx

clean :
	rm -f build/*.o
	rm -f build/*.bin
	rm -f build/*.elf
	rm -f build/*.raw
	rm -f build/*.vpk
	rm -f gfx/*.gfx gfx/*.pal gfx/*.c gfx/*.h
	rm -f *.bmp *.sav
