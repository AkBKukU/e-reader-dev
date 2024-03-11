#!/bin/bash
PATH="$PATH:../../../bin"
cd gfx

for file in *.tga
do
	gfx2gba -q -t8 -c16 -x -St.tga.gfx -D -P "$file"
	gfx2gba -q -t8 -c16 -x -Sp.tga.pal -D -G "$file"

	raw2gba "$file.gfx"
	raw2gba "$file.pal"
done


cat *.h > gfx.h
