#pragma once

#include <PA9.h>

#include "C3DSpriteData.h"
#include "Team.h"

#define MAX_CELLS 64

#define RADIUS_MIN 8
#define RADIUS_MAX 32
#define RADIUS_DELTA (RADIUS_MAX - RADIUS_MIN)
//#define RADIUS_MAXCAPACITY 256

class Blob;

class Cell{
private:
	static C3DSpriteData s_spriteData;
	
	s32 m_x, m_y;
	s32 m_str;
	u32 m_capacity;
	u16 m_radius;
	u32 m_growthRate;
	u16 m_id;
	TEAM m_team;

public:
	Cell();
	~Cell();
	void Set(s32 x, s32 y, TEAM team, u32 str, u32 capacity, u32 growthRate);
	
	static void CreateSpriteData(void * sprite, void * palette, u16 width, u16 height, u8 textureType);
	static void DestroySpriteData();

	s32 X() const;
	s32 Y() const;
	u16 SpriteId() const;
	u32 Strength() const;
	u32 Capacity() const;
	u32 GrowthRate() const;
	TEAM Team() const;
	u16 Radius() const;

	void SetStrength(u32 str);
	void SetCapacity(u32 capacity);
	void SetGrowthRate(u32 growthRate);
	void SetXY(s32 x, s32 y);
	void SetTeam(TEAM team);

	void Send(Cell * const cell, Blob blobs[]);
	void Damage(TEAM team, u16 str);

	bool IsTouched();

	void Update();
};

