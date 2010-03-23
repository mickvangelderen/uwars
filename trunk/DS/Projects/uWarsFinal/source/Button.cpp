#include <PA9.h>
#include "Button.h"

#include "DSspecs.h"
#include "IdGenerator.h"

#include "Menu.h"

#include <maxmod9.h>
#include "soundbank_bin.h"
#include "soundbank.h"

#define STATE_ALIVE (1<<0)
#define STATE_SELECTED (1<<1)

Button::Button():
    m_screen(0), m_x(0), m_y(0), m_state(0), 
    m_nSprites(0), m_nSpritesMax(0), m_sprites(0), m_spriteIds(0), 
    m_spriteShape(0), m_spriteSize(0), m_spriteColorMode(0), m_spriteWidth(0), m_spriteHeight(0),
    m_action(0){
}

Button::~Button(){
    delete[]m_sprites;
    delete[]m_spriteIds;
}

void Button::Create(u8 screen, s16 x, s16 y, u8 nSprites, u8 shape, u8 size, u8 colorMode, u8 spriteWidth, u8 spriteHeight, u8 (*actionFunc)()){
	m_screen = screen;
	m_x = x; m_y = y;
	m_state = 0;

	m_nSprites = 0;
	m_nSpritesMax = nSprites;
	m_sprites = new void*[nSprites];
	m_spriteIds = new u8[nSprites];

	m_spriteShape = shape;
	m_spriteSize = size;
	m_spriteColorMode = colorMode;
	m_spriteWidth = spriteWidth;
	m_spriteHeight = spriteHeight;

	m_action = actionFunc;
}

void Button::Destroy(){}
void Button::AddButtonPart(void * sprite){
	if(m_nSprites < m_nSpritesMax){
		m_sprites[m_nSprites] = sprite;
		m_nSprites++;
	}
}
void Button::SetSpriteBackground(u8 background){
    for(u8 i=0; i<m_nSprites; i++){
		PA_SetSpritePrio(m_screen, m_spriteIds[i], background);
    }
}   

void Button::Load(u8 paletteId){
	s16 tid;
	for(u8 i=0; i<m_nSprites; i++){
		tid = ReserveSpriteId(m_screen);
		if(tid != MAX_SPRITES){
			m_spriteIds[i] = tid;
			PA_CreateSprite(m_screen, m_spriteIds[i], m_sprites[i], m_spriteShape, m_spriteSize, m_spriteColorMode, paletteId, m_x+i*m_spriteWidth, m_y);
			PA_StartSpriteAnim(m_screen, m_spriteIds[i], 0, 0, 0);
		}
	}
	Deselect();
	Activate();
}

void Button::Unload(){
	for(u8 i=0; i<m_nSprites; i++){
		PA_DeleteSprite(m_screen, m_spriteIds[i]);
		ReleaseSpriteId(m_screen, m_spriteIds[i]);
	}
}
void Button::Select(){
	m_state |= STATE_SELECTED;
	mmEffect(SFX_KLIK);
}
void Button::Deselect(){
	m_state &= ~STATE_SELECTED;
}
bool Button::IsSelected()const{
	return (m_state & STATE_SELECTED);
}
void Button::Activate(){
	m_state |= STATE_ALIVE;
}
void Button::Deactivate(){
	m_state &= ~STATE_ALIVE;
}
bool Button::IsActivated()const{
	return (m_state & STATE_ALIVE);
}

bool Button::IsTouched(){
	return (Stylus.Newpress && Stylus.X >= m_x && Stylus.X < m_x+m_spriteWidth*m_nSprites && Stylus.Y >= m_y && Stylus.Y < m_y + m_spriteHeight);
}

void Button::Update(){
	u8 frame = 2;//inactive
	if(IsActivated()){
		if(IsSelected()) frame = 1; //selected
		else frame = 0;//normal
	}
	for(u8 i=0; i<m_nSprites; i++){
		PA_SetSpriteAnim(m_screen, m_spriteIds[i], frame);
	}
}
u8 Button::Go(){
	if(m_action != NULL){
		return m_action();
	}
	return MENU_STAY;
}

void UpdateButtons(Button * btns, u8 btnNumber){
	for(u8 i=0; i<btnNumber; i++){
		btns[i].Update();
	}
}

void SelectAButton(Button * btns, u8 btnNumber, u8 selectNumber){
	for(u8 j=0; j<btnNumber; j++) btns[j].Deselect();
	btns[selectNumber].Select();
	UpdateButtons(btns, btnNumber);
}

u8 GetSelectedButton(Button * btns, u8 btnNumber){
	for(u8 i=0; i<btnNumber; i++){
		if(btns[i].IsSelected() && btns[i].IsActivated()){
			return i;
		}
	}
	return btnNumber;
}

void ButtonSelectViaPad(Button * btns, u8 btnNumber){
	if(Pad.Newpress.Up){
		s8 i = GetSelectedButton(btns, btnNumber)-1;
		while(!btns[i].IsActivated() && i>=0) i--;
		if(i>=0 && i<btnNumber) SelectAButton(btns, btnNumber, i);
	
	}
	if(Pad.Newpress.Down){
		s8 i = GetSelectedButton(btns, btnNumber)+1;
		while(!btns[i].IsActivated() && i<btnNumber) i++;
		if(i>=0 && i<btnNumber) SelectAButton(btns, btnNumber, i);
	}
}