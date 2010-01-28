#pragma once

enum TEAM{
	TEAM_NONE = 0, 
	TEAM_NEUTRAL = 1,
	TEAM_RED = 2, //player
	TEAM_BLUE = 3 //enemy
};

TEAM Id2Team(u8 id);
u8 Team2Id(TEAM t);
