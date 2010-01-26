#pragma once

///////////////////////////////////
//Button System by Prozix		 //
//Stable version 1.0			 //
//Release date: 9 Jan 2010, 00:03//
///////////////////////////////////

#include <PA9.h>

class buttonGfx{
private:
	u8 m_screen;
	u8 m_palId;
	void* m_sprites[3];
public:
	buttonGfx(u8 screen, void* spriteLeft, void* spriteMiddle, void* spriteRight, void* paletteName);
	~buttonGfx();
	u8 GetScreen();
	void* GetButtonLeft();
	void* GetButtonMiddle();
	void* GetButtonRight();
	u8 GetPalId();
};

class button{
private:
	u8 m_x, m_y;
	char* m_text;
	buttonGfx* m_gfx;

	u8* m_spriteIds;
	u8 m_spriteIdsLength;
	
public:
	button();
	button(u8 x, u8 y, const char * const text, u8 size, buttonGfx* buttonGraphics);
	~button();

	void Set(u8 x, u8 y, const char * const text, u8 size, buttonGfx* buttonGraphics);
	void Reset();

	void RefreshText();

	bool IsTouched();
};

#include "BTNlib.cpp"
