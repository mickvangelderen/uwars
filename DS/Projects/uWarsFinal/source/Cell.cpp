#include <PA9.h>
#include "Cell.h"
#include "Blob.h"
#include "C3DspriteData.h"
#include "STDfunctions.h"
#include "all_gfx.h"
#include "Scroll.h"
#include "Selection.h"
#include "Game.h"

C3DSpriteData g_cellGfx[3] = {
	C3DSpriteData((void*)Cell_Neutral_Texture, (void*)Cell_Pal, 64, 64, TEX_256COL), 
	C3DSpriteData((void*)Cell_Red_Texture, (void*)Cell_Pal, 64, 64, TEX_256COL), 
	C3DSpriteData((void*)Cell_Blue_Texture, (void*)Cell_Pal, 64, 64, TEX_256COL)};

Cell::Cell()
:	m_x((DS_WIDTH+64)<<8), m_y(0), m_team(TEAM_NONE), 
	m_strength(0), m_capacity(0), m_radius(0), m_growthRate(0), m_id(MAX_3DSPRITES){
}
Cell::Cell(s32 x, s32 y, TEAM team, u32 strength, u32 capacity, u32 growthRate)
:	m_x(x), m_y(y), m_team(team), 
	m_strength(strength),m_capacity(capacity), m_radius(0), m_growthRate(growthRate), m_id(MAX_3DSPRITES){
}
Cell::~Cell(){
	Destroy();
}

void Cell::Create(s32 x, s32 y, TEAM team, u32 strength, u32 capacity, u32 growthRate){
	m_x = x; m_y = y; m_team = team; m_strength = strength; 
	m_capacity = capacity; m_growthRate = growthRate; m_id = MAX_3DSPRITES;
}

void Cell::Destroy(){
}

void Cell::Load(){
	m_id = Create3DSprite(&g_cellGfx[Team2Id(m_team)], unfix(m_x), unfix(m_y));
	SetXY(m_x, m_y);
	SetTeam(m_team);
	SetStrength(m_strength);
	SetCapacity(m_capacity);
	SetGrowthRate(m_growthRate);
}

void Cell::Unload(){
	Destroy3DSprite(m_id); m_id = MAX_3DSPRITES;
}

void Cell::SetXY(s32 x, s32 y){
	m_x = x; m_y = y;
	PA_3DSetSpriteXY(m_id, unfix(m_x), unfix(m_y));
}

void Cell::SetTeam(TEAM team){
	m_team = team;
	if(team!=TEAM_NONE){
		g_cellGfx[Team2Id(m_team)].ChangeSpriteGraphics(m_id);
	}
	if(g_select != NULL){
		g_select->Deselect(this);
	}
}

void Cell::SetStrength(u32 strength){
	m_strength = strength;
}

void Cell::SetCapacity(u32 capacity){
	m_capacity = capacity;
	m_radius = RADIUS_MIN + ((RADIUS_DELTA*(capacity>>8))>>8); //fancy scaling calculation
	PA_3DSetSpriteWidthHeight(m_id, (m_radius<<1), (m_radius<<1));
}

void Cell::SetGrowthRate(u32 growthRate){
	m_growthRate = growthRate;
}

s32 Cell::X()const{return m_x;}
s32 Cell::Y()const{return m_y;}
TEAM Cell::Team()const{return m_team;}
u32 Cell::Strength()const{return m_strength;}
u32 Cell::Capacity()const{return m_capacity;}
u32 Cell::GrowthRate()const{return m_growthRate;}
u32 Cell::EffectiveGrowthRate()const{return (m_strength < (s32)m_capacity)*m_growthRate;}
u16 Cell::Radius()const{return m_radius;}
u16 Cell::SpriteId()const{return m_id;}

void Cell::AttackCell(Cell * const cell, Blob blobs[]){
	s32 dstr = unfix(m_strength>>1);
	if(dstr >= 1){
		for(u8 i=0; i<MAX_BLOBS; i++){
			if(blobs[i].State() == INACTIVE){
				m_strength -= fixed(dstr);
				blobs[i].Set(m_x, m_y, m_team, dstr, 160, cell);
				break;
			}
		}
	}
}

void Cell::TakeDamage(TEAM team, u16 strength){
	if (m_team == team){
		m_strength+=fixed(strength);
		if(m_strength > (s32)m_capacity) m_strength = m_capacity;
	}
	else{
		m_strength-=fixed(strength);
		if(unfix(m_strength) < 0){
			SetTeam(team);
			m_strength *= -1;
		}
		else if(unfix(m_strength) == 0){
			SetTeam(TEAM_NEUTRAL);
			m_strength = fixed(0);
		}
	}
}

bool Cell::IsTouched(){
	s32 projx = unfix(m_x)-scrollx;
	s32 projy = unfix(m_y)-scrolly;
	return (Stylus.Newpress && (PA_Distance(projx, projy, Stylus.X, Stylus.Y) <= (u64)(m_radius*m_radius)));
}

void Cell::Update(){
	if(m_team!=TEAM_NONE){
		s32 projx = unfix(m_x)-scrollx;
		s32 projy = unfix(m_y)-scrolly;

		//grow
		if(m_team==TEAM_RED or m_team==TEAM_BLUE){
			m_strength+=m_growthRate;
			if(m_strength > (s32)m_capacity) m_strength = m_capacity;
		}

		//scroll
		if((projx >= -64) && (projx < DS_WIDTH+64) && (projy >= -64) && (projy < DS_HEIGHT+64)){
			PA_3DSetSpriteXY(m_id, projx, projy);
		}

		//text
		if((projx >= 8) && (projx < DS_WIDTH-8) && (projy >= 8) && (projy < DS_HEIGHT-8)){
			u8 txtColor = 10; //black for neutral
			if(m_team==TEAM_RED or m_team==TEAM_BLUE){
				txtColor = 1; //make it white
			}
			char buffer[6];
			sprintf(buffer, "%d", unfix(m_strength));
			buffer[5] = 0;
			PA_16cText(DS_BTM, projx-5-3*((m_strength>>8) > 99), projy-4,  500, 400, buffer, txtColor, 2, 5);
		}
	}
}
