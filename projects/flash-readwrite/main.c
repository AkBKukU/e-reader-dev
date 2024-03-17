#include "def.h"
#include "erapi.h"

ERAPI_HANDLE_REGION print_region;
extern int __end[];

const u16 palette[] = { 0x0000, 0xFFFF };

u8 sysexit = 0, win = 0;
u32 key;
#define INPUT_DELAY 5
char print[0xFFFF];

char test_up[]="Up button pressed last";
char test_down[]="Down button pressed last";
char test_left[]="Left button pressed last";
char test_right[]="Right button pressed last";

static inline void init()
{
	// Init
	ERAPI_InitMemory( (ERAPI_RAM_END - (u32)__end) >> 10);
	ERAPI_SetBackgroundMode( 0);

	print_region = ERAPI_CreateRegion( 0, 0, 0, 0, 25, 2);
	ERAPI_SetTextColor( print_region, 0x01, 0x00);
	ERAPI_LoadBackgroundSystem( 1, 24);
	ERAPI_FadeIn( 1);
}
int main()
{
	init();
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
			delay = INPUT_DELAY;
			if (key & ERAPI_KEY_A)
			{
				ERAPI_FlashReadSectorSingle(1,&print);
				ERAPI_ClearRegion(print_region);
				ERAPI_DrawText( print_region, 0, 0, print);
			}
			if (key & ERAPI_KEY_B)
			{
				sysexit=1;
			}
			if (key & ERAPI_KEY_LEFT)
			{
				ERAPI_ClearRegion(print_region);
				ERAPI_DrawText( print_region, 0, 0, "left");
				ERAPI_FlashWriteSectorSingle(1,&test_left);
				delay = INPUT_DELAY;
			}
			if (key & ERAPI_KEY_RIGHT)
			{
				ERAPI_ClearRegion(print_region);
				ERAPI_DrawText( print_region, 0, 0, "right");
				ERAPI_FlashWriteSectorSingle(1,&test_right);
				delay = INPUT_DELAY;
			}
			if (key & ERAPI_KEY_UP)
			{
				ERAPI_ClearRegion(print_region);
				ERAPI_DrawText( print_region, 0, 0, "up");
				ERAPI_FlashWriteSectorSingle(1,&test_up);
				delay = INPUT_DELAY;
			}
			if (key & ERAPI_KEY_DOWN)
			{
				ERAPI_ClearRegion(print_region);
				ERAPI_DrawText( print_region, 0, 0, "down");
				ERAPI_FlashWriteSectorSingle(1,&test_down);
				delay = INPUT_DELAY;
			}

		}

		ERAPI_RenderFrame(1);
	}

	// exit
	return ERAPI_EXIT_TO_MENU;
}
