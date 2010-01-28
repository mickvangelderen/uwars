#pragma once

#include <PA9.h>

#define __STR2__(x) #x
#define __STR1__(x) __STR2__(x)
#define __LOC__ __FILE__ "("__STR1__(__LINE__)") : Warning Msg: "

#define PA_WaitForAnything() {PA_WaitForVBL();PA_WaitFor(Pad.Newpress.Anykey||Stylus.Newpress);PA_WaitForVBL();}

#define fixed(x)	((x)<<8)	//fixed point
#define dfixed(x)	((x)<<16) //double fixed point
#define unfix(x)	((x)>>8)  //unfix
#define dunfix(x)	((x)>>16) //double unfix

//--screen brightness--
void SetFadeType(s8 type);	//-1 for black, 1 for white
void SetFadeSpeed(u32 speed); //fixed point number
void FadeIn();
void FadeOut();

void ScreenOut(); //instant fadeout both screens
void ScreenOut(u8 screen);
void ScreenIn();  //instant fadein both screens
void ScreenIn(u8 screen);

//--sprites and palettes--
//u8 GetSpriteId(bool screen);
//bool FreeSpriteId(bool screen, u16 id);

//u8 GetPalId(bool screen);
//bool FreePalId(bool screen, u16 id);

u8 CreateSpriteSimple(u8 screen, void* obj_data, u8 obj_shape, u8 obj_size, u8 palette);
u8 CreateSprite(u8 screen, void* obj_data, u8 obj_shape, u8 obj_size, u8 color_mode, u8 palette, s16 x, s16 y);
u8 CreateSpriteEx(u8 screen, void* obj_data, u8 obj_shape, u8 obj_size, u8 color_mode, u8 palette, u8 obj_mode, u8 mosaic, u8 hflip, u8 vflip, u8 prio, u8 dblsize, s16 x, s16 y);


void DestroySprite(u8 screen, u8 palette);
inline void RemoveSprite(u8 screen, u8 spriteId){
//#pragma message(__LOC__"Deprecated function name, use DestroySprite(screen, spriteId) instead")
	DestroySprite(screen, spriteId);
}

u8 CreateSpritePalette(u8 screen, void * const palette);
inline u8 LoadSpritePal(u8 screen, void * const palette){
//#pragma message(__LOC__"Deprecated function name, use CreateSpritePalette(screen, palette) instead")
	return CreateSpritePalette(screen, palette);
}

void DestroySpritePalette(u8 screen, u8 paletteId);
inline void RemovePal(u8 screen, u8 paletteId){
//#pragma message(__LOC__"Deprecated function name, use DestroySpritePalette(screen, paletteId) instead")
	DestroySpritePalette(screen, paletteId);
}

//--misc--
void ErrorMsg(u8 screen, const char * const text);
void GetName(char * const n, const u16 maxNameLen);

void ClearText();
inline void ClearText(u8 screen);

s32 abs(s32 n);
bool IsRectInScreen(s32 x, s32 y, s32 width, s32 height);

