# Example Project

This program is a simple demonstration of setting up a game and loading a
sprite with inputs for moving it on screen.

## Building
As long as you have properly setup the build environment in the way this repo expects and have sourced the DevKitPro resources you should be able to just run `make` to build the `SAV`(Flash Saved Game) and `BMP`(Dot Code Strip).

## Usage
Depending on which built file you want to use there are different ways to run the game.  
  
### SAV
The `SAV` file simulates a set of cards having been read and stored in the E-Reader's flash storage. Using an emulator you can load the `SAV` file as the accompanying save game file for the E-Reader ROM. A flash cart may also be able to use it.

Once the game is running you'll be given a new option in the menu of the E-Reader to run a saved game. 

### BMP
The `BMP` is the Dot Code strip that would be printed on a physical card. The emulator *mGBA* can read these files directly to simulate swiping cards. This program isn't very big and only requires one strip, larger programs can be spread across multiple cards though.

It is possible to print these to use on real hardware as well, but that is beyond the scope of this document.
