#include <PA9.h>
#include "Cell.h"
#include "Blob.h"
#include "STDfunctions.h"

//static var init
C3DSpriteData Cell::s_spriteData(NULL, NULL, 0, 0, 0);

Cell::Cell()
:	m_x((DS_WIDTH+64)<<8), m_y(0), 
	m_str(0), 
	m_capacity(0), m_radius(0), 
	m_growthRate(0), m_team(TEAM_NONE)
{
	m_id = s_spriteData.Create3DSprite(m_x, m_y);
	PA_3DSetSpriteFrame(m_id, Team2Id(m_team));
}

Cell::~Cell(){
	C3DSpriteData::Destroy3DSprite(m_id);
}

void Cell::Set(s32 x, s32 y, TEAM team, u32 str, u32 capacity,u32 growthRate){
		SetXY(x, y);
		SetStrength(str);
		SetCapacity(capacity);
		SetGrowthRate(growthRate);
		SetTeam(team);
}

void Cell::CreateSpriteData(void * sprite, void * palette, u16 width, u16 height, u8 textureType){
	s_spriteData.Set(sprite, palette, width, height, textureType);
}

void Cell::DestroySpriteData(){
	s_spriteData.Reset();
}

s32 Cell::X()const{return m_x;}
s32 Cell::Y()const{return m_y;}
u16 Cell::SpriteId()const{return m_id;}
u32 Cell::Strength()const{return m_str;}
u32 Cell::Capacity()const{return m_capacity;}
u32 Cell::GrowthRate()const{return m_growthRate;}
u16 Cell::Radius()const{return m_radius;}
TEAM Cell::Team()const{return m_team;}

void Cell::SetStrength(u32 str){m_str = str;}
void Cell::SetCapacity(u32 capacity){
	m_capacity = capacity;
	m_radius = RADIUS_MIN + ((RADIUS_DELTA*(capacity>>8))>>8);
	PA_3DSetSpriteWidthHeight(m_id, (m_radius<<1), (m_radius<<1));
}
void Cell::SetGrowthRate(u32 growthRate){m_growthRate = growthRate;}
void Cell::SetXY(s32 x, s32 y){
	m_x = x; m_y = y;
	PA_3DSetSpriteXY(m_id, unfix(m_x), unfix(m_y));	
}
void Cell::SetTeam(TEAM team){
	m_team = team;
	if(team!=TEAM_NONE){
		PA_3DSetSpriteAnimFrame(m_id, Team2Id(m_team));
	}
}

void Cell::Send(Cell * const cell, Blob blobs[]){
	s32 dstr = unfix(m_str>>1);
	if(m_str > fixed(4)){
		for(u8 i=0; i<MAX_BLOBS; i++){
			if(blobs[i].State() == INACTIVE){
				m_str -= fixed(dstr);
				blobs[i].Set(m_x, m_y, m_team, cell, fixed(1), dstr, TRAVEL);
				break;
			}
		}
	}
}


bool Cell::IsTouched(){
	return (Stylus.Newpress && PA_Distance(unfix(m_x), unfix(m_y), Stylus.X, Stylus.Y) <= m_radius*m_radius);
}

void Cell::Damage(TEAM team, u16 str){
	if (m_team == team){
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


void Cell::Update(){
	if(m_team!=TEAM_NONE){
		u8 txtColor = 10; //black for neutral
		if(m_team==TEAM_RED or m_team==TEAM_BLUE){
			m_str+=m_growthRate;
			if(m_str > (s32)m_capacity) m_str = m_capacity;
			txtColor = 1; //make it white
		}

		char buffer[128];
		sprintf(buffer, "%d", unfix(m_str));
		buffer[127] = 0;

		PA_16cText(DS_BTM, unfix(m_x)-5-3*((m_str>>8) > 99), unfix(m_y)-4, 
						   unfix(m_x)+16, unfix(m_x)+16,
						   buffer, txtColor, 
						   2, // text size (0-4)
						   127); // maximum number of characters (use like 10000 if you don't know) */
	}
}
