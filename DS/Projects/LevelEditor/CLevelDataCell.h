#pragma once
#include <stdio.h>
#include "Types.h"
#include "Team.h"

class CLevelDataCell{
public:
	u32 x, y;
	u32 strength, capacity, growthRate;
	TEAM team;
public:
	CLevelDataCell()
	: x(0), y(0), 
	strength(0), capacity(0), growthRate(0), 
	team(TEAM_NONE) {
	}
	~CLevelDataCell(){
	}
	void Write(FILE * const f){
		fwrite(&this->x, sizeof(u32), 1, f);
		fwrite(&this->y, sizeof(u32), 1, f);
		fwrite(&this->strength, sizeof(u32), 1, f);
		fwrite(&this->capacity, sizeof(u32), 1, f);
		fwrite(&this->growthRate, sizeof(u32), 1, f);
		fwrite(&this->team, sizeof(TEAM), 1, f);
	}
	void Read(FILE * const f){
		fread(&this->x, sizeof(u32), 1, f);
		fread(&this->y, sizeof(u32), 1, f);
		fread(&this->strength, sizeof(u32), 1, f);
		fread(&this->capacity, sizeof(u32), 1, f);
		fread(&this->growthRate, sizeof(u32), 1, f);
		fread(&this->team, sizeof(TEAM), 1, f);
	}
};