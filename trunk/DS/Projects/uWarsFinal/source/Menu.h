#pragma once
#include "MainMenu.h"
#include "ProfileMenu.h"
#include "LevelSelectMenu.h"

#include "Button.h"
#include "Palette.h"

#define MENU_EXIT 0
#define MENU_STAY 1

typedef u8 (*MenuFunction)(void);
