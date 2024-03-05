#include "def.h"
#include "erapi.h"

extern int __end[];

const u16 palette[] = { 0x0000, 0xFFFF };

#include "gfx/gfx.c"
u8 fade = 50;
u8 back=2;
u16 sound=0,sprite=1;
u8 mode=0, sysexit=0, quit=0, main_menu = 1;
u32 key=0;
u8 debounce = 10;

const u8 MODE_MENU=0,MODE_BACKGROUND=1,MODE_SOUND=2,MODE_SPRITE=3,
SOUND_MENUMOVE=65,SOUND_SPRITE_MUSIC=93;

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

ERAPI_HANDLE_SPRITE handle_arrow_r, handle_arrow_l, handle_arrow_menu,
sprite_in, sprite_out;
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

void sound_sweep(s8 dir)
{
	ERAPI_SoundPause(sound);
	sound+=dir;
	u16 sound_max=884;
	if (sound>sound_max && dir == -1) sound = sound_max;
	if (sound>sound_max) sound = 0;

	ERAPI_PlaySoundSystem(sound);
}

//u8 bad_sprite_count=10;
u8 bad_sprite[] = {28,30,31,32,33,34,35,36,48,49,51,52,61,62,63,67,70,71,75,79,91,92,94,95,
	101,103,104,105,108,109,112,122,127,128,129,134,135,147,148,149,163,167,168,172,173,179,189,190,
	192,197,198,205,206,207,208,211,212,213,215,216,236,237,238,239,240
};
int bad_sprite_count = sizeof(bad_sprite);
void sprite_sweep(s8 dir)
{
	sprite+=dir;
	u16 sprite_max=21;
	if (sprite < 2) sprite = sprite_max;
	if (sprite>sprite_max) sprite = 1;
	u8 check = 1;

	while(check)
	{
		check=0;
		for (u8 i=0;i<bad_sprite_count;++i)
		{
			if (sprite == bad_sprite[i])
			{
				sprite += dir;
				check=1;
			}
		}
	}
}
void init()
{
	ERAPI_InitMemory( (ERAPI_RAM_END - (u32)__end) >> 10);
	ERAPI_SetBackgroundMode( 0);
	// create sprites
	handle_arrow_r = ERAPI_SpriteCreateCustom( 0, &arrow_sprite);
	ERAPI_SetSpritePos( handle_arrow_r, 220, 20);

	handle_arrow_l = ERAPI_SpriteCreateCustom( 0, &arrow_sprite);
	ERAPI_SetSpritePos( handle_arrow_l, 20, 20);
	ERAPI_SpriteMirrorToggle( handle_arrow_l,0x01);

	handle_arrow_menu = ERAPI_SpriteCreateCustom( 0, &arrow_sprite);
	ERAPI_SetSpritePos( handle_arrow_menu, 60, 60+14);

	return;
}

u8 hud_chooser (ERAPI_HANDLE_REGION chooser, u8 mode, u32 key, u8 show)
{
	s8 dir = 0;
	char num_print[5]="test";

	// Re-Draw
	ERAPI_ClearRegion(chooser);
	if (show) switch(mode)
	{
		case MODE_BACKGROUND:
			ERAPI_DrawText( chooser, 0, 0, "Background:");
			citoa(back,num_print,10);
			ERAPI_DrawText( chooser, ERAPI_GetTextWidth(chooser,"Background:")+4, 0, num_print);
			break;
		case MODE_SOUND:
			ERAPI_DrawText( chooser, 0, 0, "   Sound:");
			citoa(sound,num_print,10);
			ERAPI_DrawText( chooser, ERAPI_GetTextWidth(chooser,"   Sound:")+4, 0, num_print);
			break;
		case MODE_SPRITE:
			ERAPI_DrawText( chooser, 0, 0, "   Sprite:");
			citoa(sprite,num_print,10);
			ERAPI_DrawText( chooser, ERAPI_GetTextWidth(chooser,"   Sprite:")+4, 0, num_print);
			break;
	};
	ERAPI_SetSpriteVisible(handle_arrow_l,show);
	ERAPI_SetSpriteVisible(handle_arrow_r,show);

	// Check Inputs
	if (key & ERAPI_KEY_L)
	{
		ERAPI_SetSpriteFrame( handle_arrow_l,1);
		ERAPI_SpriteAutoAnimate(handle_arrow_l,debounce,debounce+ 2);
		if (mode!=MODE_SOUND) ERAPI_PlaySoundSystem(SOUND_MENUMOVE+1);
		dir+=-1;
	}
	if (key & ERAPI_KEY_R)
	{
		ERAPI_SetSpriteFrame( handle_arrow_r,1);
		ERAPI_SpriteAutoAnimate(handle_arrow_r,debounce,debounce+ 2);
		if (mode!=MODE_SOUND) ERAPI_PlaySoundSystem(SOUND_MENUMOVE);
		dir+=1;
	}

	return dir;
}

