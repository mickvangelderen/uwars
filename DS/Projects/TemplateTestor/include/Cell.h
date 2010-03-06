#pragma once

#include <PA9.h>
#include "LIBPATH.h"

#include LIB(UnitBase.h)
#include LIB(Team.h)
#include LIB(UnitSpriteData.h)

#define MAX_CELLS 20

class Cell : public UnitBase{
private:
	static unitSpriteData uSD;

	s32 m_str;
	u32 m_capacity;
	u32 m_growthRate;

	u8 m_spriteId;

public:
	Cell();
	~Cell();
	void Set(s32 x, s32 y, u32 radius, TEAM team,u32 str,u32 capacity,u32 growthRate);
	static void CreateSpriteData(void* sprite, void* palette, u32 xOffset, u32 yOffset, u8 shape, u8 size, u8 colorMode);
	static void DestroySpriteData();

	u32 XOffset() const;
	u32 YOffset() const;

	//end of standard stuff
	u32 Str() const;
	u32 Capacity() const;
	u32 GrowthRate() const;

	void SetStr(u32 str);
	void SetCapacity(u32 capacity);
	void SetGrowthRate(u32 growthRate);
	
	void Send(Cell * const cell);
	bool IsTouched();
	void Damage(TEAM team, u16 str);

	void SetTeam(TEAM team);

	void Update();
};

