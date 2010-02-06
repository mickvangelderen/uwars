#pragma once

#include <PA9.h>
#include "LIBPATH.h"
#include "Cell.h"

#include LIB(Team.h)

#pragma pack(1)
struct LevelCellData{
	u32 x, y;
	u32 str, capacity, growthRate;
	TEAM team;
};

struct LevelData{
	char * name;
	char * description;
	u32 width;
	u32 height;
	u8 nCells;
	LevelCellData* cells;
};

void InitLevels();
u16 GetNumberOfLevels();
const LevelData * const GetLevel(u16 id);


void ReadStr(char * str, u16 * const strLen, FILE * const f){
	fread(strLen, sizeof(u16), 1, f);
	if(*strLen){
		str = new char[*strLen+1];
		fread(str, sizeof(char), *strLen, f);
		str[*strLen] = '\0';
	}
}

void WriteStr(char * const str, FILE * const f){
	u16 strLen = strlen(str);
	fwrite(&strLen, sizeof(u16), 1, f);
	if(strLen) fwrite(str, sizeof(char), strLen, f);
}

void WriteLevel(LevelData* level){
	u16 lvlNameLen = strlen(level->name) + 5; //+5 for the extension and terminating null character
	char * lvlName = new char[lvlNameLen];
	strcpy(lvlName, level->name);
	strcpy(lvlName, ".lvl");
	lvlName[lvlNameLen] = '\0';
	
	FILE * f = fopen(lvlName, "wb");
	if(f){
		WriteStr(level->name, f);
		WriteStr(level->description, f);
		fwrite(&level->width, sizeof(u32), 1, f);
		fwrite(&level->height, sizeof(u32), 1, f);
		fwrite(&level->nCells, sizeof(u8), 1, f);
		fwrite(&level->cells, sizeof(LevelCellData), level->nCells, f);
	}
}
