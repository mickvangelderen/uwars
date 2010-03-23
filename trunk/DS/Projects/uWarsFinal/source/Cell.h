#pragma once
#include <PA9.h>
#include "C3DSpriteData.h"
#include "Team.h"

#define MAX_CELLS 64

#define RADIUS_MIN 8
#define RADIUS_MAX 32
#define RADIUS_DELTA (RADIUS_MAX - RADIUS_MIN)

extern C3DSpriteData g_cellGfx[3];

class Blob;

class Cell{
private:
	s32 m_x, m_y;
	TEAM m_team;
	s32 m_strength;
	u32 m_capacity;
	u16 m_radius;
	u32 m_growthRate;
	u16 m_id;

public:
	Cell();
	Cell(s32 x, s32 y, TEAM team, u32 strength, u32 capacity, u32 growthRate);
	~Cell();

	void Create(s32 x, s32 y, TEAM team, u32 strength, u32 capacity, u32 growthRate);
	void Destroy();

	void Load();
	void Unload();
	
	void SetXY(s32 x, s32 y);
	void SetTeam(TEAM team);
	void SetStrength(u32 strength);
	void SetCapacity(u32 capacity);
	void SetGrowthRate(u32 growthRate);

	s32 X() const;
	s32 Y() const;
	TEAM Team() const;
	u32 Strength() const;
	u32 Capacity() const;
	u32 GrowthRate() const;
	u32 EffectiveGrowthRate() const;
	u16 Radius() const;
	u16 SpriteId() const;

	void AttackCell(Cell * const cell, Blob blobs[]);
	void TakeDamage(TEAM team, u16 strength);

	bool IsTouched();
	void Update();
};

