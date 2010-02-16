#include <PA9.h>
#include "Blob.h"
#include "Cell.h"

#define LIBPATH C:/SVN/DS/Libraries
#define LIB(x) <LIBPATH/x>

#include LIB(DSspecs.h)
#include LIB(STDfunctions.h)

#include LIB(UnitBase.h)
#include LIB(Team.h)
#include LIB(UnitSpriteData.h)

unitSpriteData Blob::uSD = {NULL, NULL, 0, 0, 0, 0, 0, 0};

Blob::Blob():
	UnitBase(0, 0, 0, TEAM_NONE),
	m_target(NULL), 
	m_velocity(0), 
	m_vx(0), 
	m_vy(0), 
	m_str(0),
	m_state(INACTIVE)
{
	m_spriteId = CreateSprite(DS_BTM, uSD.sprite, uSD.shape, uSD.size, uSD.colorMode, uSD.paletteId, DS_WIDTH, 0);
	PA_SetSpriteAnim(DS_BTM, m_spriteId, 0);
}

Blob::~Blob(){
	DestroySprite(DS_BTM, m_spriteId);
}

void Blob::Set(s32 x, s32 y, u32 radius, TEAM team, Cell * const target, u32 velocity, u16 str, BLOB_STATE state){
		SetXYCenter(x, y);
		SetRadius(radius);
		SetTeam(team);

		m_target = target;

		m_velocity = velocity;
		UpdateVelocity();

		m_str = str;
		m_state = state;

		PA_SetSpriteAnim(DS_BOTTOM, m_spriteId, Team2Id(Team()));
		PA_SetSpriteXY(DS_BOTTOM, m_spriteId, unfix(X()),unfix(Y()));	
}

void Blob::CreateSpriteData(void* sprite, void* palette, u32 xOffset, u32 yOffset, u8 shape, u8 size, u8 colorMode){
	uSD.sprite = sprite;
	uSD.palette = palette;
	uSD.paletteId = CreateSpritePalette(DS_BTM, uSD.palette);
	uSD.xOffset = xOffset;
	uSD.yOffset = yOffset;
	uSD.shape = shape;
	uSD.size = size;
	uSD.colorMode = colorMode;
}
void Blob::DestroySpriteData(){
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
u32 Blob::XOffset()const{
	return uSD.xOffset;
}
u32 Blob::YOffset()const{
	return uSD.yOffset;
}

//End of Standard stuff
BLOB_STATE Blob::State()const{
	return m_state;
}

void Blob::Reset(){
	SetXY(fixed(DS_WIDTH), 0);
	PA_SetSpriteXY(DS_BOTTOM, m_spriteId, DS_WIDTH, 0);
	SetTeam(TEAM_NONE);//inexisting team, so it won't cause any trouble
	m_state=INACTIVE;
}
void Blob::UpdateVelocity(){
	u16 angle = PA_GetAngle(XCenter(),YCenter(), m_target->XCenter(), m_target->YCenter());
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
		if(IsCollision(m_target)){
			m_state = DESTINATION;				
		}
		else{
			SetXY(X()+m_vx, Y()+m_vy);
			PA_SetSpriteXY(DS_BOTTOM,m_spriteId,unfix(X()),unfix(Y()));
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
