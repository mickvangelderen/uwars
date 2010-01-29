#pragma once

#include <PA9.h>
#include "Cell.h"
#include "LIBPATH.h"

#include LIB(UnitBase.h)
#include LIB(Team.h)
#include LIB(UnitSpriteData.h)

#define MAX_BLOBS 50

enum BLOB_STATE{
	INACTIVE, 
	CREATION, 
	TRAVEL, 
	COLLISION, 
	DESTINATION
};

class Blob : public UnitBase{
private:
	static unitSpriteData uSD;

	Cell * m_target;
	u32 m_velocity;
	s16 m_vx;
	s16 m_vy;
	u16 m_str;
	BLOB_STATE m_state;

	u8 m_spriteId;

	void UpdateVelocity();
public:
	Blob();
	~Blob();
	void Set(s32 x, s32 y, u32 radius, TEAM team, Cell * const target, u32 velocity, u16 str, BLOB_STATE state);
	static void CreateSpriteData(void* sprite, void* palette, u32 xOffset, u32 yOffset, u8 shape, u8 size, u8 colorMode);
	static void DestroySpriteData();
	u32 XOffset() const;
	u32 YOffset() const;
	//end of standard stuff
	BLOB_STATE State() const;

	void Reset();
	void Update();
};

