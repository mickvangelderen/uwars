#pragma once
#include <PA9.h>
#include "PlayerProfile.h"

extern PlayerProfile * g_playerProfile;
extern u8 g_playerProfileId;
extern PlayerProfile * g_playerProfiles;
#define g_nPlayerProfiles 3

u8 ProfileStartLevel();
u8 AreYouSureYouWantToReturn();
u8 ProfileViewStats();
u8 ProfileSave();
u8 ProfileMenu();
