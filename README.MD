# GBA E-Reader Toolchain and Project Enviornment

The E-Reader was a playing card based hardware accessory the the Nintendo
Game Boy Advance that reads optically encoded software.

The reason this hardware is of particular interest is the ability to "publish"
your own software by simply printing the dot code strips and giving them to
other's with the hardware. There is no need for any custom hardware to run
new software.

## Understanding The Development Tools

The dev tools usable for the E-Reader can be split into three categories:
 - GBA Development
 - E-Reader Conversion Tools
 - General GBA Dev Tools

The E-Reader has several execution modes, most notably it can run as an NES
or as a GBA. If you stick with the GBA mode you can use the standard open
source GBA developement toolchain. The main difference is linking in some
decicated E-Reader features that are availble. These give you some generic
assets to save executable space and reduce the card count.

## GBA Developement

The recommended toolchain is [DevKitPro](https://devkitpro.org) which supports
targeting multiple devices. But we are intersted in the GBA ARM toolchain for
this.

### Installation

DevKitPro has an annoying and inflexible [setup process](https://devkitpro.org/wiki/devkitPro_pacman#Debian_and_derivatives)
rather than distributting binaries.

The TL;DR to get it usable for E-Reader GBA Dev is the following:

	wget https://apt.devkitpro.org/install-devkitpro-pacman
	chmod +x ./install-devkitpro-pacman
	sudo ./install-devkitpro-pacman
	sudo dkp-pacman -S gba-dev

### Usage
Each time you start your terminal enviornment for GBA dev you need to load in
configuration for the DevKitPro toolchain.

	source /etc/profile.d/devkit-env.sh

This sets up environment variables for you to use in your dev processes:

	export DEVKITPRO=/opt/devkitpro
	export DEVKITARM=${DEVKITPRO}/devkitARM
	export PATH=${DEVKITPRO}/tools/bin:$PATH     

The `$DEVKITARM/bin` directory contains the `gcc` tools for compiling C/C++ code
for the GBA. 

## E-Reader Conversion Tools

Once you have a valid GBA binary you need to convert it to work on the E-Reader.
There is a version of the tools available for [multiple platforms](https://github.com/breadbored/nedclib)
Downloading and compiling these tools will give you nearly everything you need
to create usable E-Reader cards.

With the tools, there are a few steps to this process:

 1. `nevpk` - VPK Compression
 2. `nedcmake` - Dot Code binary generation
 3. `nedcenc` - Dot Code Encoding
 4. `raw2bmp` - Dot Code image generation

These steps are best done as part of a build process after compiling with GCC
and make sense to include in a MakeFile. They generally each take the output as
the input to the next step. There is a variance when your program becomes large
enough to split across multiple cards (which I am not able to cover yet.)

There is one alternative step 2 you can do when doing development. Using 
[neflashmaker](https://www.caitsith2.com/ereader/tools/neflashmaker10.zip)
will encode the program in a way GBA emulators can load as game save data. The 
E-Reader can store software on it, and this can "inject" your program into it.







