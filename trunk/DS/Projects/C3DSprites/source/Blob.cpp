#include <PA9.h>
#include "Blob.h"
#include "Cell.h"
#include "C3DSpriteData.h"
#include "Team.h"
#include "DSspecs.h"
#include "STDfunctions.h"

//static var init
C3DSpriteData Blob::s_spriteData(NULL, NULL, 0, 0, 0);

Blob::Blob()
:	m_x((DS_WIDTH+64)<<8), m_y(0), 
	m_str(0), 
	m_radius(0), 
	m_velocity(0), 
	m_vx(0), m_vy(0),
	m_team(TEAM_NONE),
	m_state(INACTIVE) 
	
{
	m_id = s_spriteData.Create3DSprite(m_x, m_y);
}

Blob::~Blob(){
	C3DSpriteData::Destroy3DSprite(m_id);
}

void Blob::Set(s32 x, s32 y, TEAM team, Cell * const target, u16 velocity, u16 str, BLOB_STATE state){
		SetXY(x, y);
		SetTeam(team);
		m_target = target;
		SetVelocity(velocity);

		SetStrength(str);
		m_state = state;
}

void Blob::CreateSpriteData(void * sprite, void * palette, u16 width, u16 height, u8 textureType){
	s_spriteData.Set(sprite, palette, width, height, textureType);
}

void Blob::DestroySpriteData(){
	s_spriteData.Reset();
}

s32 Blob::X()const{return m_x;}
s32 Blob::Y()const{return m_y;}
u16 Blob::SpriteId()const{return m_id;}
u32 Blob::Strength()const{return m_str;}
u32 Blob::Velocity()const{return m_velocity;}
u16 Blob::Radius()const{return m_radius;}
TEAM Blob::Team()const{return m_team;}
BLOB_STATE Blob::State()const{return m_state;}

void Blob::SetStrength(u32 str){
	m_str = str;
	m_radius = 2 + ((16*m_str)>>7);
	PA_3DSetSpriteWidthHeight(m_id, (m_radius<<1), (m_radius<<1));
}
void Blob::SetXY(s32 x, s32 y){
	m_x = x; m_y = y;
	PA_3DSetSpriteXY(m_id, unfix(m_x), unfix(m_y));	
}
void Blob::SetTeam(TEAM team){
	m_team = team;
	if(team!=TEAM_NONE){
		PA_3DSetSpriteAnimFrame(m_id, Team2Id(m_team));
	}
}
void Blob::SetVelocity(u32 velocity){
	m_velocity = velocity;
	UpdateVelocity();
}

void Blob::Reset(){
	SetXY(fixed(DS_WIDTH+64), 0);
	SetTeam(TEAM_NONE);//inexisting team, so it won't cause any trouble
	m_state = INACTIVE;
}

void Blob::UpdateVelocity(){
	u16 angle = PA_GetAngle(unfix(m_x), unfix(m_y), unfix(m_target->X()), unfix(m_target->Y()));
    m_vx = unfix(PA_Cos(angle)*m_velocity);
    m_vy = unfix(-PA_Sin(angle)*m_velocity);
}
void Blob::Update(){
	switch(m_state){
	case INACTIVE:
		//
		//if(Stylus.Held) m_state = TRAVEL;
		break;
	case CREATION:
		//
		break;
	case TRAVEL:
		if((u64)PA_Distance(unfix(m_x), unfix(m_y), unfix(m_target->X()), unfix(m_target->Y())) <= (u64)(m_radius+m_target->Radius())*(m_radius+m_target->Radius())){
			m_state = DESTINATION;				
		}
		else{
			SetXY(X()+m_vx, Y()+m_vy);
			UpdateVelocity();
		}
		break;
	case COLLISION:
		//jeej
		break;
	case DESTINATION:
		//Do stuff to target :D
		m_target->Damage(Team(), m_str);

		Reset();
		break;
	default:
		ErrorMsg(DS_TOP,"blob class: unhandled state");
		break;
	}
}
