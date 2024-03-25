#include "def.h"
#include "erapi.h"
#include <stdlib.h>

#include "gfx/gfx.h"
#include "map/map.h"
#include "snd/mario_raw.h" // The goal is to later add sound but so far I haven't got ERAPI_PlaySoundCustom to behave predictably

#define MAZE_GRID 8
#define MAZE_WIDTH 32
#define INPUT_DELAY 5

ERAPI_HANDLE_REGION chooser;
extern int __end[];

const u16 palette[] = { 0x0000, 0xFFFF };

u8 sysexit = 0, win = 0;
u32 key;
u16 MAZE_OPEN, MAZE_FINISH;

char print_str[32]="some text";
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

u32 SND_ADDRESS = 0x03006600;
#define SND_MP2K_TRACKEND 0xB1

char test[]="This is a line of text I'm going to save";
u8 snd_tracks[] = {

	0xBE, 0x7F, 0xBC, 0x00,
	0xBB,
		0x32, 0xBD, 0x20, 0x92, 0xD1, 0x4F, 0x7F, 0x83, 0x4C, 0x83, 0xD2, 0x48, 0x8C, 0x43, 0x8C, 0x42, 0x8C, 0xD2, 0x8C, 0x43,
	0x83, 0xB1,

	0xBE, 0x7F, 0xBC, 0x00,
	0xBD,
		0x03, 0xBF, 0x40, 0xEE, 0x30, 0x7F, 0xA0, 0xD2, 0x41, 0x50, 0x8C, 0xD2, 0x8C, 0x42,
	0x83, 0xB1,

	0xBE, 0x7F, 0xBC, 0x00,
	0xBD,
		0x5D, 0xF6, 0x41, 0x60, 0xA7,
	0x83, 0xB1
};
u8 snd_tracks_size = sizeof(snd_tracks);
u8 snd_track_count = 3;
u8 snd_song[];


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

static void buildSong(u8* song, u8 tracks[], u8 tracks_size, u8 tracks_count)
{
	// Allocate memory for song
	// +4 Sequence Header address
	// +8 Sequence Header data
	// +tracks_count*4 Track addresses
	*song = malloc( (tracks_size+12+(tracks_count*4)) * sizeof(u8));
	// Sequence Header Address
	*((u32*) &song[0]) = SND_ADDRESS |  (u32) (tracks_size+4);
	//song[0]=6;
	// Track offset storage
	u32 tracks_addr[tracks_count];
	tracks_addr[0] = 4; // First track address is always the same
	// Copy track data and find address offsets
	u8 tracks_pos = 1;
	for (u8 i = 0; i < tracks_size; ++i)
	{
		// If end of track,
		if (tracks[i] == SND_MP2K_TRACKEND && tracks_pos < tracks_count)
		{
			tracks_addr[tracks_pos] = i+5; // S.H.A. + Next position
			++tracks_pos;
		}
		song[i+4]=tracks[i];
	}
	*((u32*) &song[4+tracks_size]) = tracks_count;
	*((u32*) &song[4+tracks_size+4]) = 0;
	// Build Sequence header track positions
	for (u8 i = 0; i < tracks_count; ++i)
	{
		*((u32*) &song[4+tracks_size+8+(i*4)])=SND_ADDRESS | ((u32) (tracks_addr[i]));
	}
	//*((u32*) &song[0]) = SND_ADDRESS;
}






ERAPI_HANDLE_REGION chooser;
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

	buildSong(&snd_song,snd_tracks,snd_tracks_size,snd_track_count);
	chooser = ERAPI_CreateRegion(0,0,0x0a, 0x02,0xc, 0x01);
	ERAPI_SetTextColor( chooser, 0x01, 0x00);
	citoa(snd_song,print_str,16);
	ERAPI_DrawText( chooser, 0, 0, print_str);

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
					ERAPI_PlaySoundCustom(audio1_raw,audio1_raw_bytes);
					pmove(-1,0);
					delay = INPUT_DELAY;
				}
				if (key & ERAPI_KEY_RIGHT)
				{
					ERAPI_PlaySoundCustom(audio2_raw,audio2_raw_bytes);
					pmove(1,0);
					delay = INPUT_DELAY;
				}
				if (key & ERAPI_KEY_UP)
				{
					ERAPI_PlaySoundCustom(audio3_raw,sizeof(audio3_raw));
					pmove(0,-1);
					delay = INPUT_DELAY;
				}
				if (key & ERAPI_KEY_DOWN)
				{
					snd_song[1]=0x66;
					ERAPI_PlaySoundCustom(snd_song,168
							      //snd_tracks_size+12+(snd_track_count*4)
							      );
					pmove(0,1);
					delay = INPUT_DELAY;
				}
			}
		}
		//++snd_song[0];
		ERAPI_RenderFrame(1);
	}

	// exit
	return ERAPI_EXIT_TO_MENU;
}
