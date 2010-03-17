#pragma once

#include <PA9.h>
#include "Cell.h"
#include "C3DSpriteData.h"
#include "Team.h"

#define MAX_BLOBS 128

enum BLOB_STATE{
	INACTIVE, 
	CREATION, 
	TRAVEL, 
	COLLISION, 
	DESTINATION
};

class Blob{
private:
	static C3DSpriteData s_spriteData;

	s32 m_x, m_y;
	u16 m_str;
	u16 m_radius;
	

	Cell * m_target;
	u32 m_velocity;
	s16 m_vx;
	s16 m_vy;

	TEAM m_team;
	BLOB_STATE m_state;

	u8 m_id;

	void UpdateVelocity();
public:
	Blob();
	~Blob();
	void Set(s32 x, s32 y, TEAM team, Cell * const target, u16 velocity, u16 str, BLOB_STATE state);
	static void CreateSpriteData(void * sprite, void * palette, u16 width, u16 height, u8 textureType);
	static void DestroySpriteData();

	s32 X()const;
	s32 Y()const;
	u16 SpriteId()const;
	u32 Strength()const;
	u32 Velocity()const;
	u16 Radius()const;
	TEAM Team()const;
	BLOB_STATE State()const;
	void SetStrength(u32 str);
	void SetXY(s32 x, s32 y);
	void SetTeam(TEAM team);
	void SetVelocity(u32 velocity);
	void Reset();
	void Update();
};

