# Project Storage

Projects here can refrence the `../../bin` folder for running tools.

## Start

The `start` script sets up the development enviornment and must be 
run with `source start` for every new session.

## Example Project

An example project has been included to show how the development tools can be
used. It is based off of the [Mario Sprite](https://web.archive.org/web/20160514172806/http://users.skynet.be/firefly/gba/e-reader)
example by Tim Schuerewegen but modified with a different sprite image.

The build process has also been changed to simplify how image files are 
included. A `gfx` folder has been added that that all graphics data should go
into. It will be converted and put into `gfx/gfx.c` for inclusion in the 
program easier. This script could be further expanded, for example, to 
check filenames for information such as the color depth of the desired sprite.

This project also puts all intermediate files into `build/` to clean up the 
project space. The dot code BMP file and game save SAV files are put in the 
project root.
