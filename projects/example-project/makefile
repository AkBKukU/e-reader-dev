PATH = /bin:$(DEVKITARM)/bin:../../bin:/opt/devkitpro/tools/bin

PATH_LIB_GCC = $DEVKITARM/arm-none-eabi/lib/thumb

# name of application
NAME = "mario sprite"

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
NESAV    = wine ../../bin/neflmake.exe
NEDCMAKE = nedcmake
NEDCENC  = nedcenc
NEDCBMP  = raw2bmp

all : sav bmp

sav : main.sav

# E-Reader Dot Code Strip

bmp : main.vpk
	$(NEDCMAKE) -i $< -o "main" -type 2 -bin -fill 1
	$(NEDCENC) -i main.*.bin -o "main.raw"
	$(NEDCBMP) -i "main.raw" -o "main"

main.sav : main.vpk
	$(NESAV) -i $< -o "$@" -type 2 -name $(NAME)

# E-Reader Save Data

main.vpk : main.bin
	$(NEVPK) -i "$<" -o "$@" -c -level 0

# GBA Binary

main.bin : main.elf
	$(OBJCOPY) -O binary "$<" "$@"

main.elf : crt0.o main.o ../../lib/ereader.ld
	$(LD) crt0.o main.o  -L $(PATH_LIB_GCC) -T ../../lib/ereader.ld -O3 -o "$@"

main.o : main.c gfx $(wildcard *.c)
	$(GCC) -I ../../lib -mthumb -c -O3 -o "$@" "$<"

crt0.o : ../../lib/crt0.s
	$(AS) -o "$@" "$<"


# Graphics
gfx : mario.bmp.raw.c mario.bmp.pal.c

mario.bmp.raw.c : mario.bmp.raw
	$(B2X) -c -n mario_gfx -t u8 < "$<" > "$@"

mario.bmp.pal.c : mario.bmp.pal
	$(B2X) -c -n mario_pal -t u8 < "$<" > "$@"

mario.bmp.raw : gfx/mario.bmp
	$(GFX2GBA) -q -t8 -c16 -x -St.bmp.raw -D -P "$<"

mario.bmp.pal : gfx/mario.bmp
	$(GFX2GBA) -q -t8 -c16 -x -Sp.bmp.pal -D -G "$<"

clean :
	rm -f main.*.bin main.raw main.bmp
	rm -f main.elf main.map main.bin main.vpk main.sav
	rm -f main.o crt0.o
	rm -f mario.bmp.*
	rm -f *.gba
