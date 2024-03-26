#ifndef ERAPI_H
#define ERAPI_H

#include "def.h"

#define ERAPI_KEY_A      0x0001
#define ERAPI_KEY_B      0x0002
#define ERAPI_KEY_SELECT 0x0004
#define ERAPI_KEY_START  0x0008
#define ERAPI_KEY_RIGHT  0x0010
#define ERAPI_KEY_LEFT   0x0020
#define ERAPI_KEY_UP     0x0040
#define ERAPI_KEY_DOWN   0x0080
#define ERAPI_KEY_R      0x0100
#define ERAPI_KEY_L      0x0200

#define ERAPI_RAM_START 0x02000000
#define ERAPI_RAM_END   0x02027800

#define ERAPI_EXIT_RESTART 0
#define ERAPI_EXIT_TO_MENU 2

typedef u32 (*FUNC_U32_X1)( u32 r0);
typedef u32 (*FUNC_U32_X2)( u32 r0, u32 r1);
typedef u32 (*FUNC_U32_X3)( u32 r0, u32 r1, u32 r2);
typedef u32 (*FUNC_U32_X4)( u32 r0, u32 r1, u32 r2, u32 r3);
typedef u32 (*FUNC_U32_X5)( u32 r0, u32 r1, u32 r2, u32 r3, u32 r4);
typedef u32 (*FUNC_U32_X6)( u32 r0, u32 r1, u32 r2, u32 r3, u32 r4, u32 r5);

#define ERAPI_FUNC_X1 ((FUNC_U32_X1)*(u32*)0x030075FC)
#define ERAPI_FUNC_X2 ((FUNC_U32_X2)*(u32*)0x030075FC)
#define ERAPI_FUNC_X3 ((FUNC_U32_X3)*(u32*)0x030075FC)
#define ERAPI_FUNC_X4 ((FUNC_U32_X4)*(u32*)0x030075FC)
#define ERAPI_FUNC_X5 ((FUNC_U32_X5)*(u32*)0x030075FC)
#define ERAPI_FUNC_X6 ((FUNC_U32_X6)*(u32*)0x030075FC)

typedef u8 ERAPI_HANDLE_REGION;
typedef u16 ERAPI_HANDLE_SPRITE;

typedef struct _ERAPI_SPRITE ERAPI_SPRITE;
struct _ERAPI_SPRITE
{
  u8* data_gfx; // Tile data:       .... .... .... .... ....
  u8* data_pal; // Palette data:    .... .... .... .... ....
  u8 width;     // Width in tiles:  0x08 0x06 0x06 0x02 0x04
  u8 height;    // Height in tiles: 0x08 0x06 0x02 0x02 0x04
  u8 frames;    // Frame count:     0x04 0x04 0x02 0x02 0x04
  u8 unk1;      // Unknown:         0x01 0x01 0x01 0x01 0x01
  u8 hitbox_w;  // Hitbox width:    0x08 0x00 0x00 0x00 0x00
  u8 hitbox_h;  // Hitbox height:   0x08 0x00 0x00 0x00 0x00
  u8 unk4;      // Unknown frames:  0x03 0x04 0x02 0x02 0x04
};


typedef struct _ERAPI_BACKGROUND ERAPI_BACKGROUND;
struct _ERAPI_BACKGROUND
{
  u8* data_gfx; // Tile data
  u8* data_pal; // Palette data
  u8* data_map; // Map data
  u16 tiles;    // Tile count
  u16 palettes; // Palette count
};

//#define ERAPI_STUB