void mode_run_sprite(ERAPI_HANDLE_REGION chooser)
{
	// init
	ERAPI_LoadBackgroundSystem( 3, 70);
	ERAPI_LayerShow(3);

	// background
	u16 back_pos_x=0;
	u16 back_pos_y=0;
	ERAPI_SetBackgroundOffset(3,back_pos_x,back_pos_y);

	hud_chooser(chooser,MODE_SPRITE,0, 1);


	ERAPI_PlaySoundSystem(SOUND_SPRITE_MUSIC);
	ERAPI_FadeIn( fade);
	ERAPI_RenderFrame( fade);

	sprite_in = ERAPI_SpriteCreateSystem( 1, sprite);
	ERAPI_SetSpritePos( sprite_in, 120, 80);

	// loop
	quit = 0;
	u8 timer = 0;
	u8 show = 1;
	int dir = 0;
	while (quit == 0)
	{
		// read keys
		key = ERAPI_GetKeyStateRaw();
		if (timer)
		{
			timer--;
		}else{
			if (key & ERAPI_KEY_A)
			{
				show = !show;
				timer=debounce;
			}

			dir = hud_chooser(chooser,MODE_SPRITE,key, show);
			if (dir != 0)
			{
				ERAPI_SpriteFree(sprite_in);
				sprite_sweep(dir);
				hud_chooser(chooser,MODE_SPRITE,key, show);
		ERAPI_RenderFrame( 1);
				sprite_in = ERAPI_SpriteCreateSystem( 1, sprite);
				ERAPI_SetSpritePos( sprite_in, 120, 80);
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

	// Hide everything
	ERAPI_PlaySoundSystem(68);
	ERAPI_FadeOut(fade);
	ERAPI_RenderFrame( fade);
	hud_chooser(chooser,MODE_BACKGROUND,0, 0);
	ERAPI_SpriteFree(sprite_in);
	ERAPI_SoundPause(SOUND_SPRITE_MUSIC);

	ERAPI_LayerHide(3);

	mode = 0;
	return;
}


void mode_run_sound(ERAPI_HANDLE_REGION chooser)
{
	// init
	ERAPI_LoadBackgroundSystem( 3, 60);
	ERAPI_LoadBackgroundSystem( 2, 21);
	ERAPI_LoadBackgroundSystem( 1, 8);
	ERAPI_LayerShow(3);
	ERAPI_SetBackgroundOffset(3,0,45);
	ERAPI_SetBackgroundAutoScroll(3,20,0);
	ERAPI_LayerShow(2);
	ERAPI_SetBackgroundOffset(2,0,0);
	ERAPI_SetBackgroundAutoScroll(2,40,0);
	ERAPI_LayerShow(1);
	ERAPI_SetBackgroundOffset(1,0,-55);
	ERAPI_SetBackgroundAutoScroll(1,80,0);

	hud_chooser(chooser,MODE_SOUND,0, 1);


	ERAPI_FadeIn( fade);
	ERAPI_RenderFrame( fade);

	// loop
	quit = 0;
	u8 timer = 0;
	u8 show = 1;
	int dir = 0;
	while (quit == 0)
	{
		// read keys
		key = ERAPI_GetKeyStateRaw();
		if (timer)
		{
			timer--;
		}else{
			if (key & ERAPI_KEY_A)
			{
				ERAPI_PlaySoundSystem(sound);
				timer=debounce;
			}

			dir = hud_chooser(chooser,MODE_SOUND,key, show);
			if (dir != 0)
			{
				sound_sweep(dir);
				timer=debounce;
			}

		}
		// quit
		if (key & ERAPI_KEY_B) quit = 1;
		// render frame
		ERAPI_RenderFrame( 1);
	}

	// Hide everything
	ERAPI_PlaySoundSystem(68);
	ERAPI_FadeOut(fade);
	ERAPI_RenderFrame( fade);
	hud_chooser(chooser,MODE_BACKGROUND,0, 0);

	ERAPI_LayerHide(1);
	ERAPI_SetBackgroundAutoScroll(1,0,0);
	ERAPI_SetBackgroundOffset(1,0,0);
	ERAPI_LayerHide(2);
	ERAPI_SetBackgroundAutoScroll(2,0,0);
	ERAPI_SetBackgroundOffset(2,0,0);
	ERAPI_LayerHide(3);
	ERAPI_SetBackgroundAutoScroll(3,0,0);
	ERAPI_SetBackgroundOffset(3,0,0);

	mode = 0;
	return;
}

void background(ERAPI_HANDLE_REGION chooser)
{
	// init
	background_sweep( 0);
	ERAPI_LayerShow(3);

	// background
	u16 back_pos_x=0;
	u16 back_pos_y=0;
	ERAPI_SetBackgroundOffset(3,back_pos_x,back_pos_y);

	hud_chooser(chooser,MODE_BACKGROUND,0, 1);


	ERAPI_PlaySoundSystem(82);
	ERAPI_FadeIn( fade);
	ERAPI_RenderFrame( fade);

	// loop
	quit = 0;
	u8 timer = 0;
	u8 show = 1;
	int dir = 0;
	while (quit == 0)
	{
		// read keys
		key = ERAPI_GetKeyStateRaw();
		if (timer)
		{
			timer--;
		}else{
			if (key & ERAPI_KEY_A)
			{
				show = !show;
				timer=debounce;
			}

			dir = hud_chooser(chooser,MODE_BACKGROUND,key, show);
			if (dir != 0)
			{
				background_sweep(dir);
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

	// Hide everything
	ERAPI_PlaySoundSystem(68);
	ERAPI_FadeOut(fade);
	ERAPI_RenderFrame( fade);
	hud_chooser(chooser,MODE_BACKGROUND,0, 0);
	ERAPI_SoundPause(82);

	ERAPI_LayerHide(3);

	mode = 0;
	return;
}

void menu (ERAPI_HANDLE_REGION region_menu)
{
	ERAPI_ClearRegion(region_menu);
	ERAPI_SetTextColor( region_menu, 0x2, 0);
	ERAPI_DrawText( region_menu, 0, 0, "System Assets");

	ERAPI_DrawText( region_menu, 0, 10+12, "Backgrounds");
	ERAPI_DrawText( region_menu, 0, 24+12, "Sounds");
	ERAPI_DrawText( region_menu, 0, 36+12, "Sprites");
	ERAPI_SetSpriteVisible(handle_arrow_menu,1);

	ERAPI_LayerShow(1);
	ERAPI_LoadBackgroundSystem( 1, 24);
	ERAPI_SetBackgroundAutoScroll(1,0,0x40);

	ERAPI_PlaySoundSystem(738);
	ERAPI_FadeIn( fade);
	ERAPI_RenderFrame( fade);


	// loop
	quit = 0;
	u8 timer = 0;
	while (quit == 0)
	{
		// read keys
		key = ERAPI_GetKeyStateRaw();
		if (timer)
		{
			timer--;
		}else{
			if (key & ERAPI_KEY_A)
			{
				ERAPI_SetSpriteFrame( handle_arrow_menu,1);
				ERAPI_SpriteAutoAnimate(handle_arrow_menu,debounce,debounce+ 2);
				mode = main_menu;
				ERAPI_PlaySoundSystem(86);
				quit = 1;
			}

			if (key & ERAPI_KEY_UP)
			{
				main_menu--;
				if (main_menu<1) main_menu=3;
				ERAPI_PlaySoundSystem(SOUND_MENUMOVE);
				timer=debounce;
			}
			if (key & ERAPI_KEY_DOWN)
			{
				main_menu++;
				if (main_menu>3) main_menu=1;
				ERAPI_PlaySoundSystem(SOUND_MENUMOVE+1);
				timer=debounce;
			}
			// quit
			if (key & ERAPI_KEY_B)
			{
				ERAPI_PlaySoundSystem(68);
				quit = 1;
				sysexit= 1;
			}
		}
		ERAPI_SetSpritePos( handle_arrow_menu, 60, 60+(main_menu * 14));
		ERAPI_RenderFrame( 1);
	}
	ERAPI_SoundPause(738);
	ERAPI_FadeOut(fade);
	ERAPI_RenderFrame( fade);
	ERAPI_LayerHide(1);
	ERAPI_ClearRegion(region_menu);
	ERAPI_SetSpriteVisible(handle_arrow_menu,0);
	ERAPI_SetBackgroundAutoScroll(1,0,0);

	return;
}

int main()
{
	init();

	// region & text
	ERAPI_HANDLE_REGION region_menu = ERAPI_CreateRegion( 0, 0, 0xa, 0x06, 0x0A, 0x08);

	// region & text
	ERAPI_HANDLE_REGION chooser = ERAPI_CreateRegion(
		0,
		0, // 0= no change, 4 = text grey if color is set, * text is black
		0x0a, 0x02,
		0xc, 0x01
	);
	ERAPI_SetTextColor( chooser, 0x01, 0x00);

	hud_chooser(chooser,MODE_MENU,0, 0);

	// palette
	ERAPI_SetBackgroundPalette( &palette[0], 0x00, 0x02);

	while (sysexit == 0)
	{
		switch(mode)
		{
			case MODE_MENU: 	menu (region_menu); break;
			case MODE_BACKGROUND: 	background (chooser); break;
			case MODE_SOUND: 	mode_run_sound (chooser); break;
			case MODE_SPRITE: 	mode_run_sprite (chooser); break;
		}
	}

	// exit
	ERAPI_SpriteFree( handle_arrow_menu);
	ERAPI_SpriteFree( handle_arrow_l);
	ERAPI_SpriteFree( handle_arrow_r);
	return ERAPI_EXIT_TO_MENU;
}
