#include <PA_BgStruct.h>

extern const char uWars_Bg_Game_B_Tiles[];
extern const char uWars_Bg_Game_B_Map[];
extern const char uWars_Bg_Game_B_Pal[];

const PA_BgStruct uWars_Bg_Game_B = {
	PA_BgNormal,
	256, 192,

	uWars_Bg_Game_B_Tiles,
	uWars_Bg_Game_B_Map,
	{uWars_Bg_Game_B_Pal},

	49152,
	{1536}
};
