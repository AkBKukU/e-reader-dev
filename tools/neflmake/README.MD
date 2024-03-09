# Nintendo e-Reader Flash Maker

	Nintendo e-Reader Flash Maker v1.2
	(c) 2003-2004 Tim Schuerewegen

	usage :
	neflmake [options]
	options :
	-i <file>          input file                  (required)
	-o <file>          output file                 (required)
	-v                 verbose                     (optional)
	-type <value>      type (0=nes 1=z80 2=gba)    (required)
	-name <string>     name                        (default = none)
	-region <value>    region (0=jap 1=usa)        (default = usa)


This is the source code to the `neflmake` make tool by Tim Schuerewegen, provided here with permission.

## Building for Linux

To build for linux enter the `platform/linux` directory and run `make`. The `neflmake` binary should be produced from that.  
  
### Changes
Some changes have been made to the original code

 - The `exe` extension was removed and the `japtext.o` dependancy added to the `makefile`
 - `stricmp` is no longer part of the standard libraries and has been added to the custom `strutil.h` source.