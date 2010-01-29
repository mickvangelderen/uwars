#pragma once

#include <PA9.h>
#include "LIBPATH.h"
#include "Cell.h"

#include LIB(Team.h)

struct LevelCellData{
	u32 x, y;
	u32 str, capacity, growthRate;
	TEAM team;
};

struct LevelData{
	char * mapName;
	char * mapDescription;
	u32 mapWidth;
	u32 mapHeight;
	LevelCellData cells[MAX_CELLS];
};

void InitLevels();
u16 GetNumberOfLevels();
const LevelData * const GetLevel(u16 id);
