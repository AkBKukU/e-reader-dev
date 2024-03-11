#!/bin/bash
PATH="$PATH:../../../bin"
cd gfx

#for file in *.tga
#do
	# tile 8x8
	# 16b depth
	#gfx2gba -q -t8 -c16 -x -St.tga.gfx -D -P "$file"
	#gfx2gba -q -t8 -c16 -x -Sp.tga.pal -D -G "$file"

	#raw2gba "$file.gfx"
	#raw2gba "$file.pal"
#done

grit *.tga -gu8 -gB8 -ftc -fh! -pS -tw8 -th8 -tc


cat *.c > gfx.h
#cat *.c >> gfx.h
