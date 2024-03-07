# System Resources

This is a test and demonstration program that allows you to view all of the assets 
built into the E-Reader. The cartridge itself contains around 80 backgrounds, several hundred sound effects and music clips, and some sprites (of only Pokemon that I've been able to find.) This program gives you different menus to explore these assets on the system.

A note, the backgrounds and sprites have index maps in the program to avoid some resources. These crash the game when loaded and must be skipped. This also makes it difficult the know that you have found "all" of them. For the backgrounds I am reasonably confident they end at `101` due to how the program fails different after that index. The sprites all hard crash on an improper load (to the point of needing to restart mGBA even due to corrupting the console state.) After 244 I have not been able to find any more sprites, however, I suspect there are more because I did not see anything related to the E-Reader system, Animal Crossing, or other cards that likely have resources on the cartridge.

## Building
Like the example project, you can just run `make` to compile this as long as your build environment is working. This project has a different method of preparing sprites though that allows you to automatically import an arbitrary number of images as `TGAs` using a script to put them all into a single `gfx.c` file.

## Usage
The different mods of the program let you see different resources.

### Backgrounds
- L/R Shoulder : Switch backgrounds
- A : Hide HUD
- D-Pad : Move background
- B : Exit

### Sounds
- L/R Shoulder : Switch sound
- A : Replay Sound
- B : Exit

### Sprites
- L/R Shoulder : Switch sprite
- A : Load Sprite
- B : Exit

Note: There is a limitation I have not been abl to solve that causes the system to crash after loading too many sprites. My suspicion is the provided API does not full unallocate the data for the sprites or changes the pointer position where it attempts to load new sprites into an inappropriate memory location. From my tests, the program will crash after loading 36 sprites, a limit of 32 has been put in place for some headroom. So you must manually load each sprite to view them and once you have exhausted the 32 spaces, completely restart the system, and continue from where you left off.

