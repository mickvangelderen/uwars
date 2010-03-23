#include <PA_BgStruct.h>

extern const char uWars_Bg_Menu_T_Tiles[];
extern const char uWars_Bg_Menu_T_Map[];
extern const char uWars_Bg_Menu_T_Pal[];

const PA_BgStruct uWars_Bg_Menu_T = {
	PA_BgNormal,
	256, 192,

	uWars_Bg_Menu_T_Tiles,
	uWars_Bg_Menu_T_Map,
	{uWars_Bg_Menu_T_Pal},

	49088,
	{1536}
};
