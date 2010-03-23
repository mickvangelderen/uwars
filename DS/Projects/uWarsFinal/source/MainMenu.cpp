#include <PA9.h>
#include "Menu.h"

#include "DSspecs.h"
#include "all_gfx.h"
#include "InitFat.h"
#include "Sound.h"

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

void MainMenuCreateNewProfile(PlayerProfile * profile){
	PA_InitKeyboard(2);
	PA_KeyboardIn(20,95);

	PA_Init16cBg(DS_TOP, BG_TEXT);
	PA_InitText(DS_BTM, BG_TEXT);
	PA_16cText(DS_TOP, 32, 32, 255, 191, "Creating a new profile...", 8, 3, 100);
	PA_OutputSimpleText(DS_BTM, 1, 4, "Tell me, ");
	PA_OutputSimpleText(DS_BTM, 1, 5, "how you would like be called: "); 
	
	//Set name to default
	u16 i;
	s16 nLetter = 0; //cursor position
	char n[PLAYER_PROFILE_MAX_NAMELENGTH+1]; n[PLAYER_PROFILE_MAX_NAMELENGTH] = '\0';

	for(i=0; i<PLAYER_PROFILE_MAX_NAMELENGTH; i++){
		if(PA_UserInfo.Name[i] != '\0'){
			n[i] = PA_UserInfo.Name[i];
			nLetter++;
		}
		else n[i] = '\0';
	}
	
	char chInput = 0; //input character
	bool bDone = 0;
	bool cursorOn = false;
	u8 t=0;
	bool firstInput = true;

	PA_WaitForVBL();
	while (!bDone)
	{
		chInput = PA_CheckKeyboard();
		if ((chInput > 31) && (nLetter < PLAYER_PROFILE_MAX_NAMELENGTH)) // there is a new letter and there is space enough to input more
		{
			n[nLetter] = chInput;
			nLetter++;
			if(firstInput){
				n[0] = chInput;
				for(i = 1; i < PLAYER_PROFILE_MAX_NAMELENGTH+1; i++) n[i] = '\0';
				nLetter = 1;
				firstInput = false;
			}
		}
		else if ((chInput == PA_BACKSPACE || Pad.Newpress.B) && nLetter>0) //backspace pressed
		{
			n[--nLetter] = '\0'; // Erase the last letter
			if(firstInput){
				for(i = 0; i < PLAYER_PROFILE_MAX_NAMELENGTH+1; i++) n[i] = '\0';
				nLetter = 0;
				firstInput = false;
			}
		}
		else if ((chInput == '\n' || Pad.Newpress.Start || Pad.Newpress.A) && nLetter>0)//enter pressed
		{
			bDone = 1;
		}
		PA_OutputSimpleText(DS_BOTTOM, 0, 7, "                                ");
		if(cursorOn) PA_OutputText(DS_BOTTOM, 1, 7, "%s_", n); // Write the text
		else PA_OutputText(DS_BOTTOM, 1, 7, "%s", n); // Write the text
		t = (t+1)%30;
		if(t==0) cursorOn = !cursorOn;
		PA_WaitForVBL();
	}
	PA_KeyboardOut();

	profile->Set(n, 1, 0);
	PA_OutputSimpleText(0, profile->NameLength()+1, 7, " it is. ");
	PA_WaitFor(Stylus.Newpress||Pad.Newpress.Anykey);
	
	PA_ClearTextBg(DS_TOP);
	PA_ClearTextBg(DS_BTM);
}

