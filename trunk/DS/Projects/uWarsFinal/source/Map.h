#pragma once
#include <PA9.h>
#include "Team.h"

#define MAP_MAX_NAMELENGTH 31
#define MAP_MAX_DESCRIPTIONLENGTH 127
#define MAP_MAX_CELLS 32

struct MapCell{
	s32 x, y;
	u32 strength, capacity, growthRate;
	TEAM team;
};

enum MAP_DIFFICULTY{
	MAP_DIFFICULTY_NONE, 
	MAP_DIFFICULTY_EASY, 
	MAP_DIFFICULTY_MEDIUM, 
	MAP_DIFFICULTY_HARD, 
	MAP_DIFFICULTY_INSANE
};

struct Map{
	char name[MAP_MAX_NAMELENGTH+1];
	char description[MAP_MAX_DESCRIPTIONLENGTH+1];
	u8 level;
	MAP_DIFFICULTY difficulty;
	u32 width;
	u32 height;
	u16 nCells;
	MapCell cells[MAP_MAX_CELLS];
};

void ResetMaps(Map * const maps, u16 n);
void ResetMapCells(MapCell * const mc, u16 n);

void SetMap(Map * const map, const char * const name, const char * const description, u8 level, MAP_DIFFICULTY difficulty, u32 width, u32 height, u16 nCells);
void SetMapCell(Map * const map, u16 cellId, s32 x, s32 y, u32 strength, u32 capacity, u32 growthRate, TEAM team);
