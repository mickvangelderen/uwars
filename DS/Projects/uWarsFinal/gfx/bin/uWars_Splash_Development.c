#include <PA_BgStruct.h>

extern const char uWars_Splash_Development_Tiles[];
extern const char uWars_Splash_Development_Map[];
extern const char uWars_Splash_Development_Pal[];

const PA_BgStruct uWars_Splash_Development = {
	PA_BgNormal,
	256, 192,

	uWars_Splash_Development_Tiles,
	uWars_Splash_Development_Map,
	{uWars_Splash_Development_Pal},

	25344,
	{1536}
};