u8 MainMenuLoadProfile(){
	const u8 btnNum = 4;
	Button btn[btnNum];

	for(u8 curBtn=0; curBtn<btnNum-1; curBtn++){
		btn[curBtn].Create(DS_BTM, 64, 36+curBtn*32, 8, OBJ_SIZE_16X32, 1, 16, 24, NULL);
		btn[curBtn].AddButtonPart((void*)BlueButton_0_Sprite);
		for(u8 i=1; i<7; i++) btn[curBtn].AddButtonPart((void*)BlueButton_1_Sprite);
		btn[curBtn].AddButtonPart((void*)BlueButton_2_Sprite);
	}
	btn[3].Create(DS_BTM, 64, 36+32*3, 8, OBJ_SIZE_16X32, 1, 16, 24, NULL);
	btn[3].AddButtonPart((void*)BlueButton_0_Sprite);
	for(u8 i=1; i<7; i++) btn[3].AddButtonPart((void*)BlueButton_1_Sprite);
	btn[3].AddButtonPart((void*)BlueButton_2_Sprite);

    Palette pal(DS_BTM, (void*)ButtonBlue_Pal);
    
	u8 stay = MENU_STAY;
	while(stay == MENU_STAY){
		//Load backgrounds
		PA_Init16cBg(DS_TOP, BG_TEXT);
		PA_Init16cBg(DS_BTM, BG_TEXT);
		PA_DeleteBg(DS_TOP, BG_BACKGROUND); //PA_LoadBackground(DS_TOP, BG_BACKGROUND, &uWars_Bg_Menu_T);
		PA_DeleteBg(DS_BTM, BG_BACKGROUND); //PA_LoadBackground(DS_BTM, BG_BACKGROUND, &uWars_Bg_Menu_B);
		//Load palettes
        pal.Load();
		//Load buttons
		for(u8 i=0; i<btnNum; i++){
			btn[i].Load(pal.Id());
			btn[i].SetSpriteBackground(3);
		}
		//Load profiles
		if(g_playerProfiles != NULL) PA_16cText(DS_TOP, 0, 0, 255, 191, "ERROR: g_playerProfiles != NULL", 2, 4, 1000);
		g_playerProfiles = new PlayerProfile[g_nPlayerProfiles];
		LoadPlayerProfiles(g_playerProfiles, g_nPlayerProfiles);

		//Button text
		PA_16cText(0, 64+6, 36+6,	 255, 191, g_playerProfiles[0].Name(),	8, 3, 100);
		PA_16cText(0, 64+6, 36+6+32, 255, 191, g_playerProfiles[1].Name(),	8, 3, 100);
		PA_16cText(0, 64+6, 36+6+64, 255, 191, g_playerProfiles[2].Name(),	8, 3, 100);
		PA_16cText(0, 64+6, 36+6+96, 255, 191, "Cancel",		8, 3, 100);
		//Deactivate nonworking buttons
		for(u8 i=0; i<3; i++){
			if(!g_playerProfiles[i].IsInitialized()) btn[i].Deactivate();
		}
		//Select a button
		for(u8 i=0; i<btnNum; i++){
			if(btn[i].IsActivated()){
				btn[i].Select();
				break;
			}
		}

		UpdateButtons(btn, btnNum);

		u8 menuAction = btnNum;

		PA_WaitForVBL();

		while(menuAction == btnNum){ //get input
			//Dynamic text
			char buffer[128]; buffer[127] = '\0';
			u8 sel = GetSelectedButton(btn, btnNum);

			if(sel >= 0 && sel < 3){
				snprintf(buffer, 127, "Name: %s", g_playerProfiles[sel].Name());
				PA_16cText(DS_TOP, 16, 16, 255, 191, buffer, 8, 3, 127);

				snprintf(buffer, 127, "Level: %d", g_playerProfiles[sel].Level());
				PA_16cText(DS_TOP, 16, 32, 255, 191, buffer, 8, 3, 127);

				snprintf(buffer, 127, "Experience: %d/%d", g_playerProfiles[sel].Experience(), PLAYER_PROFILE_EXP_FOR_LVLUP);
				PA_16cText(DS_TOP, 16, 48, 255, 191, buffer, 8, 3, 127);
			}
			else if(sel == btnNum-1){
				PA_16cText(DS_TOP, 16, 16, 255, 191, "Go back to the main menu", 8, 3, 127);
			}

			for(u8 curBtn=0; curBtn<btnNum; curBtn++){
				if(btn[curBtn].IsTouched() && btn[curBtn].IsActivated()){
					if(btn[curBtn].IsSelected()){
						menuAction = curBtn;
					}
					else{
						for(u8 j=0; j<btnNum; j++) btn[j].Deselect();
						btn[curBtn].Select();
						for(u8 j=0; j<btnNum; j++) btn[j].Update();
					}
				}
			}
			if(Pad.Newpress.A){
				menuAction = GetSelectedButton(btn, btnNum);
			}
			if(Pad.Newpress.B && btn[btnNum-1].IsActivated()){
				if(btn[btnNum-1].IsSelected()){
					menuAction = btnNum-1;
				}
				else{
					SelectAButton(btn, btnNum, btnNum-1);
				}
			}
			ButtonSelectViaPad(btn, btnNum); //implement arrow key functionality
			PA_WaitForVBL();
			PA_16cErase(DS_TOP);
		}
		//clean
		for(u8 curBtn=0; curBtn<btnNum; curBtn++){
			btn[curBtn].Unload();
		}
		pal.Unload();
		PA_ClearTextBg(DS_TOP);
		PA_ClearTextBg(DS_BTM);

		if(menuAction >= 0 && menuAction < btnNum-1){
			g_playerProfile = new PlayerProfile(g_playerProfiles[menuAction]);
			g_playerProfileId = menuAction;
			stay = ProfileMenu();
			delete g_playerProfile; g_playerProfile = NULL;
			g_playerProfileId = g_nPlayerProfiles;
		}
		else if(menuAction == btnNum-1){
			stay = MENU_EXIT; //quit this menu
		}
		else{
			stay = MENU_EXIT; //unhandled button id
		}
		delete[]g_playerProfiles; g_playerProfiles=NULL;
	}
	return MENU_STAY; //stay in the parent menu
}

