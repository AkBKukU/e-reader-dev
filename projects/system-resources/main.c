#include "def.h"
#include "erapi.h"

extern int __end[];

const u16 palette[] = { 0x0000, 0xFFFF };

u8 back=2;
u16 sound=0,sprite=1;
u8 mode=0, sysexit=0, quit=0, main_menu = 1;
u8 debounce = 10;

#define SPRITES_MAX 32
#define FADE  50

#define MODE_MENU 0
#define MODE_BACKGROUND 1
#define MODE_SOUND 2
#define MODE_SPRITE 3

#define SOUND_MENUMOVE 65
#define SOUND_MENUMOVE_DOWN 66
#define SOUND_MENU_MUSIC 738
#define SOUND_SPRITE_MUSIC 93
#define SOUND_SPRITE_SHOW 16
#define SOUND_EXIT 68
#define SOUND_BACK_MUSIC 82

ERAPI_HANDLE_REGION chooser;
ERAPI_HANDLE_REGION region_menu;

// Sprite for arrows
#include "gfx/gfx.c"
ERAPI_SPRITE arrow_sprite = { 
	arrowTiles,
	gfxSharedPal,
	1, // Tile X
	1, // Tile Y
	2, // Frame Count
	0, // ?
	0, // ? width
	0, // ? height
	0  // ? frames
};

ERAPI_HANDLE_SPRITE handle_arrow_r, handle_arrow_l, sprite_in;

// A utility function to reverse a string
static inline void reverse(char str[], int length)
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
 
	/* Handle 0 explicitly, otherwise empty string is
	 * printed for 0 */
	if (num == 0) {
		str[i++] = '0';
		str[i] = '\0';
		return str;
	}
 
	// Process individual digits
	while (num != 0) {
		int rem =ERAPI_Mod( num , base);
		str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
		num =ERAPI_Div( num , base);
	}

	str[i] = '\0'; // Append string terminator
 
	// Reverse the string
	reverse(str, i);
 
	return str;
}

u8 bad_back[] = {28,42,46,47,48,55,62,63,64,71,72,73,79,80,82,83,84,86,87,88,89};
u8 bad_sound[]={};
u8 bad_sprite[] = {28,30,31,32,33,34,35,36,48,49,51,52,61,62,63,67,70,71,75,79,91,92,94,95,
	101,103,104,105,108,109,112,122,127,128,129,134,135,147,148,149,163,167,168,172,173,179,189,190,
	192,197,198,205,206,207,208,211,212,213,215,216,236,237,238,239,240
};
u8 sprite_loaded[SPRITES_MAX];
u8 sprite_loaded_count=0;

static inline void choose_num(s8 dir, u16 *value, u8 min, u16 max, u8 skip[], u8 skip_count )
{
	*value += dir;
	if (*value < min || (*value > max && dir == -1)) *value = max;
	if (*value > max) *value = min;
	//u8 skip_count = sizeof(skip);
	u8 check = 1;
	while(check)
	{
		check=0;
		for (u8 i=0;i<skip_count;++i)
		{
			if (*value == skip[i])
			{
				*value += dir;
				check=1;
				break;
			}
		}
	}

	return;
}

u8 hud_chooser (u8 mode, u8 show)
{
	s8 dir = 0;
	char num_print[5];

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
			ERAPI_DrawText( chooser, 0, 0, "    Sprite:");
			citoa(SPRITES_MAX-sprite_loaded_count,num_print,10);
			ERAPI_DrawText( chooser, 0, 0, num_print);
			citoa(sprite,num_print,10);
			ERAPI_DrawText( chooser, ERAPI_GetTextWidth(chooser,"    Sprite:")+4, 0, num_print);
			break;
	};
	ERAPI_SetSpriteVisible(handle_arrow_l,show);
	ERAPI_SetSpriteVisible(handle_arrow_r,show);

	// Check Inputs
	if (ERAPI_GetKeyStateRaw() & ERAPI_KEY_L)
	{
		ERAPI_SetSpriteFrame( handle_arrow_l,1);
		ERAPI_SpriteAutoAnimate(handle_arrow_l,debounce,12);
		if (mode!=MODE_SOUND) ERAPI_PlaySoundSystem(SOUND_MENUMOVE_DOWN);
		dir+=-1;
	}
	if (ERAPI_GetKeyStateRaw() & ERAPI_KEY_R)
	{
		ERAPI_SetSpriteFrame( handle_arrow_r,1);
		ERAPI_SpriteAutoAnimate(handle_arrow_r,debounce,12);
		if (mode!=MODE_SOUND) ERAPI_PlaySoundSystem(SOUND_MENUMOVE);
		dir+=1;
	}

	return dir;
}

