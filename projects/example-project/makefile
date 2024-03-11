PATH = /bin:$(DEVKITARM)/bin:../../bin:/opt/devkitpro/tools/bin

PATH_LIB_GCC = $DEVKITARM/arm-none-eabi/lib/thumb

# name of application
NAME = "sprite demo"
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

bmp : build/$(TARGET).vpk
	$(NEDCMAKE) -i $< -o build/"$(TARGET)" -type 2 -bin -fill 1
	$(NEDCENC) -i build/$(TARGET).*.bin -o build/"$(TARGET).raw"
	$(NEDCBMP) -i build/"$(TARGET).raw" -o "$(TARGET)"

$(TARGET).sav : build/$(TARGET).vpk
	$(NESAV) -i $< -o "$@" -type 2 -name $(NAME)

# E-Reader Save Data

build/$(TARGET).vpk : build/$(TARGET).bin
	$(NEVPK) -i "$<" -o "$@" -c -level 0

# GBA Binary

build/$(TARGET).bin : build/$(TARGET).elf
	$(OBJCOPY) -O binary "$<" "$@"

build/$(TARGET).elf : build/crt0.o build/$(TARGET).o ../../lib/ereader.ld
	$(LD) build/crt0.o build/$(TARGET).o  -L $(PATH_LIB_GCC) -T ../../lib/ereader.ld -O3 -o "$@"

build/$(TARGET).o : $(TARGET).c gfx/gfx.c 
	$(GCC) -I ../../lib -mthumb -c -O3 -o "$@" "$<"

build/crt0.o : ../../lib/crt0.s
	$(AS) -o "$@" "$<"


# Graphics
gfx/gfx.c :
	gfx/convert.sh

clean :
	rm -f build/*.o
	rm -f build/*.bin
	rm -f build/*.elf
	rm -f build/*.raw
	rm -f build/*.vpk
	rm -f gfx/*.gfx gfx/*.pal gfx/*.c  gfx/*.h
	rm -f *.bmp *.sav
