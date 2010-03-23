#pragma once
#include <PA9.h>
#include "PlayerProfile.h"

	/*
	===structure===
    *  Load Profile
          o 3 buttons with profile names, sends copy of profile to the Profile menu
          o Return button
    * Create Profile
          o Enter new user name
                + Choose profile slot
                + Cancel button
          o Cancel button
    * Start uPedia
          o Choose text file
                + Change pages for reading
                + Return button
          o Return button
    * Credits
          o Return button
	*/

void MainMenuCreateNewProfile(PlayerProfile * profile);
u8 MainMenuLoadProfile();
u8 MainMenuCreateProfile();
u8 MainMenuStartUPedia();
u8 MainMenuCredits();
void MainMenu();