#ifndef ERAPI_STUB
#define ERAPI_Div(a,b)                                    ERAPI_FUNC_X3( 0x103, a, b)
#define ERAPI_Mod(a,b)                                    ERAPI_FUNC_X3( 0x104, a, b)
#define ERAPI_PlaySoundSystem(a)                          ERAPI_FUNC_X2( 0x105, a)
//            a: Sound Index
#define ERAPI_0106(a,b)                                   ERAPI_FUNC_X3( 0x106, a, b)
#define ERAPI_Rand()                                      ERAPI_FUNC_X1( 0x107)
#define ERAPI_SetSoundVolume(a,b)                         ERAPI_FUNC_X3( 0x108, a, b)
#define ERAPI_0109(a,b,c)                                 ERAPI_FUNC_X4( 0x109, a, b, c)
#define ERAPI_010A(a,b)                                   ERAPI_FUNC_X2( 0x10A, a, b)
#define ERAPI_Set0400XXXX(a,b)                            ERAPI_FUNC_X3( 0x10B, a, b)
#define ERAPI_Get0400XXXX(a)                              ERAPI_FUNC_X2( 0x10C, a)
#define ERAPI_RandMax(a)                                  ERAPI_FUNC_X2( 0x112, a)
#define ERAPI_SetSoundSpeed(a,b)                          ERAPI_FUNC_X3( 0x113, a, b)
#define ERAPI_SoundPause(a)                               ERAPI_FUNC_X2( 0x116, a)
//            a: Sound Index
#define ERAPI_SoundResume(a)                              ERAPI_FUNC_X2( 0x117, a)
//            a: Sound Index
#define ERAPI_PlaySoundSystemEx(a,b)                      ERAPI_FUNC_X3( 0x118, a, b)
#define ERAPI_IsSoundPlaying(a,b)                         ERAPI_FUNC_X3( 0x119, a, b)
#define ERAPI_GetExitCount()                              ERAPI_FUNC_X1( 0x11D)
#define ERAPI_PlaySoundCustom(a,b)                        ERAPI_FUNC_X3( 0x12F, (u32)a, b)
#define ERAPI_PlaySoundCustomEx(a,b,c)                    ERAPI_FUNC_X4( 0x131, (u32)a, b, c)
#define ERAPI_FadeIn(a)                                   ERAPI_FUNC_X2( 0x200, a)
//            a: Frame count for animation
#define ERAPI_FadeOut(a)                                  ERAPI_FUNC_X2( 0x201, a)
//            a: Frame count for animation
#define ERAPI_LoadBackgroundSystem(a,b)                   ERAPI_FUNC_X3( 0x210, b, a)
//            a: Background Layer
//            b: Background Index
#define ERAPI_SetBackgroundOffset(a,b,c)                  ERAPI_FUNC_X4( 0x211, a, b, c)
//            a: Background Layer
//            b: X Offset
//            b: Y Offset
#define ERAPI_SetBackgroundAutoScroll(a,b,c)              ERAPI_FUNC_X4( 0x212, a, b, c)
//            a: Background Layer
//            b: X Offset
//            b: Y Offset
#define ERAPI_SetBackgroundMirrorToggle(a,b)              ERAPI_FUNC_X3( 0x213, a, b)
//            a: Background Layer
//            b: Bitmask{0x01: Horizontal . 0x02: Vertical}  
#define ERAPI_SetBackgroundMode(a)                        ERAPI_FUNC_X2( 0x219, a)
#define ERAPI_LayerShow(a)                                ERAPI_FUNC_X2( 0x220, a)
//            a: Background Layer
#define ERAPI_LayerHide(a)                                ERAPI_FUNC_X2( 0x221, a)
//            a: Background Layer
#define ERAPI_LoadBackgroundCustom(a,b)                   ERAPI_FUNC_X3( 0x22D, a, (u32)b)
#define ERAPI_SpriteCreateSystem(a,b)                     ERAPI_FUNC_X3( 0x230, a, b)
#define ERAPI_SpriteFree(a)                               ERAPI_FUNC_X2( 0x231, a)
#define ERAPI_SetSpritePos(a,b,c)                         ERAPI_FUNC_X4( 0x232, a, b, c)
#define ERAPI_SpriteFrameNext(a)                          ERAPI_FUNC_X2( 0x234, a)
//            a: Sprite Handle
#define ERAPI_SpriteFramePrev(a)                          ERAPI_FUNC_X2( 0x235, a)
//            a: Sprite Handle
#define ERAPI_SetSpriteFrame(a,b)                         ERAPI_FUNC_X3( 0x236, a, b)
//            a: Sprite Handle
//            b: Sprite Frame Index
#define ERAPI_SetSpriteAutoMove(a,b,c)                    ERAPI_FUNC_X4( 0x239, a, b, c)
#define ERAPI_SpriteAutoAnimate(a,b,c)                    ERAPI_FUNC_X4( 0x23C, a, b, c)
//            a: Sprite Handle
//            b: Animation speed in frames
//            c: 0 = Start Animating Forever
//               1 = Stop Animation
//               2 > Number of frames to animate for -2 (ex. 12 animates for 10 frames)
#define ERAPI_SpriteAutoRotateUntilAngle(a,b,c)           ERAPI_FUNC_X4( 0x23E, a, b, c)
#define ERAPI_SpriteAutoRotateByAngle(a,b,c)              ERAPI_FUNC_X4( 0x23F, a, b, c)
#define ERAPI_SpriteAutoRotateByTime(a,b,c)               ERAPI_FUNC_X4( 0x240, a, b, c)
#define ERAPI_SetSpriteAutoMoveHorizontal(a,b)            ERAPI_FUNC_X3( 0x242, a, b)
#define ERAPI_SetSpriteAutoMoveVertical(a,b)              ERAPI_FUNC_X3( 0x243, a, b)
#define ERAPI_SpriteDrawOnBackground(a,b,c)   	          ERAPI_FUNC_X4
//            a: Sprite Handle
//            b: Background Layer
//            c: Sprite Palette pointer
#define ERAPI_SpriteShow(a)			          ERAPI_FUNC_X2( 0x246, a)
//            a: Sprite Handle
#define ERAPI_SpriteHide(a)			          ERAPI_FUNC_X2( 0x247, a)
//            a: Sprite Handle
#define ERAPI_SpriteMirrorToggle(a,b)          	          ERAPI_FUNC_X3( 0x248, a, b)
//            a: Sprite Handle
//            b: Bitmask{0x01: Horizontal . 0x02: Vertical}  
#define ERAPI_GetSpritePos(a,b,c)                         ERAPI_FUNC_X4( 0x24C, a, (u32)b, (u32)c)
#define ERAPI_SpriteCreateCustom(a,b)                     ERAPI_FUNC_X3( 0x24D, a, (u32)b)
#define ERAPI_SpriteMove(a,b,c)                           ERAPI_FUNC_X4( 0x257, a, b, c)
#define ERAPI_SpriteAutoScaleUntilSize(a,b,c)             ERAPI_FUNC_X4( 0x25B, a, b, c)
#define ERAPI_SpriteAutoScaleBySize(a,b,c)     	          ERAPI_FUNC_X4( 0x25C, a, b, c)
#define ERAPI_HANDLE_SpriteAutoScaleWidthUntilSize(a,b,c) ERAPI_FUNC_X4( 0x25D, a, b, c)
#define ERAPI_SpriteAutoScaleHeightBySize(a,b,c)	  ERAPI_FUNC_X4( 0x25E, a, b, c)
#define ERAPI_SetSpriteVisible(a,b)                       ERAPI_FUNC_X3( 0x266, a, b)
#define ERAPI_SetBackgroundPalette(a,b,c)                 ERAPI_FUNC_X4( 0x27E, (u32)a, b, c)
#define ERAPI_GetBackgroundPalette(a,b,c)                 ERAPI_FUNC_X4( 0x27F, (u32)a, b, c)
#define ERAPI_SetSpritePalette(a,b,c)                     ERAPI_FUNC_X4( 0x280, a, b, c)
#define ERAPI_GetSpritePalette(a,b,c)                     ERAPI_FUNC_X4( 0x281, (u32)a, b, c)
#define ERAPI_ClearPalette()                              ERAPI_FUNC_X1( 0x282)
#define ERAPI_CreateRegion(a,b,c,d,e,f)                   ERAPI_FUNC_X4( 0x290, (a << 8) | b, (c << 8) | d, (e << 8) | f)
//            a: 0,2=normal, 1,4,5,6,10=nothing, 3=messes with background tile memory
//            b: 0= no change, 4 = text grey if color is set, * text is black
//            c: x position in tiles
//            d: y position in tiles
//            e: width in tiles
//            f: height in tiles
#define ERAPI_SetRegionColor(a,b)                         ERAPI_FUNC_X3( 0x291, a, b)
//            a: Region
//            b: Color code
#define ERAPI_ClearRegion(a)                              ERAPI_FUNC_X2( 0x292, a)
//            a: Region
#define ERAPI_SetPixel(a,b,c)                             ERAPI_FUNC_X3( 0x293, a, (b << 8) | c)
#define ERAPI_GetPixel(a,b,c)                             ERAPI_FUNC_X3( 0x294, a, (b << 8) | c)
#define ERAPI_DrawLine(a,b,c,d,e,f)                       ERAPI_FUNC_X5( 0x295, a, (b << 8) | c, (d << 8) | e,f)
//            a: Region
//            b: start x position in pixels
//            c: start y position in pixels
//            d: start x position in pixels
//            e: start y position in pixels
//            f: frame count to draw line over