u8 MainMenuCreateProfile(){
	const u8 btnNum = 4;
	Button btn[btnNum];

	for(u8 curBtn=0; curBtn<btnNum-1; curBtn++){
		btn[curBtn].Create(DS_BTM, 64, 36+curBtn*32, 8, OBJ_SIZE_16X32, 1, 16, 24, NULL);
		btn[curBtn].AddButtonPart((void*)BlueButton_0_Sprite);
		for(u8 i=1; i<7; i++) btn[curBtn].AddButtonPart((void*)BlueButton_1_Sprite);
		btn[curBtn].AddButtonPart((void*)BlueButton_2_Sprite);
	}
	btn[3].Create(DS_BTM, 64, 36+32*3, 8, OBJ_SIZE_16X32, 1, 16, 24, NULL);
	btn[3].AddButtonPart((void*)BlueButton_0_Sprite);
	for(u8 i=1; i<7; i++) btn[3].AddButtonPart((void*)BlueButton_1_Sprite);
	btn[3].AddButtonPart((void*)BlueButton_2_Sprite);

    Palette pal(DS_BTM, (void*)ButtonBlue_Pal);
    
	u8 stay = MENU_STAY;
	while(stay == MENU_STAY){
		//Load backgrounds
		PA_Init16cBg(DS_TOP, BG_TEXT);
		PA_Init16cBg(DS_BTM, BG_TEXT);
		PA_DeleteBg(DS_TOP, BG_BACKGROUND); //PA_LoadBackground(DS_TOP, BG_BACKGROUND, &uWars_Bg_Menu_T);
		PA_DeleteBg(DS_BTM, BG_BACKGROUND); //PA_LoadBackground(DS_BTM, BG_BACKGROUND, &uWars_Bg_Menu_B);
		//Load palettes
        pal.Load();
		//Load buttons
		for(u8 i=0; i<btnNum; i++){
			btn[i].Load(pal.Id());
			btn[i].SetSpriteBackground(3);
		}
		//Load profiles
		if(g_playerProfiles != NULL) PA_16cText(DS_TOP, 0, 0, 255, 191, "ERROR: g_playerProfiles != NULL", 2, 4, 1000);
		g_playerProfiles = new PlayerProfile[g_nPlayerProfiles];
		LoadPlayerProfiles(g_playerProfiles, g_nPlayerProfiles);

		//Button text
		PA_16cText(0, 64+6, 36+6,	 255, 191, g_playerProfiles[0].Name(),	8, 3, 100);
		PA_16cText(0, 64+6, 36+6+32, 255, 191, g_playerProfiles[1].Name(),	8, 3, 100);
		PA_16cText(0, 64+6, 36+6+64, 255, 191, g_playerProfiles[2].Name(),	8, 3, 100);
		PA_16cText(0, 64+6, 36+6+96, 255, 191, "Cancel",		8, 3, 100);
		//Deactivate nonworking buttons

		//Select a button
		for(u8 i=0; i<btnNum; i++){
			if(btn[i].IsActivated()){
				btn[i].Select();
				break;
			}
		}

		UpdateButtons(btn, btnNum);

		u8 menuAction = btnNum;

		PA_WaitForVBL();

		while(menuAction == btnNum){ //get input
			//Dynamic text
			char buffer[128]; buffer[127] = '\0';
			u8 sel = GetSelectedButton(btn, btnNum);

			if(sel >= 0 && sel < 3){
				snprintf(buffer, 127, "Name: %s", g_playerProfiles[sel].Name());
				PA_16cText(DS_TOP, 16, 16, 255, 191, buffer, 8, 3, 127);

				snprintf(buffer, 127, "Level: %d", g_playerProfiles[sel].Level());
				PA_16cText(DS_TOP, 16, 32, 255, 191, buffer, 8, 3, 127);

				snprintf(buffer, 127, "Experience: %d/%d", g_playerProfiles[sel].Experience(), PLAYER_PROFILE_EXP_FOR_LVLUP);
				PA_16cText(DS_TOP, 16, 48, 255, 191, buffer, 8, 3, 127);
			}
			else if(sel == btnNum-1){
				PA_16cText(DS_TOP, 16, 16, 255, 191, "Go back to the main menu", 8, 3, 127);
			}

			for(u8 curBtn=0; curBtn<btnNum; curBtn++){
				if(btn[curBtn].IsTouched() && btn[curBtn].IsActivated()){
					if(btn[curBtn].IsSelected()){
						menuAction = curBtn;
					}
					else{
						for(u8 j=0; j<btnNum; j++) btn[j].Deselect();
						btn[curBtn].Select();
						for(u8 j=0; j<btnNum; j++) btn[j].Update();
					}
				}
			}
			if(Pad.Newpress.A){
				menuAction = GetSelectedButton(btn, btnNum);
			}
			if(Pad.Newpress.B && btn[btnNum-1].IsActivated()){
				if(btn[btnNum-1].IsSelected()){
					menuAction = btnNum-1;
				}
				else{
					SelectAButton(btn, btnNum, btnNum-1);
				}
			}
			ButtonSelectViaPad(btn, btnNum); //implement arrow key functionality
			PA_WaitForVBL();
			PA_16cErase(DS_TOP);
		}
		//clean
		for(u8 curBtn=0; curBtn<btnNum; curBtn++){
			btn[curBtn].Unload();
		}
		pal.Unload();
		PA_ClearTextBg(DS_TOP);
		PA_ClearTextBg(DS_BTM);

		if(menuAction >= 0 && menuAction < btnNum-1){
			g_playerProfile = new PlayerProfile();
			g_playerProfileId = menuAction;
			MainMenuCreateNewProfile(g_playerProfile);
			stay = ProfileMenu();
			delete g_playerProfile;
			g_playerProfile = NULL;
		}
		else if(menuAction == btnNum-1){
			stay = MENU_EXIT; //quit this menu
		}
		else{
			stay = MENU_EXIT; //unhandled button id
		}
		delete[]g_playerProfiles; g_playerProfiles=NULL;
	}
	return MENU_STAY; //stay in the parent menu
}

