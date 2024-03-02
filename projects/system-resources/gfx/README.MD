# Graphics

My prefrence so far when creating graphics has been to use
 [gfx2gba](https://www.gbadev.org/tools.php?showinfo=142) & [b2x](http://www.devrs.com/gb/files/b2x.zip)
which are a GBA graphics conversion and c array encoder. This uses a
palettization of the colors in a sprite and doesn't require a large amount
of code space.

## Input

`gfx2gba` supports several different image file formats but the easiest to
work with I've found is TGA exported by Gimp.

Use the following steps to create a valid TGA:

 1. Open or create your sprite (with a size as a multiple of 8 pixels)
 2. For final export set Mode>Color>Indexed...
 3. Open Color>Map>Set Colormap... and move your transparent color into position #0
 4. Export as TGA

