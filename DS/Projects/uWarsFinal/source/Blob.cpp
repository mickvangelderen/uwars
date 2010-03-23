#include <PA9.h>
#include "Blob.h"
#include "Cell.h"
#include "C3DSpriteData.h"
#include "Team.h"
#include "DSspecs.h"
#include "STDfunctions.h"
#include "all_gfx.h"
#include "Scroll.h"

#include "Sound.h"

C3DSpriteData g_blobGfx[3] = {
	C3DSpriteData((void*)NULL, (void*)NULL, 32, 32, TEX_256COL), //fake neutral
	C3DSpriteData((void*)Blob_Red_Texture, (void*)Blob_Pal, 32, 32, TEX_256COL),
	C3DSpriteData((void*)Blob_Blue_Texture, (void*)Blob_Pal, 32, 32, TEX_256COL)};

void Blob::UpdateVelocity(){
	u16 angle = PA_GetAngle(unfix(m_x), unfix(m_y), unfix(m_target->X()), unfix(m_target->Y()));
    m_vx = unfix(PA_Cos(angle)*m_velocity);
    m_vy = unfix(-PA_Sin(angle)*m_velocity);
}

Blob::Blob()
	:m_x((DS_WIDTH+64)<<8), m_y(0), m_team(TEAM_NONE), 
	m_strength(0), m_radius(0), m_velocity(0), m_vx(0), m_vy(0), 
	m_state(INACTIVE), m_target(NULL), m_id(MAX_3DSPRITES){
}

Blob::Blob(s32 x, s32 y, TEAM team, u32 strength, u32 velocity, Cell * const target)
	:m_x(x), m_y(y), m_team(team), 
	m_strength(strength), m_radius(0), m_velocity(velocity), m_vx(0), m_vy(0), 
	m_state(INACTIVE), m_target(target), m_id(MAX_3DSPRITES){
}

Blob::~Blob(){
}
/*
void Blob::Create(s32 x, s32 y, TEAM team, u32 strength, u32 velocity, Cell * const target){
	m_x = x; m_y = y; m_team = team; 
	m_strength = strength; m_radius = 0; m_velocity = velocity; m_vx = 0; m_vy = 0; 
	m_state = INACTIVE; m_target = target; m_id = MAX_3DSPRITES;
}

void Blob::Destroy(){
}
*/

void Blob::Load(){
	m_id = Create3DSprite(&g_blobGfx[Team2Id(m_team)], unfix(m_x), unfix(m_y));
}

void Blob::Unload(){
	Destroy3DSprite(m_id); m_id = MAX_3DSPRITES;
}

void Blob::Set(s32 x, s32 y, TEAM team, u32 strength, u32 velocity, Cell * const target){
	SetXY(x, y);
	SetTeam(team);
	SetStrength(strength);
	SetVelocity(velocity);
	SetState(CREATION);
	SetTarget(target);
}

void Blob::Reset(){
	PA_3DSetSpriteXY(m_id, DS_WIDTH+64, 0);
	SetTeam(TEAM_NONE);
	SetState(INACTIVE);
}

void Blob::SetXY(s32 x, s32 y){
	m_x = x; m_y = y;
	PA_3DSetSpriteXY(m_id, unfix(m_x)-scrollx, unfix(m_y)-scrolly);
}


void Blob::SetTeam(TEAM team){
	m_team = team;
	if(team!=TEAM_NONE){
		g_blobGfx[Team2Id(m_team)].ChangeSpriteGraphics(m_id);
	}
}

void Blob::SetStrength(u32 strength){
	m_strength = strength;
	m_radius = 2 + ((16*m_strength)>>7);
	PA_3DSetSpriteWidthHeight(m_id, (m_radius<<1), (m_radius<<1));
}

void Blob::SetVelocity(u32 velocity){
	m_velocity = velocity;
	UpdateVelocity();
}

void Blob::SetTarget(Cell * const target){
	m_target = target;
}

void Blob::SetState(BLOB_STATE state){
	m_state = state;
}

s32 Blob::X()const{return m_x;}
s32 Blob::Y()const{return m_y;}
TEAM Blob::Team()const{return m_team;}
u32 Blob::Strength()const{return m_strength;}
u16 Blob::Radius()const{return m_radius;}
s32 Blob::Velocity()const{return m_velocity;}
s16 Blob::XVelocity()const{return m_vx;}
s16 Blob::YVelocity()const{return m_vy;}
BLOB_STATE Blob::State()const{return m_state;}
u16 Blob::SpriteId()const{return m_id;}

void Blob::Update(){
	switch(m_state){
	case INACTIVE:
		//
		//if(Stylus.Held) m_state = TRAVEL;
		break;
	case CREATION:
		m_state = TRAVEL;
		//break;
	case TRAVEL:
		if((u64)PA_Distance(unfix(m_x), unfix(m_y), unfix(m_target->X()), unfix(m_target->Y())) <= (u64)(m_radius+m_target->Radius())*(m_radius+m_target->Radius())){
			m_state = DESTINATION;				
		}
		else{
			m_x += m_vx;
			m_y += m_vy;
			//scroll
			PA_3DSetSpriteXY(m_id, unfix(m_x)-scrollx, unfix(m_y)-scrolly);
			UpdateVelocity();
		}
		break;
	case COLLISION:
		//jeej
		break;
	case DESTINATION:
		//Do stuff to target :D
		mmEffectCancelAll();
		mmEffectRelease(mmEffect(SFX_BLUB));
		m_target->TakeDamage(Team(), m_strength);
		Reset();
		break;
	default:
		//ErrorMsg(DS_TOP,"blob class: unhandled state");
		break;
	}
}




