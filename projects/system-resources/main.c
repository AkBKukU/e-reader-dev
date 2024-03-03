#include "def.h"
#include "erapi.h"

extern int __end[];

const u16 palette[] = { 0x0000, 0xFFFF };

#include "gfx/gfx.c"
u8 fade = 50;
u8 back=0;
u8 mode=0, sysexit=0;

// Sprite for arrows
ERAPI_SPRITE arrow_sprite = { 
	arrow_gfx, 
	arrow_pal, 
	2, // Tile X
	2, // Tile Y
	2, // Frame Count
	1, // ?
	0, // ? width
	0, // ? height
	1  // ? frames
};

ERAPI_HANDLE_SPRITE handle_arrow_r, handle_arrow_l, handle_arrow_menu;
// A utility function to reverse a string
void reverse(char str[], int length)
{
	int start = 0;
	int end = length - 1;
	while (start < end) {
		char temp = str[start];
		str[start] = str[end];
		str[end] = temp;
		end--;
		start++;
	}
}

// Implementation of citoa()
char* citoa(int num, char* str, int base)
{
	int i = 0;
	u8 isNegative = 0;
 
	/* Handle 0 explicitly, otherwise empty string is
	 * printed for 0 */
	if (num == 0) {
		str[i++] = '0';
		str[i] = '\0';
		return str;
	}
 
	// In standard itoa(), negative numbers are handled
	// only with base 10. Otherwise numbers are
	// considered unsigned.
	if (num < 0 && base == 10) {
		isNegative = 1;
		num = -num;
	}
 
	// Process individual digits
	while (num != 0) {
		int rem =ERAPI_Mod( num , base);
		str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
		num =ERAPI_Div( num , base);
	}
 
	// If number is negative, append '-'
	if (isNegative)
		str[i++] = '-';
 
	str[i] = '\0'; // Append string terminator
 
	// Reverse the string
	reverse(str, i);
 
	return str;
}

u8 bad_back_count=21;
u8 bad_back[] = {28,42,46,47,48,55,62,63,64,71,72,73,79,80,82,83,84,86,87,88,89};
void background_sweep(s8 dir)
{
	back+=dir;
	if (back>101 && dir == -1) back = 101;
	if (back>101) back = 0;
	u8 check = 1;
	while(check)
	{
		check=0;
		for (u8 i=0;i<bad_back_count;++i)
		{
			if (back == bad_back[i])
			{
				back += dir;
				check=1;
			}
		}
	}

	ERAPI_LoadBackgroundSystem( 3, back);
}