#define ERAPI_DrawRect(a,b,c,d,e,f)                       ERAPI_FUNC_X4( 0x296, (a << 8) | f, (b << 8) | c, (d << 8) | e)
#define ERAPI_SetTextColor(a,b,c)                         ERAPI_FUNC_X3( 0x298, a, (b << 8) | c)
#define ERAPI_DrawText(a,b,c,d)                           ERAPI_FUNC_X4( 0x299, a, (b << 8) | c, (u32)d)
//            a: Region
//            b: x position in pixels in region
//            c: y position in pixels in region
//            d: Text
#define ERAPI_SetTextSize(a,b)                            ERAPI_FUNC_X2( 0x29A, (a << 8) | b)
#define ERAPI_SetBackgroundModeRaw(a)                     ERAPI_FUNC_X2( 0x29F, (u32)a)
#define ERAPI_02B5(a,b)                                   ERAPI_FUNC_X3( 0x2B5, a, b)
#define ERAPI_GetTextWidth(a,b)                           ERAPI_FUNC_X3( 0x2C0, a, (u32)b)
//            a: Region
//            b: Text
//            Return: Pixel count
#define ERAPI_GetTextWidthEx(a,b,c)                       ERAPI_FUNC_X3( 0x2C1, (a << 8) | c, (u32)b)
#define ERAPI_02C2(a)                                     ERAPI_FUNC_X2( 0x2C2, (u32)a)
#define ERAPI_02C3(a)                                     ERAPI_FUNC_X2( 0x2C3, a)
#define ERAPI_02DD(a,b)                                   ERAPI_FUNC_X3( 0x2DD, a, b)
#define ERAPI_FlashWriteSectorSingle(a,b)	          ERAPI_FUNC_X3( 0x2DE, a, (u32)b)
//            a: Sector index
//            a: Pointer to read data from
#define ERAPI_FlashReadSectorSingle(a,b)                  ERAPI_FUNC_X3( 0x2DF, a, (u32)b)
//            a: Sector index
//            a: Pointer to read data into
#define ERAPI_SoftReset()                                 ERAPI_FUNC_X1( 0x2E0)
#define ERAPI_InitMemory(a)                               ERAPI_FUNC_X2( 0x2EA, a)
#define ERAPI_FlashWriteSectorMulti(a,b,c)                ERAPI_FUNC_X4( 0x2ED, a, b, c)
#define ERAPI_FlashReadPart(a,b,c)                        ERAPI_FUNC_X4( 0x2EE, a, (u32)b, c)
#define ERAPI_RandInit(a)                                 ERAPI_FUNC_X2( 0x2F1, a)
#define ERAPI_RenderFrame(a)                              ERAPI_FUNC_X2( 0x300, a)
//            a: Number of frames to render before returning
#define ERAPI_GetKeyStateSticky()                         ERAPI_FUNC_X1( 0x301)
#define ERAPI_GetKeyStateRaw()                            ERAPI_FUNC_X1( 0x302)

