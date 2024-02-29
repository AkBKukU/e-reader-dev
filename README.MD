# GBA E-Reader Toolchain and Project Enviornment

The E-Reader was a playing card based hardware accessory the the Nintendo
Game Boy Advance that reads optically encoded software.

The reason this hardware is of particular interest is the ability to "publish"
your own software by simply printing the dot code strips and giving them to
other's with the hardware. There is no need for any custom hardware to run
new software.

## Understanding The Development Tools

The dev tools usable for the E-Reader can be split into two categories:
 - GBA Development
 - E-Reader Conversion Tools

The E-Reader has several execution modes, most notably it can run as an NES
or as a GBA. If you stick with the GBA mode you can use the standard open
source GBA developement toolchain. The main difference is linking in some
decicated E-Reader features that are availble. These give you some generic
assets to save executable space and reduce the card count.

## GBA Toolchain

The recommended toolchain is (DevKitPro)[https://devkitpro.org] which supports
targeting multiple devices. But we are intersted in the GBA ARM toolchain for
this.

### Installation

DevKitPro has an annoying and inflexible (setup process)[https://devkitpro.org/wiki/devkitPro_pacman#Debian_and_derivatives] 
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