void background()
{
	u32 key, quit;
	// init
	ERAPI_LoadBackgroundSystem( 3, back);
	ERAPI_LayerShow(3);
	// region & text
	//ERAPI_HANDLE_REGION chooser = ERAPI_CreateRegion( 0, 0, 0x01, 0x01, 0x1C, 0x03);
	//ERAPI_SetTextColor( chooser, 0x01, 0x00);


	// background
	u16 back_pos_x=0;
	u16 back_pos_y=0;
	char back_prt[5]="test";

	// create sprite
	handle_arrow_r = ERAPI_SpriteCreateCustom( 0, &arrow_sprite);
	ERAPI_SetSpritePos( handle_arrow_r, 220, 20);

	handle_arrow_l = ERAPI_SpriteCreateCustom( 0, &arrow_sprite);
	ERAPI_SetSpritePos( handle_arrow_l, 20, 20);
	ERAPI_SpriteMirrorToggle( handle_arrow_l,0x01);

	ERAPI_FadeIn( fade);
	ERAPI_RenderFrame( fade);

	// loop
	quit = 0;
	u8 timer = 0;
	u8 debounce = 10;
	u8 show = 1;
	while (quit == 0)
	{
		// read keys
		key = ERAPI_GetKeyStateRaw();
		// zoom
		if (timer)
		{
			timer--;
		}else{
			if (key & ERAPI_KEY_A)
			{
				show = !show;
				timer=debounce;
			}

			if (key & ERAPI_KEY_L)
			{
				ERAPI_SetSpriteFrame( handle_arrow_l,1);
				ERAPI_SpriteAutoAnimate(handle_arrow_l,debounce,debounce+ 2);
				background_sweep(-1);

				timer=debounce;
			}
			if (key & ERAPI_KEY_R)
			{
				ERAPI_SetSpriteFrame( handle_arrow_r,1);
				ERAPI_SpriteAutoAnimate(handle_arrow_r,debounce,debounce+ 2);
				background_sweep(1);

				timer=debounce;
			}

			//ERAPI_ClearRegion(chooser);
			if (show)
			{
				//ERAPI_DrawText( chooser, 0x40, 0x06, "System Backgrounds");
				citoa(back,back_prt,10);
				//ERAPI_DrawText( chooser, 0x65, 0x0f, back_prt);
			}
			ERAPI_SetSpriteVisible(handle_arrow_l,show);
			ERAPI_SetSpriteVisible(handle_arrow_r,show);

			if (key & ERAPI_KEY_LEFT) back_pos_x--;
			if (key & ERAPI_KEY_RIGHT) back_pos_x++;
			if (key & ERAPI_KEY_UP) back_pos_y--;
			if (key & ERAPI_KEY_DOWN) back_pos_y++;
			ERAPI_SetBackgroundOffset(3,back_pos_x,back_pos_y);
		}
		// quit
		if (key & ERAPI_KEY_B) quit = 1;
		// render frame
		ERAPI_RenderFrame( 1);
	}
	// free sprite
	//ERAPI_ClearRegion(chooser);
	ERAPI_FadeOut(fade);
	ERAPI_RenderFrame( fade);
	ERAPI_SetSpriteVisible(handle_arrow_l,0);
	ERAPI_SetSpriteVisible(handle_arrow_r,0);
	ERAPI_RenderFrame( 1);
	ERAPI_SpriteFree( handle_arrow_l);
	ERAPI_SpriteFree( handle_arrow_r);

	ERAPI_LayerHide(3);

	mode = 0;
	return;
}

void menu ()
{
	ERAPI_FadeIn( fade);
	// region & text
	// ERAPI_HANDLE_REGION region;
	// region = ERAPI_CreateRegion( 0, 0, 0x01, 0x01, 0x1C, 0x03);
	// ERAPI_SetTextColor( region, 0x01, 0x00);

	// background
	u16 back_pos_x=0;
	u16 back_pos_y=0;
	char back_prt[5]="test";

	ERAPI_LayerShow(1);
	ERAPI_LoadBackgroundSystem( 1, 24);
	ERAPI_SetBackgroundAutoScroll(1,0,0x40);

	// create sprite
	handle_arrow_menu = ERAPI_SpriteCreateCustom( 0, &arrow_sprite);
	ERAPI_SetSpritePos( handle_arrow_menu, 20, 80);

	ERAPI_RenderFrame( fade);

	// loop
	u32 key, quit;
	quit = 0;
	u8 timer = 0;
	u8 debounce = 10;
	u8 menu = 1;
	while (quit == 0)
	{
		key = ERAPI_GetKeyStateRaw();
		// zoom
		if (timer)
		{
			timer--;
		}else{
			if (key & ERAPI_KEY_A)
			{
				mode = 1;
				quit = 1;
			}

			if (key & ERAPI_KEY_UP)
			{
				menu--;
				if (menu<1) menu=1;
				timer=debounce;
			}
			if (key & ERAPI_KEY_DOWN)
			{
				menu++;
				if (menu>2) menu=2;
				timer=debounce;
			}
			// quit
			if (key & ERAPI_KEY_B)
			{
				quit = 1;
				sysexit= 1;
			}
		}
		ERAPI_RenderFrame( 1);
	}

	ERAPI_FadeOut(fade);
	ERAPI_RenderFrame( fade);
	ERAPI_LayerHide(1);
	ERAPI_SetSpriteVisible(handle_arrow_menu,0);
	// exit
	ERAPI_SpriteFree( handle_arrow_menu);

	return;
}

int main()
{
	ERAPI_InitMemory( (ERAPI_RAM_END - (u32)__end) >> 10);
	ERAPI_SetBackgroundMode( 0);
	// palette
	ERAPI_SetBackgroundPalette( &palette[0], 0x00, 0x02);

	while (sysexit == 0)
	{
		switch(mode)
		{
			case 0: menu (); break;
			case 1: background (); break;
		}
	}

	return ERAPI_EXIT_TO_MENU;
}
