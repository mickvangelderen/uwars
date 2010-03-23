#include <PA_BgStruct.h>

extern const char uWars_Bg_Menu_B_Tiles[];
extern const char uWars_Bg_Menu_B_Map[];
extern const char uWars_Bg_Menu_B_Pal[];

const PA_BgStruct uWars_Bg_Menu_B = {
	PA_BgNormal,
	256, 192,

	uWars_Bg_Menu_B_Tiles,
	uWars_Bg_Menu_B_Map,
	{uWars_Bg_Menu_B_Pal},

	20544,
	{1536}
};