u8 MainMenuStartUPedia(){

	return MENU_STAY;
}

u8 MainMenuCredits(){
	const u8 btnNum = 1;
	Button btn[btnNum];

	btn[0].Create(DS_BTM, 88, 160, 5, OBJ_SIZE_16X32, 1, 16, 24, NULL);
	btn[0].AddButtonPart((void*)BlueButton_0_Sprite);
	btn[0].AddButtonPart((void*)BlueButton_1_Sprite);
	btn[0].AddButtonPart((void*)BlueButton_1_Sprite);
	btn[0].AddButtonPart((void*)BlueButton_1_Sprite);
	btn[0].AddButtonPart((void*)BlueButton_2_Sprite);

    Palette pal(DS_BTM, (void*)ButtonBlue_Pal);
    
	//Load backgrounds
	PA_Init16cBg(DS_TOP, BG_TEXT);
	PA_Init16cBg(DS_BTM, BG_TEXT);
	PA_DeleteBg(DS_TOP, BG_BACKGROUND);
	PA_DeleteBg(DS_BTM, BG_BACKGROUND);
	
	//Load palettes
    pal.Load();
	//Load buttons
	for(u8 i=0; i<btnNum; i++){
		btn[i].Load(pal.Id());
		btn[i].SetSpriteBackground(BG_BACKGROUND);
	}
	//Button text
	PA_16cText(DS_BTM, 40, 8, 255, 191, "                      uWars\n                          by\n                 Jelle Licht\n                           &\n       Mick van Gelderen", 9, 3, 200);
	PA_16cText(DS_BTM, 88+6+8,160+6,	 255, 191, "Return",	8, 3, 100);
	PA_16cText(DS_BTM, 64, 110, 255, 191, "Have fun gaming!", 2, 3, 100);
	//Select button
	btn[0].Select();

	mmLoad(MOD_UWARSCREDITS);
	mmStart(MOD_UWARSCREDITS, MM_PLAY_LOOP);

	UpdateButtons(btn, btnNum);

	u8 menuAction = btnNum;

	PA_WaitForVBL(); //clear stylus/pad.newpress

	//get input
	while(menuAction == btnNum){
		for(u8 curBtn=0; curBtn<btnNum; curBtn++){
			if(btn[curBtn].IsTouched() && btn[curBtn].IsActivated()){
				if(btn[curBtn].IsSelected()){
					menuAction = curBtn;
				}
				else{
					SelectAButton(btn, btnNum, curBtn);
				}
			}
		}
		if(Pad.Newpress.A){
			menuAction = GetSelectedButton(btn, btnNum);
		}
		PA_WaitForVBL();
	}

	//clean
	for(u8 curBtn=0; curBtn<btnNum; curBtn++){
		btn[curBtn].Unload();
	}
	pal.Unload();
	PA_ClearTextBg(DS_TOP);
	PA_ClearTextBg(DS_BTM);

	mmStart(MOD_UWARSGAMEHYPER, MM_PLAY_LOOP);
	mmUnload(MOD_UWARSCREDITS);

	return MENU_STAY;
}

