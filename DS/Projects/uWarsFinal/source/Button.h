#pragma once
#include <PA9.h>
#include "DSspecs.h"
#include "Palette.h"

class Button{
private:
	//=button=
	u8 m_screen;
	s16 m_x, m_y;
	u8 m_state;

	//=sprites=
	u8 m_nSprites;
	u8 m_nSpritesMax; 
	//sprite data
	void **m_sprites; //pointer to (array of sprite graphic pointers)
	u8 *m_spriteIds;
	//general sprite data
	u8 m_spriteShape, m_spriteSize, m_spriteColorMode;
	u8 m_spriteWidth, m_spriteHeight;
	Palette m_spritePalette;

	//=action=
	u8 (*m_action)(void);
public:
	Button();
	~Button();

	void Create(u8 screen, s16 x, s16 y, u8 nSprites, u8 shape, u8 size, u8 colorMode, u8 spriteWidth, u8 spriteHeight, u8 (*actionFunc)());//
	void Destroy();
	void AddButtonPart(void * sprite);//add a sprite
    void SetSpriteBackground(u8 background);
    
	void Load(u8 paletteId);//create sprites
	void Unload();//delete sprites

	void Select();
	void Deselect();
	bool IsSelected()const;

	void Activate(); //make button pressable
	void Deactivate(); //make button unpressable
	bool IsActivated()const;

	bool IsTouched();

	void Update(); //frames

	u8 Go();//excecute action function
};

void UpdateButtons(Button * btns, u8 btnNumber);
void SelectAButton(Button * btns, u8 btnNumber, u8 selectNumber);
u8 GetSelectedButton(Button * btns, u8 btnNumber);
void ButtonSelectViaPad(Button * btns, u8 btnNumber);
