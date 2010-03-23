#pragma once
#include <PA9.h>
#include "Cell.h"
#include "Team.h"

#define MAX_BLOBS 128

extern C3DSpriteData g_blobGfx[3];

enum BLOB_STATE{
	INACTIVE, 
	CREATION, 
	TRAVEL, 
	COLLISION, 
	DESTINATION
};

class Blob{
private:
	s32 m_x, m_y;
	TEAM m_team;

	u16 m_strength;
	u16 m_radius;

	s32 m_velocity;
	s16 m_vx;
	s16 m_vy;
	BLOB_STATE m_state;
	Cell * m_target;

	u16 m_id;

	void UpdateVelocity();
public:
	Blob();
	Blob(s32 x, s32 y, TEAM team, u32 strength, u32 velocity, Cell * const target);
	~Blob();

	/*
	void Create(s32 x, s32 y, TEAM team, u32 strength, u32 velocity, Cell * const target);
	void Destroy();
	*/ 

	void Load();
	void Unload();

	void Set(s32 x, s32 y, TEAM team, u32 strength, u32 velocity, Cell * const target);
	void Reset();

	void SetXY(s32 x, s32 y);
	void SetTeam(TEAM team);
	void SetStrength(u32 strength);
	void SetVelocity(u32 velocity);
	void SetTarget(Cell * const target);
	void SetState(BLOB_STATE state);

	s32 X()const;
	s32 Y()const;
	TEAM Team()const;
	u32 Strength()const;
	u16 Radius()const;
	s32 Velocity()const;
	s16 XVelocity()const;
	s16 YVelocity()const;
	BLOB_STATE State()const;
	u16 SpriteId()const;
	
	void Update();
};