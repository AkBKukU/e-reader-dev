#include "def.h"
#include "erapi.h"

#include "gfx/gfx.h"
#include "map/map.h"
//#include "snd/snd.h" // The goal is to later add sound but so far I haven't got ERAPI_PlaySoundCustom to behave predictably

#define MAZE_GRID 8
#define MAZE_WIDTH 32
#define INPUT_DELAY 5

ERAPI_HANDLE_REGION chooser;
extern int __end[];

const u16 palette[] = { 0x0000, 0xFFFF };

u8 sysexit = 0, win = 0;
u32 key;
u16 MAZE_OPEN, MAZE_FINISH;

u16 rainPal[mapSharedPalLen/2];


ERAPI_BACKGROUND background =
{
  mapTiles,
  mapSharedPal,
  mapMap,
  sizeof( mapTiles) >> 5,
  2
};

ERAPI_SPRITE player_sprite = { playerTiles, gfxSharedPal, 1, 1, 1, 1, 8, 8, 1};
ERAPI_HANDLE_SPRITE player_handle;
s8 px=1,py=0;

static inline void init()
{
	// Init
	ERAPI_InitMemory( (ERAPI_RAM_END - (u32)__end) >> 10);
	ERAPI_SetBackgroundMode( 0);
	ERAPI_SetBackgroundPalette( &palette[0], 0x00, 0x04);
	ERAPI_LoadBackgroundCustom( 3, &background);
	player_handle = ERAPI_SpriteCreateCustom( 0, &player_sprite);
	MAZE_OPEN = mapMap[1];
	MAZE_FINISH = mapMap[28+19*MAZE_WIDTH];

	ERAPI_FadeIn( 1);
}
void pmove(s8 x, s8 y)
{
	char num_print[5];
	u8 next = mapMap[ (px+x) + ((py+y)*MAZE_WIDTH)];
	if ( next==MAZE_OPEN || next==MAZE_FINISH)
	{
		px += x;
		py += y;
	}
	ERAPI_SetSpritePos( player_handle,px*MAZE_GRID+4, py*MAZE_GRID+4);

	if (  next==MAZE_FINISH)
	{
		win = 1;

		for ( u8 i=1;i < (mapSharedPalLen / 2) ;++i )
		{
			rainPal[i] = mapSharedPal[i];
		}
	}
}

void win_fun()
{
	for ( u8 i=1;i < (mapSharedPalLen / 2) ;++i )
	{
		++rainPal[i];
	}

	ERAPI_BACKGROUND background_rain = {
		mapTiles,
		rainPal,
		mapMap,
		sizeof( mapTiles) >> 5,
		2
	};
	ERAPI_LoadBackgroundCustom( 3, &background_rain);
}

int main()
{
	init();
	pmove(0,0);
	// Main Loop
	u8 delay=0;
	while (sysexit == 0)
	{
		// Switch menus
		key = ERAPI_GetKeyStateRaw();
		// movement
		if (delay)
		{
			delay--;
		} else {
			if (win)
			{
				win_fun();
				delay = INPUT_DELAY;
				if (key & ERAPI_KEY_B)
				{
					win = 0;
					ERAPI_LoadBackgroundCustom( 3, &background);
					px=1;
					py=0;
					pmove(0,0);
				}
			} else {
				if (key & ERAPI_KEY_LEFT)
				{
					pmove(-1,0);
					delay = INPUT_DELAY;
				}
				if (key & ERAPI_KEY_RIGHT)
				{
					pmove(1,0);
					delay = INPUT_DELAY;
				}
				if (key & ERAPI_KEY_UP)
				{
					pmove(0,-1);
					delay = INPUT_DELAY;
				}
				if (key & ERAPI_KEY_DOWN)
				{
					pmove(0,1);
					delay = INPUT_DELAY;
				}
			}
		}

		ERAPI_RenderFrame(1);
	}

	// exit
	return ERAPI_EXIT_TO_MENU;
}
