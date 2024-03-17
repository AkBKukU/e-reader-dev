# Flash Read/Write

This program is a demonstration of using the save flash build into the E-Reader to store and load data.

## Building
As long as you have properly setup the build environment in the way this repo expects and have sourced the DevKitPro resources you should be able to just run `make` to build the `SAV`(Flash Saved Game) and `BMP`(Dot Code Strip).

## Usage
Press a D-Pad direction to set a last pressed button. Press A after to reload the button and print it to the screen. Works after a reset/shutdown.

### Controls

- D-Pad: Save the text of the last button pressed
- A: Reload text
- B: Quit