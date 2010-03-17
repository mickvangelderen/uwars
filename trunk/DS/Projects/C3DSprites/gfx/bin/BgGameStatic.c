#include <PA_BgStruct.h>

extern const char BgGameStatic_Tiles[];
extern const char BgGameStatic_Map[];
extern const char BgGameStatic_Pal[];

const PA_BgStruct BgGameStatic = {
	PA_BgNormal,
	256, 192,

	BgGameStatic_Tiles,
	BgGameStatic_Map,
	{BgGameStatic_Pal},

	49152,
	{1536}
};
