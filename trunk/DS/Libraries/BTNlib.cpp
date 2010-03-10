///////////////////////////////////
//Button System by Prozix		 //
//Stable version 1.0			 //
//Release date: 9 Jan 2010, 00:03//
///////////////////////////////////

#include <PA9.h>

#include "DSspecs.h"
#include "STDfunctions.h"
 
#include <cstring>
#include "all_gfx.h"

#include "BTNlib.h"

#define BUTTON_SPRITE_WIDTH			16
#define BUTTON_SPRITE_HEIGHT		24
#define MIN_SPRITES_PER_BUTTON		2 //no halve buttons
const u8 MAX_SPRITES_PER_BUTTON =	(DS_WIDTH/BUTTON_SPRITE_WIDTH); //256/16
#define MIN_TEXTLENGTH				2 //on the L and R buttons fit two characters
const u16 MAX_TEXTLENGTH =			(MAX_SPRITES_PER_BUTTON*2 - 2);

/* buttonGfx class */
buttonGfx::buttonGfx(u8 screen, void* spriteLeft, void* spriteMiddle, void* spriteRight, void* paletteName): m_screen(screen) {
	m_sprites[0] = spriteLeft;
	m_sprites[1] = spriteMiddle;
	m_sprites[2] = spriteRight;
	m_palId = LoadSpritePal(screen, paletteName);
}
buttonGfx::~buttonGfx(){
	RemovePal(m_screen, m_palId);
}
u8 buttonGfx::GetScreen(){ 
	return m_screen;
}
void* buttonGfx::GetButtonLeft(){ 
	return m_sprites[0];
}
void* buttonGfx::GetButtonMiddle(){
	return m_sprites[1];
}
void* buttonGfx::GetButtonRight(){
	return m_sprites[2];
}
u8 buttonGfx::GetPalId(){
	return m_palId;
}


/* button class */

button::button(): m_x(0), m_y(0), m_text(NULL), m_gfx(NULL), m_spriteIds(NULL), m_spriteIdsLength(0){}
button::button(u8 x, u8 y, const char * const text, u8 size, buttonGfx* buttonGraphics):
	m_x(0), m_y(0), m_text(NULL), m_gfx(NULL), m_spriteIds(NULL), m_spriteIdsLength(0){
	Set(x, y, text, size, buttonGraphics);
}

button::~button(){
	delete[] m_text;
	for(u8 i=0; i<m_spriteIdsLength; i++) {
		PA_OutputSimpleText(m_gfx->GetScreen(), m_x+(i<<1), m_y+1, "  ");
		RemoveSprite(m_gfx->GetScreen(), m_spriteIds[i]);
	}
	delete[] m_spriteIds;
}

void button::Set(u8 x, u8 y, const char * const text, u8 size, buttonGfx* buttonGraphics){
	Reset();

	m_x = x;
	m_y = y;
	m_gfx = buttonGraphics;

	//--text--
	u8 textSize = (size == 0) ? strlen(text) : (size);
	if(textSize < MIN_TEXTLENGTH) textSize = MIN_TEXTLENGTH;
	else if(textSize > MAX_TEXTLENGTH) textSize = MAX_TEXTLENGTH;
	
	m_text = new char[textSize+1];
	strncpy(m_text, text, textSize);
	m_text[textSize] = '\0';
	
	//--Sprites--
	m_spriteIdsLength = ((textSize+(textSize&1))>>1)+1;
	if(m_spriteIdsLength < MIN_SPRITES_PER_BUTTON) m_spriteIdsLength = MIN_SPRITES_PER_BUTTON;
	else if(m_spriteIdsLength > MAX_SPRITES_PER_BUTTON) m_spriteIdsLength = MAX_SPRITES_PER_BUTTON;
	
	m_spriteIds = new u8[m_spriteIdsLength];
	
    for(u8 i=0; i<m_spriteIdsLength; i++){
		if(i==0)							
			m_spriteIds[i] = CreateSprite(m_gfx->GetScreen(), m_gfx->GetButtonLeft(), OBJ_SIZE_16X32, 1, m_gfx->GetPalId(), (m_x<<3), (m_y<<3));
		else if(i+1 == m_spriteIdsLength)	
			m_spriteIds[i] = CreateSprite(m_gfx->GetScreen(), m_gfx->GetButtonRight(), OBJ_SIZE_16X32, 1, m_gfx->GetPalId(), ((m_x+2*i)<<3), (m_y<<3));
		else								
			m_spriteIds[i] = CreateSprite(m_gfx->GetScreen(), m_gfx->GetButtonMiddle(), OBJ_SIZE_16X32, 1, m_gfx->GetPalId(), ((m_x+2*i)<<3), (m_y<<3));

		PA_SetSpritePrio(m_gfx->GetScreen(), m_spriteIds[i], BG_TEXT+1);
    }

	RefreshText();
}

void button::Reset(){
	if(m_text){
		delete[] m_text;
		m_text = NULL;
	}

	if(m_spriteIds){
		for(u8 i=0; i<m_spriteIdsLength; i++) {
			PA_OutputSimpleText(m_gfx->GetScreen(), m_x+(i<<1), m_y+1, "  ");
			RemoveSprite(m_gfx->GetScreen(), m_spriteIds[i]);
		}
		delete[] m_spriteIds;
		m_spriteIds = NULL;
	}
	m_spriteIdsLength = 0;

	m_x = 0;
	m_y = 0;

	m_gfx = NULL;
}

void button::RefreshText(){
	if(m_text) PA_OutputText(m_gfx->GetScreen(), m_x+1, m_y+1, "%s", m_text);
}

bool button::IsTouched(){	
	return StylusOn() && Stylus.Newpress;
}

bool button::StylusOn(){
	if(	m_gfx->GetScreen()==DS_BOTTOM &&
		m_spriteIds &&
		Stylus.X >= (m_x<<3) &&
		Stylus.X < ((m_x<<3) + m_spriteIdsLength*BUTTON_SPRITE_WIDTH) &&
		Stylus.Y >= (m_y<<3) && 
		Stylus.Y < (m_y<<3) + BUTTON_SPRITE_HEIGHT ){
		return true;
	}
	return false;
}

