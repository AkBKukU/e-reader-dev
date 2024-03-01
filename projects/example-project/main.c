//------------------------------------------------//
// Sprite Demo with custom graphics, based on:    //
// Nintendo e-Reader "Mario Sprite" Example (GBA) //
// (c) 2004 Tim Schuerewegen                      //
//------------------------------------------------//

#include "def.h"
#include "erapi.h"

extern int __end[];

const u16 palette[] = { 0x0000, 0xFFFF };

#include "gfx/gfx.c"
ERAPI_SPRITE mario_sprite = { mario_gfx, mario_pal, 4, 4, 1, 1, 8, 8, 1};

u32 mario_pos_x, mario_pos_y, mario_size;
ERAPI_HANDLE_SPRITE handle_mario;

void update_mario_size()
{
  ERAPI_SpriteAutoScaleUntilSize( handle_mario, mario_size, 1);
}

void update_mario_pos()
{
  ERAPI_SetSpritePos( handle_mario, mario_pos_x, mario_pos_y);
}

int main()
{
  ERAPI_HANDLE_REGION region;
  u32 key, quit;
  // init
  ERAPI_FadeIn( 1);
  ERAPI_InitMemory( (ERAPI_RAM_END - (u32)__end) >> 10);
  ERAPI_SetBackgroundMode( 0);
  // palette
  ERAPI_SetBackgroundPalette( &palette[0], 0x00, 0x02);
  // region & text
  region = ERAPI_CreateRegion( 0, 0, 0x01, 0x01, 0x1C, 0x03);
  ERAPI_SetTextColor( region, 0x01, 0x00);
  ERAPI_DrawText( region, 0x50, 0x08, "sprite demo");
  // background
  ERAPI_LoadBackgroundSystem( 3, 2);
  ERAPI_SetBackgroundAutoScroll( 3, 0x40, 0x40);
  // create sprite
  handle_mario = ERAPI_SpriteCreateCustom( 0, &mario_sprite);
  mario_pos_x = 120;
  mario_pos_y = 80;
  update_mario_pos();
  mario_size = 0x100;
  update_mario_size();
  // loop
  quit = 0;
  while (quit == 0)
  {
    // read keys
    key = ERAPI_GetKeyStateRaw();
    // movement
    if (key & ERAPI_KEY_LEFT) mario_pos_x--;
    if (key & ERAPI_KEY_RIGHT) mario_pos_x++;
    if (key & ERAPI_KEY_UP) mario_pos_y--;
    if (key & ERAPI_KEY_DOWN) mario_pos_y++;
    // zoom
    if (key & ERAPI_KEY_L) mario_size++;
    if (key & ERAPI_KEY_R) mario_size--;
    // quit
    if (key & ERAPI_KEY_B) quit = 1;
    // update sprite position (only if necessary)
    if (key & (ERAPI_KEY_LEFT | ERAPI_KEY_RIGHT | ERAPI_KEY_UP | ERAPI_KEY_DOWN)) update_mario_pos();
    // update sprite size (only if necessary)
    if (key & (ERAPI_KEY_L | ERAPI_KEY_R)) update_mario_size();
    // render frame
    ERAPI_RenderFrame( 1);
  }
  // free sprite
  ERAPI_SpriteFree( handle_mario);
  // exit
  return ERAPI_EXIT_TO_MENU;
}