#define ERAPI_CalcDistanceBetweenPoints(a,b,c,d)          ERAPI_FUNC_X5( 0x2B2, a, b, c, d)

#define ERAPI_CalcDistanceBetweenSprites(a,b)             ERAPI_FUNC_X3( 0x2AB, a, b)
//            a: u16 Sprite Handle
//            b: u16 Sprite Handle
//            Return: u16 Distance

#define ERAPI_CalcAngleBetweenSprites(a,b)                ERAPI_FUNC_X3( 0x2AC, a, b)
//            a: u16 Sprite Handle
//            b: u16 Sprite Handle
//            Return: u8 Angle (0-255)
#define ERAPI_ScanDotCode(a)                              ERAPI_FUNC_X2( 0x2C2, (u32)a)
//            a: u8* Data (minimum 0x81C bytes)
//            Return: u8 Status (0: ok, 1: read error, 4: timeout, 5: wrong region)

#define ERAPI_SpriteSetPosAnimatedDuration(a,b,c,d)       ERAPI_FUNC_X5( 0x23B, a, b, c, d)
//            a: u16 Sprite Handle
//            b: u16 X Position
//            c: u16 Y Position
//            d: u8 Duration (fixed duration, variable speed, depends on distance)

#define ERAPI_SpriteSetPosAnimatedSpeed(a,b,c,d)          ERAPI_FUNC_X5( 0x2DA, a, b, c, d)
//            a: u16 Sprite Handle
//            b: u16 X Position
//            c: u16 Y Position
//            d: u16 Speed (fixed speed, variable duration, depends on distance)