static inline void mode_run_sprite()
{
	// init
	ERAPI_LoadBackgroundSystem( 3, 70);
	ERAPI_SetBackgroundOffset(3,0,0);
	ERAPI_LayerShow(3);

	hud_chooser(MODE_SPRITE, 1);


	ERAPI_PlaySoundSystem(SOUND_SPRITE_MUSIC);
	ERAPI_FadeIn( FADE);
	ERAPI_RenderFrame( FADE);

	// loop
	quit = 0;
	u8 timer = 0;
	u8 show = 0;
	u8 load = 0;
	int dir = 0;
	while (quit == 0)
	{
		// read keys
		if (timer)
		{
			timer--;
		}else{
			if (ERAPI_GetKeyStateRaw() & ERAPI_KEY_A)
			{
				int missing = 1;
				for (u8 i=0;i<sprite_loaded_count;++i)
				{
					if (sprite == sprite_loaded[i]) missing = 0;
				}
				if (missing) if (sprite_loaded_count < SPRITES_MAX)
				{
					ERAPI_PlaySoundSystem(SOUND_SPRITE_SHOW);
					sprite_loaded[sprite_loaded_count] = sprite;
					++sprite_loaded_count;
					load = 1;
				}
				timer=debounce;
			}

			dir = hud_chooser(MODE_SPRITE, 1);
			if (dir != 0)
			{
				choose_num(dir, &sprite, 1, 244, bad_sprite,sizeof(bad_sprite));
				timer=debounce;
			}
		}
		// quit
		if (ERAPI_GetKeyStateRaw() & ERAPI_KEY_B) quit = 1;
		show=load;
		for (u8 i=0;i<sprite_loaded_count;++i)
		{
			if (sprite == sprite_loaded[i]) show = 1;
		}

		// render frame
		if (show)
		{
			sprite_in = ERAPI_SpriteCreateSystem( 1, sprite);
			ERAPI_SetSpritePos( sprite_in, 120, 80);
		}
		ERAPI_RenderFrame( 1);
		load = 0;
		if (show) ERAPI_SpriteFree(sprite_in);
	}

	// Hide everything
	ERAPI_PlaySoundSystem(SOUND_EXIT);
	ERAPI_FadeOut(FADE);
	ERAPI_RenderFrame( FADE);
	hud_chooser(MODE_SPRITE, 0);
	ERAPI_SoundPause(SOUND_SPRITE_MUSIC);

	ERAPI_LayerHide(3);

	mode = 0;
	return;
}

