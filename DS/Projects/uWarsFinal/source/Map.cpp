#include <PA9.h>
#include "Map.h"
#include "DSspecs.h"
#include <cstring>

void ResetMaps(Map * const maps, u16 n){
	for(u16 i=0; i<n; i++){
		SetMap(&maps[i], "No name", "No description", 0, MAP_DIFFICULTY_NONE, 0, 0, 0);
		ResetMapCells(maps[i].cells, MAP_MAX_CELLS);
	}
}

void ResetMapCells(MapCell * const mc, u16 n){
	for(u16 i=0; i<n; i++){
		mc[i].x = 0; mc[i].y = 0;
		mc[i].strength = 0; mc[i].capacity = 0; mc[i].growthRate = 0;
		mc[i].team = TEAM_NONE;
	}
}

void SetMap(Map * const map, const char * const name, const char * const description, u8 level, MAP_DIFFICULTY difficulty, u32 width, u32 height, u16 nCells){
	strncpy(map->name, name, MAP_MAX_NAMELENGTH);
	map->name[MAP_MAX_NAMELENGTH] = '\0';

	strncpy(map->description, description, MAP_MAX_DESCRIPTIONLENGTH);
	map->description[MAP_MAX_DESCRIPTIONLENGTH] = '\0';

	map->level = level;
	map->difficulty = difficulty;
	map->width = width;
	map->height = height;
	map->nCells = nCells;
}

void SetMapCell(Map * const map, u16 cellId, s32 x, s32 y, u32 strength, u32 capacity, u32 growthRate, TEAM team){
	map->cells[cellId].x = x;
	map->cells[cellId].y = y;
	map->cells[cellId].strength = strength;
	map->cells[cellId].capacity = capacity;
	map->cells[cellId].growthRate = growthRate;
	map->cells[cellId].team = team;
}