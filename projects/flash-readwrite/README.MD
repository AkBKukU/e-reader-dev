# Flash Read/Write

This program is a demonstration of using the save flash build into the E-Reader to store and load data.

Flash data is broken into sectors of `0xFFF` bytes that are accessed by an index value. The flash data location access is the same as games that are stored. Index 0 is the start of any game data saved to the E-Reader (this data is VPK compressed in flash and will be the size of your VPK file.) If you want to store data for your game, you will need to use an index located *after* your program data.

## Building
As long as you have properly setup the build environment in the way this repo expects and have sourced the DevKitPro resources you should be able to just run `make` to build the `SAV`(Flash Saved Game) and `BMP`(Dot Code Strip).

## Usage
Press a D-Pad direction to set a last pressed button. Press A after to reload the button and print it to the screen. Works after a reset/shutdown.

### Controls

- D-Pad: Save the text of the last button pressed
- A: Reload text
- B: Quit