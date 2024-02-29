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
decicated E-Reader features that



source /etc/profile.d/devkit-env.sh
