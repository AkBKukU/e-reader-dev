#include "def.h"
#include "erapi.h"

extern int __end[];

const u16 palette[] = { 0x0000, 0xFFFF };

#include "gfx/gfx.c"

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

ERAPI_HANDLE_SPRITE handle_arrow_r, handle_arrow_l;
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
	ERAPI_DrawText( region, 0x40, 0x08, "System Backgrounds");

	// background
	u8 back=0;
	u16 back_pos_x=0;
	u16 back_pos_y=0;
	char back_prt[5]="test";
	ERAPI_LoadBackgroundSystem( 3, back);

	// create sprite
	handle_arrow_r = ERAPI_SpriteCreateCustom( 0, &arrow_sprite);
	ERAPI_SetSpritePos( handle_arrow_r, 220, 20);

	handle_arrow_l = ERAPI_SpriteCreateCustom( 0, &arrow_sprite);
	ERAPI_SetSpritePos( handle_arrow_l, 20, 20);
	ERAPI_SpriteMirrorToggle( handle_arrow_l,0x01); 

	// loop
	quit = 0;
	u8 timer = 0;
	u8 debounce = 30;
	while (quit == 0)
	{
		// read keys
		key = ERAPI_GetKeyStateRaw();
		// zoom
		if (timer)
		{
			timer--;
		}else{
			if (key & ERAPI_KEY_L)
			{
				ERAPI_SetSpriteFrame( handle_arrow_l,1);
				ERAPI_SpriteAutoAnimate(handle_arrow_l,10,12);
				back-=10;
				if (back>200) back = 0;
				ERAPI_LoadBackgroundSystem( 3, 0);
				ERAPI_LoadBackgroundSystem( 3, back);
				citoa(back,back_prt,10);
				ERAPI_DrawText( region, 0x50, 0x0f, back_prt);

				timer=debounce;
			}
			if (key & ERAPI_KEY_R)
			{
				ERAPI_SetSpriteFrame( handle_arrow_r,1);
				ERAPI_SpriteAutoAnimate(handle_arrow_r,10,12);
				back+=10;
				if (back>120) back = 120;
				ERAPI_LoadBackgroundSystem( 3, 0);
				ERAPI_LoadBackgroundSystem( 3, back);
				citoa(back,back_prt,10);
				ERAPI_DrawText( region, 0x50, 0x0f, back_prt);
				timer=debounce;
			}

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
	ERAPI_SpriteFree( handle_arrow_r);
	// exit
	return ERAPI_EXIT_TO_MENU;
}