s8 bg_scroll[3][3] = {
	{8,-55,80},
	{21,0,40},
	{60,45,20}
};
static inline void mode_run_sound()
{
	// Load and configure backgrounds
	for (u8 i = 3 ; i ; --i)
	{
		ERAPI_LoadBackgroundSystem( i, bg_scroll[i-1][0]);
		ERAPI_SetBackgroundOffset(i,0,bg_scroll[i-1][1]);
		ERAPI_SetBackgroundAutoScroll(i,bg_scroll[i-1][2],0);
		ERAPI_LayerShow(i);
	}

	hud_chooser(MODE_SOUND, 1);


	ERAPI_FadeIn( FADE);
	ERAPI_RenderFrame( FADE);

	// loop
	quit = 0;
	u8 timer = 0;
	u8 show = 1;
	int dir = 0;
	while (quit == 0)
	{
		// read keys
		if (timer)
		{
			timer--;
		}else{
			if (ERAPI_GetKeyStateRaw() & ERAPI_KEY_A)
			{
				ERAPI_PlaySoundSystem(sound);
				timer=debounce;
			}

			dir = hud_chooser(MODE_SOUND, show);
			if (dir != 0)
			{
				ERAPI_SoundPause(sound);
				choose_num(dir, &sound, 0, 884, bad_sound,sizeof(bad_sound));
				ERAPI_PlaySoundSystem(sound);
				timer=debounce;
			}
		}
		// quit
		if (ERAPI_GetKeyStateRaw() & ERAPI_KEY_B) quit = 1;
		// render frame
		ERAPI_RenderFrame( 1);
	}

	// Hide everything
	ERAPI_PlaySoundSystem(SOUND_EXIT);
	ERAPI_FadeOut(FADE);
	ERAPI_RenderFrame( FADE);
	hud_chooser(MODE_BACKGROUND, 0);


	// Unload and unconfigure backgrounds
	for (u8 i = 3 ; i ; --i)
	{
		ERAPI_SetBackgroundOffset(i,0,0);
		ERAPI_SetBackgroundAutoScroll(i,0,0);
		ERAPI_LayerHide(i);
	}

	mode = 0;
	return;
}

static inline void background()
{
	// init
	ERAPI_LoadBackgroundSystem( 3, back);
	ERAPI_LayerShow(3);

	// background
	u16 back_pos_x=0;
	u16 back_pos_y=0;
	ERAPI_SetBackgroundOffset(3,back_pos_x,back_pos_y);

	hud_chooser(MODE_BACKGROUND, 1);


	ERAPI_PlaySoundSystem(SOUND_BACK_MUSIC);
	ERAPI_FadeIn( FADE);
	ERAPI_RenderFrame( FADE);

	// loop
	quit = 0;
	u8 timer = 0;
	u8 show = 1;
	int dir = 0;
	while (quit == 0)
	{
		// read keys
		if (timer)
		{
			timer--;
		}else{
			if (ERAPI_GetKeyStateRaw() & ERAPI_KEY_A)
			{
				show = !show;
				timer=debounce;
			}

			dir = hud_chooser(MODE_BACKGROUND, show);
			if (dir != 0){
				choose_num(dir, &back, 0, 101, bad_back,sizeof(bad_back));
				ERAPI_LoadBackgroundSystem( 3, back);
				timer=debounce;
			}

			if (ERAPI_GetKeyStateRaw() & ERAPI_KEY_LEFT) back_pos_x--;
			if (ERAPI_GetKeyStateRaw() & ERAPI_KEY_RIGHT) back_pos_x++;
			if (ERAPI_GetKeyStateRaw() & ERAPI_KEY_UP) back_pos_y--;
			if (ERAPI_GetKeyStateRaw() & ERAPI_KEY_DOWN) back_pos_y++;
			ERAPI_SetBackgroundOffset(3,back_pos_x,back_pos_y);
		}
		// quit
		if (ERAPI_GetKeyStateRaw() & ERAPI_KEY_B) quit = 1;
		// render frame
		ERAPI_RenderFrame( 1);
	}

	// Hide everything
	ERAPI_PlaySoundSystem(SOUND_EXIT);
	ERAPI_FadeOut(FADE);
	ERAPI_RenderFrame( FADE);
	hud_chooser(MODE_BACKGROUND, 0);
	ERAPI_SoundPause(SOUND_BACK_MUSIC);

	ERAPI_LayerHide(3);

	mode = 0;
	return;
}

