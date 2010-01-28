#pragma once

#include <PA9.h>
#include "Team.h"

class UnitBase{
private:
    s32 m_x; 
    s32 m_y;
    u32 m_radius; 
    TEAM m_team;
public:
    UnitBase();
    UnitBase(s32 x, s32 y, u32 radius, TEAM team);
    ~UnitBase();
    
    void SetX(s32 x);
    void SetY(s32 y);
    void SetXY(s32 x, s32 y);
    
    void SetXCenter(s32 x);
    void SetYCenter(s32 y);
    void SetXYCenter(s32 x, s32 y);
    
    void SetRadius(u32 radius);
    void SetTeam(TEAM team);
    
    s32 X()const;
    s32 Y()const;
    
    s32 XCenter()const;
    s32 YCenter()const;
    
    u32 Radius()const;
    TEAM Team()const;

	u32 GetDist(UnitBase * const a)const;
	u32 GetDist(s32 x, s32 y)const;

	bool IsCollision(UnitBase * const u)const;

	//virtual
	virtual u32 XOffset() const = 0;
	virtual u32 YOffset() const = 0;
};

/* UnitBase derived class template *//*
#include <PA9.h>
#include <STDfunctions.h>
#include <DSspecs.h>
#include <UnitSpriteData.h>
#include <Team.h>

class obj : public UnitBase{
private:
	static unitSpriteData uSD;

	//(...)

	u8 m_spriteId;
public:
	obj();
	~obj();

	static void CreateSpriteData(void* sprite, void* palette, u32 xOffset, u32 yOffset, u8 shape, u8 size, u8 colorMode);
	static void DestroySpriteData();
	u32 XOffset() const;
	u32 YOffset() const;
};

obj::obj()
: UnitBase(0, 0, 0, TEAM_NONE)
{
	//create sprite
	m_spriteId = CreateSprite(DS_BTM, uSD.sprite, uSD.shape, uSD.size, uSD.colorMode, uSD.paletteId, DS_WIDTH, 0);
}
obj::~obj(){
	//free sprite
	DestroySprite(DS_BTM, m_spriteId);
}

void obj::CreateSpriteData(void* sprite, void* palette, u32 xOffset, u32 yOffset, u8 shape, u8 size, u8 colorMode){
	uSD.sprite = sprite;
	uSD.palette = palette;
	uSD.paletteId = LoadSpritePal(DS_BTM, uSD.palette);
	uSD.xOffset = xOffset;
	uSD.yOffset = yOffset;
	uSD.shape = shape;
	uSD.size = size;
	uSD.colorMode = colorMode;
}
void obj::DestroySpriteData(){
	uSD.sprite = NULL;
	uSD.palette = NULL;
	FreePalId(DS_BTM, uSD.paletteId);
	uSD.paletteId = MAX_PALETTES;
	uSD.xOffset = 0;
	uSD.yOffset = 0;
	uSD.shape = 0;
	uSD.size = 0;
	uSD.colorMode = 0;
}
u32 obj::XOffset()const{
	return uSD.xOffset;
}
u32 obj::YOffset()const{
	return uSD.yOffset;
}

unitSpriteData obj::uSD = {NULL, NULL, 0, 0, 0, 0, 0, 0};
*/
