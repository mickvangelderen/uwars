#include <PA_BgStruct.h>

extern const char BgGameStatic_Tiles[];
extern const char BgGameStatic_Map[];
extern const char BgGameStatic_Pal[];

const PA_BgStruct BgGameStatic = {
	PA_BgUnlimited,
	512, 512,

	BgGameStatic_Tiles,
	BgGameStatic_Map,
	{BgGameStatic_Pal},

	252224,
	{4096}
};
