#pragma once

#include <PA9.h>

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

//--misc--
void ErrorMsg(u8 screen, const char * const text);
void GetName(char * const n, const u16 maxNameLen);

void ClearText();
inline void ClearText(u8 screen);

s32 abs(s32 n);
bool IsRectInScreen(s32 x, s32 y, s32 width, s32 height);