static inline void menu ()
{
	//ERAPI_ClearRegion(region_menu);
	ERAPI_SetTextColor( region_menu, 0x2, 0);
	ERAPI_DrawText( region_menu, 0, 0, "System Assets");

	ERAPI_DrawText( region_menu, 0, 22, "Backgrounds");
	ERAPI_DrawText( region_menu, 0, 36, "Sounds");
	ERAPI_DrawText( region_menu, 0, 50, "Sprites");
	ERAPI_SetSpriteVisible(handle_arrow_r,1);
	ERAPI_SetSpritePos( handle_arrow_r, 60, 60+(main_menu * 14));

	ERAPI_LayerShow(1);
	ERAPI_LoadBackgroundSystem( 1, 24);
	ERAPI_SetBackgroundAutoScroll(1,0,0x40);

	ERAPI_PlaySoundSystem(SOUND_MENU_MUSIC);
	ERAPI_FadeIn( FADE);
	ERAPI_RenderFrame( FADE);


	// loop
	quit = 0;
	u8 timer = 0;
	while (quit == 0)
	{
		// read keys
		if (timer)
		{
			timer--;
		}else{
			if (ERAPI_GetKeyStateRaw() & ERAPI_KEY_A)
			{
				ERAPI_SetSpriteFrame( handle_arrow_r,1);
				ERAPI_SpriteAutoAnimate(handle_arrow_r,debounce,12);
				mode = main_menu;
				ERAPI_PlaySoundSystem(86);
				quit = 1;
			}

			if (ERAPI_GetKeyStateRaw() & ERAPI_KEY_UP)
			{
				main_menu--;
				if (main_menu<1) main_menu=3;
				ERAPI_PlaySoundSystem(SOUND_MENUMOVE);
				timer=debounce;
			}
			if (ERAPI_GetKeyStateRaw() & ERAPI_KEY_DOWN)
			{
				main_menu++;
				if (main_menu>3) main_menu=1;
				ERAPI_PlaySoundSystem(SOUND_MENUMOVE_DOWN);
				timer=debounce;
			}
			// quit
			if (ERAPI_GetKeyStateRaw() & ERAPI_KEY_B)
			{
				ERAPI_PlaySoundSystem(68);
				quit = 1;
				sysexit= 1;
			}
		}
		ERAPI_SetSpritePos( handle_arrow_r, 60, 60+(main_menu * 14));
		ERAPI_RenderFrame( 1);
	}
	ERAPI_SoundPause(SOUND_MENU_MUSIC);
	ERAPI_FadeOut(FADE);
	ERAPI_RenderFrame( FADE);
	ERAPI_LayerHide(1);
	ERAPI_ClearRegion(region_menu);
	ERAPI_SetSpriteVisible(handle_arrow_r,0);
	ERAPI_SetSpritePos( handle_arrow_r, 220, 20);
	ERAPI_SetBackgroundAutoScroll(1,0,0);

	return;
}

int main()
{
	// Init
	ERAPI_InitMemory( (ERAPI_RAM_END - (u32)__end) >> 10);
	ERAPI_SetBackgroundMode( 0);
	// create sprites
	handle_arrow_r = ERAPI_SpriteCreateCustom( 0, &arrow_sprite);
	ERAPI_SetSpritePos( handle_arrow_r, 220, 20);

	handle_arrow_l = ERAPI_SpriteCreateCustom( 0, &arrow_sprite);
	ERAPI_SetSpritePos( handle_arrow_l, 20, 20);
	ERAPI_SpriteMirrorToggle( handle_arrow_l,0x01);

	// palette
	ERAPI_SetBackgroundPalette( &palette[0], 0x00, 0x02);

	// Create regions for text display
	region_menu = ERAPI_CreateRegion( 0, 0, 0xa, 0x06, 0x0A, 0x08);
	chooser = ERAPI_CreateRegion(0,0,0x0a, 0x02,0xc, 0x01);
	ERAPI_SetTextColor( chooser, 0x01, 0x00);
	hud_chooser(MODE_MENU, 0);

	// Main Loop
	while (sysexit == 0)
	{
		// Switch menus
		switch(mode)
		{
			case MODE_MENU: 	menu (region_menu); break;
			case MODE_BACKGROUND: 	background (chooser); break;
			case MODE_SOUND: 	mode_run_sound (chooser); break;
			case MODE_SPRITE: 	mode_run_sprite (chooser); break;
		}
	}

	// exit
	ERAPI_SpriteFree( handle_arrow_l);
	ERAPI_SpriteFree( handle_arrow_r);
	return ERAPI_EXIT_TO_MENU;
}
