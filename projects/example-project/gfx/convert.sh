#!/bin/bash
PATH="$PATH:../../../bin"
cd gfx
echo "" > gfx.c

for file in *.tga
do
	gfx2gba -q -t8 -c16 -x -St.tga.raw -D -P "$file"
	gfx2gba -q -t8 -c16 -x -Sp.tga.pal -D -G "$file"

	b2x -c -n "${file%.*}_gfx" -t u8 < "$file.raw" >> "gfx.c"
	b2x -c -n "${file%.*}_pal" -t u8 < "$file.pal" >> "gfx.c"
done
