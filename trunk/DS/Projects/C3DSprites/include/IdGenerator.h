#pragma once
#include <PA9.h>
#include "DSspecs.h"


u16 GetNumberOf3DSprites();
u16 GetNumberOf3DPalettes();
u16 GetNumberOfSprites(u8 screen);
u16 GetNumberOfPalettes(u8 screen);

//Reserves an 3dsprite id, returns MAX_3DSPRITES if it failed
u16 Reserve3DSpriteId();
void Release3DSpriteId(u16 id);

//Reserves an 3dpalette id, returns MAX_3DPALETTES if it failed
u16 Reserve3DPaletteId();
void Release3DPaletteId(u16 id);

//Reserves an sprite id, returns MAX_SPRITES if it failed
u16 ReserveSpriteId(u8 screen);
void ReleaseSpriteId(u8 screen, u16 id);

//Reserves an palette id, returns MAX_PALETTES if it failed
u16 ReservePaletteId(u8 screen);
void ReleasePaletteId(u8 screen, u16 id);

