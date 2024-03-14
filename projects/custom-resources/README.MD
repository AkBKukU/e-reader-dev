# Custom Resources

This program is a demonstration of using custom assets with the E-Reader.

## Building
As long as you have properly setup the build environment in the way this repo expects and have sourced the DevKitPro resources you should be able to just run `make` to build the `SAV`(Flash Saved Game) and `BMP`(Dot Code Strip).

## Usage
This will directly run a simple maze game using a custom background and player sprite.

### Controls

- D-Pad: Move player
- B: When at end, reset game

## Sound
It's a goal for this to have custom sound eventually, but I am presently unsure how to use `ERAPI_PlayCustomSound` correctly. It takes an array for the PCM data as the first parameter for sure, I have succeeded in playing sound, but at incorrect samplerates. But the second parameter is completely unknown. I would speculate that it may be possible to set a samplerate and whether or not the sound should loop at minimum. So I suspect it is a bit masked value for configuring the playback of the sample.