void MainMenu(){
	const u8 btnNum = 4;
	Button btn[btnNum];

	MenuFunction functions[btnNum];
	functions[0] = &MainMenuLoadProfile;
	functions[1] = &MainMenuCreateProfile;
	functions[2] = &MainMenuStartUPedia;
	functions[3] = &MainMenuCredits;

	for(u8 curBtn=0; curBtn<btnNum; curBtn++){
		btn[curBtn].Create(DS_BTM, 64, 36+curBtn*32, 8, OBJ_SIZE_16X32, 1, 16, 24, functions[curBtn]);
		btn[curBtn].AddButtonPart((void*)BlueButton_0_Sprite);
		for(u8 i=1; i<7; i++) btn[curBtn].AddButtonPart((void*)BlueButton_1_Sprite);
		btn[curBtn].AddButtonPart((void*)BlueButton_2_Sprite);
	}

    Palette pal(DS_BTM, (void*)ButtonBlue_Pal);
   
	while(true){
		//Load backgrounds
		PA_InitText(DS_TOP, 0);
		PA_Init16cBg(DS_BTM, 0);
		PA_LoadBackground(DS_TOP, BG_BACKGROUND, &uWars_Bg_Menu_T);
		PA_LoadBackground(DS_BTM, BG_BACKGROUND, &uWars_Bg_Menu_B);
		//Load palettes
        pal.Load();
		//Load buttons
		for(u8 i=0; i<btnNum; i++){
			btn[i].Load(pal.Id());
			btn[i].SetSpriteBackground(3);
		}
		//Button text
		PA_16cText(0, 64+6, 36+6,	 255, 191, "Load profile",	8, 3, 100);
		PA_16cText(0, 64+6, 36+6+32, 255, 191, "Create profile",	8, 3, 100);
		PA_16cText(0, 64+6, 36+6+64, 255, 191, "Start uPedia",		8, 3, 100);
		PA_16cText(0, 64+6, 36+6+96, 255, 191, "Credits",		8, 3, 100);

		//Deactivate nonworking buttons
		//btn[1].Deactivate();
		btn[2].Deactivate();
		//btn[3].Deactivate();

		//Select a button
		btn[0].Select();
		
		UpdateButtons(btn, btnNum);

		u8 menuAction = btnNum;

		PA_WaitForVBL();

		while(menuAction == btnNum){ //get input
			for(u8 curBtn=0; curBtn<btnNum; curBtn++){
				if(btn[curBtn].IsTouched() && btn[curBtn].IsActivated()){
					if(btn[curBtn].IsSelected()){
						menuAction = curBtn;
					}
					else{
						for(u8 j=0; j<btnNum; j++) btn[j].Deselect();
						btn[curBtn].Select();
						for(u8 j=0; j<btnNum; j++) btn[j].Update();
					}
				}
			}
			if(Pad.Newpress.A){
				menuAction = GetSelectedButton(btn, btnNum);
			}

			ButtonSelectViaPad(btn, btnNum); //implement arrow key functionality
			PA_WaitForVBL();
		}
		//clean
		for(u8 curBtn=0; curBtn<btnNum; curBtn++){
			btn[curBtn].Unload();
		}
		pal.Unload();
		PA_ClearTextBg(DS_TOP);
		PA_ClearTextBg(DS_BTM);

		btn[menuAction].Go();
	}
}