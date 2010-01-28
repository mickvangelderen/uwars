#pragma once
#include <ndstypes.h>

struct unitSpriteData{
	void * sprite;
	void * palette;
	u8 paletteId;
	u32 xOffset;
	u32 yOffset;
	u8 shape;
	u8 size;
	u8 colorMode;
};
