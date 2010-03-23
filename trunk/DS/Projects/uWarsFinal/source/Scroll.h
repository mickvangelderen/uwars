#pragma once
#include <PA9.h>

extern s32 scrollx;
extern s32 scrolly;

void InitScroll(s32 mapWidth, s32 mapHeight, u8 scrollSpeed);
void UpdateScroll();