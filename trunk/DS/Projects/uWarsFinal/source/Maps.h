#pragma once
#include <PA9.h>
#include "Map.h"

#define MAP_NUMBER 12

extern Map g_maps[MAP_NUMBER];
extern Map * g_map;

void InitMaps();