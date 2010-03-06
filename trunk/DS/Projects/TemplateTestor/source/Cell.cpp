#include <PA9.h>
#include "Cell.h"
#include "Blob.h"

#define LIBPATH C:/SVN/DS/Libraries
#define LIB(x) <LIBPATH/x>

#include LIB(DSspecs.h)
#include LIB(STDfunctions.h)

extern Blob * blobs;

//static var init
unitSpriteData Cell::uSD = {NULL, NULL, 0, 0, 0, 0, 0, 0};

Cell::Cell()
:	UnitBase(fixed(DS_WIDTH), 0, 0, TEAM_NONE), 
	m_str(0), 
	m_capacity(0), 
	m_growthRate(0)
{
	m_spriteId = CreateSprite(DS_BTM, uSD.sprite, uSD.shape, uSD.size, uSD.colorMode, uSD.paletteId, DS_WIDTH, 0);
	PA_SetSpriteExtPrio(DS_BTM, m_spriteId, 127);
	PA_SetSpritePrio(DS_BTM, m_spriteId, BG_TEXT+1);
	PA_SetSpriteAnim(DS_BTM, m_spriteId, 0);
}

Cell::~Cell(){
	DestroySprite(DS_BTM, m_spriteId);
}

void Cell::Set(s32 x, s32 y, u32 radius, TEAM team,u32 str,u32 capacity,u32 growthRate){
		SetXYCenter(x, y);
		SetRadius(radius);

		

		m_str = str;
		m_capacity = capacity;
		m_growthRate = growthRate;

		SetTeam(team);
		PA_SetSpriteXY(DS_BTM, m_spriteId, unfix(x-uSD.xOffset), unfix(y-uSD.yOffset));	
}

void Cell::CreateSpriteData(void* sprite, void* palette, u32 xOffset, u32 yOffset, u8 shape, u8 size, u8 colorMode){
	uSD.sprite = sprite;
	uSD.palette = palette;
	uSD.paletteId = CreateSpritePalette(DS_BTM, uSD.palette);
	uSD.xOffset = xOffset;
	uSD.yOffset = yOffset;
	uSD.shape = shape;
	uSD.size = size;
	uSD.colorMode = colorMode;
}
void Cell::DestroySpriteData(){
	uSD.sprite = NULL;
	uSD.palette = NULL;
	DestroySpritePalette(DS_BTM, uSD.paletteId);
	uSD.paletteId = MAX_PALETTES;
	uSD.xOffset = 0;
	uSD.yOffset = 0;
	uSD.shape = 0;
	uSD.size = 0;
	uSD.colorMode = 0;
}
u32 Cell::XOffset()const{return uSD.xOffset;}
u32 Cell::YOffset()const{return uSD.yOffset;}

//End of standard stuff

u32 Cell::Str()const{return m_str;}
u32 Cell::Capacity()const{return m_capacity;}
u32 Cell::GrowthRate()const{return m_growthRate;}

void Cell::SetStr(u32 str){m_str = str;}
void Cell::SetCapacity(u32 capacity){m_capacity = capacity;}
void Cell::SetGrowthRate(u32 growthRate){m_growthRate = growthRate;}

void Cell::Send(Cell * const cell){
	s32 dstr = unfix(m_str>>1);
	if(m_str > fixed(4)){
		for(u8 i=0; i<MAX_BLOBS; i++){
			if(blobs[i].State() == INACTIVE){
				m_str -= fixed(dstr);
				blobs[i].Set(XCenter(), YCenter(), fixed(4), Team(), cell, fixed(1), dstr, TRAVEL);
				break;
			}
		}
	}
}

bool Cell::IsTouched(){
	return (Stylus.Newpress && GetDist(fixed(Stylus.X), fixed(Stylus.Y)) <= Radius()*Radius());
}

void Cell::Damage(TEAM team, u16 str){
	if (Team() == team){
		m_str+=fixed(str);
		if(m_str > (s32)m_capacity) m_str = m_capacity;
	}
	else{
		m_str-=fixed(str);
		if(unfix(m_str) < 0){
			SetTeam(team);
			m_str *= -1;
		}
		else if(unfix(m_str) == 0){
			SetTeam(TEAM_NEUTRAL);
			m_str = fixed(0);
		}
	}
}

void Cell::SetTeam(TEAM team){
	UnitBase::SetTeam(team);
	if(team!=TEAM_NONE){
		PA_SetSpriteAnim(DS_BTM, m_spriteId, Team2Id(team));
	}
}

void Cell::Update(){
	if(Team()!=TEAM_NONE){
		u8 txtColor = 10; //black for neutral
		if(Team()==TEAM_RED or Team()==TEAM_BLUE){
			m_str+=m_growthRate;
			if(m_str > (s32)m_capacity) m_str = m_capacity;
			txtColor = 1; //make it white
		}

		char buffer[256];
		
		sprintf(buffer, "%d", unfix(m_str));

		PA_16cText(DS_BTM, unfix(XCenter())-5, unfix(YCenter())-4, 
						   unfix(XCenter())+16, unfix(YCenter())+16,
						   buffer, txtColor, 
						   2, // text size (0-4)
						   100); // maximum number of characters (use like 10000 if you don't know) */
	}
}