#define ERAPI_SpriteSetType(a,b)                          ERAPI_FUNC_X3( 0x268, a, b)
//            a: u16 Sprite Handle
//            b: u8 Type

#define ERAPI_SpriteGetType(a)                            ERAPI_FUNC_X2( 0x269, a)
//            a: u16 Sprite Handle
//            Return: u8 Type

#define ERAPI_SpriteFindClosestSprite(a,b,c)              ERAPI_FUNC_X4( 0x2AA, a, b, (u32)c)
//            a: u16 Sprite Handle
//            b: u8 Type
//            c: u16* Distance (0 if no sprites of specified type exist)
//            Return: u16 Sprite Handle (187 if no sprites of specified type exist)

#define ERAPI_Sin(a,b)                                    ERAPI_FUNC_X3( 0x2B5, a, b)
//            a: u8 Angle
//            d: u16 Multiply
//            Return: s16 Result

#define ERAPI_Cos(a,b)                                    ERAPI_FUNC_X3( 0x2B6, a, b)
//            a: u8 Angle
//            d: u16 Multiply
//            Return: s16 Result

#define ERAPI_SpriteSetFrameBank(a,b)                     ERAPI_FUNC_X3( 0x237, a, b)
//            a: u16 Sprite Handle
//            b: u8 Bank Index
// Pokèmon system sprites have 3 banks, 0 = facing left, 1 = facing front, 2 = facing right.
// typedef struct _ERAPI_SPRITE ERAPI_SPRITE;

#define ERAPI_SpriteSetHitboxSize(a,b,c)                  ERAPI_FUNC_X4( 0x258, a, ((b)>>1), ((c)>>1))
//            a: u16 Sprite Handle
//            b: u8 Width
//            c: u8 Height

#define ERAPI_SpriteCheckCollision(a,b)                   ERAPI_FUNC_X3( 0x241, a, b)
//            a: u16 Sprite Handle
//            b: u16 Sprite Handle
//            Return: u8 Collision (0: no, 1: yes)

#define ERAPI_SpriteFindCollisions(a,b,c)                 ERAPI_FUNC_X4( 0x2E5, a, b, (u32)c)
//            a: u16 Sprite Handle
//            b: u8 Type
//            c: u32* Sprite Handles (not u16*)
//            Return: u8 Number of sprites

#endif

#endif

