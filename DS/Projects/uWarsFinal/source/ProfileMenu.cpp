#include <PA9.h>
#include "ProfileMenu.h"
#include "DSspecs.h"
#include "InitFat.h"
#include "../gfx/all_gfx.h"

#include "Menu.h"
#include "Palette.h"
#include "Button.h"

#include "PlayerProfile.h"
#include "LevelSelectMenu.h"

PlayerProfile * g_playerProfile = NULL;
u8 g_playerProfileId = g_nPlayerProfiles;
PlayerProfile * g_playerProfiles = NULL;

u8 ProfileStartLevel(){
	LevelSelectMenu();
	/*PA_OutputSimpleText(DS_TOP, 4, 4, "No levels");
	PA_DeleteBg(DS_BTM, BG_BACKGROUND);
	PA_DeleteBg(DS_TOP, BG_BACKGROUND);
	PA_WaitForVBL();
	PA_WaitFor(Stylus.Newpress);*/
	return MENU_STAY;
};

u8 AreYouSureYouWantToReturn(){
	const u8 btnNum = 2;
	Button btn[btnNum];

	btn[0].Create(DS_BTM, 104, 60, 3, OBJ_SIZE_16X32, 1, 16, 24, NULL);
	btn[0].AddButtonPart((void*)BlueButton_0_Sprite);
	btn[0].AddButtonPart((void*)BlueButton_1_Sprite);
	btn[0].AddButtonPart((void*)BlueButton_2_Sprite);

	btn[1].Create(DS_BTM, 104, 100, 3, OBJ_SIZE_16X32, 1, 16, 24, NULL);
	btn[1].AddButtonPart((void*)BlueButton_0_Sprite);
	btn[1].AddButtonPart((void*)BlueButton_1_Sprite);
	btn[1].AddButtonPart((void*)BlueButton_2_Sprite);

    Palette pal(DS_BTM, (void*)ButtonBlue_Pal);
    
	while(true){
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
		PA_16cText(DS_BTM, 10, 20+6, 255, 191, "Are you sure you want to return?", 9, 3, 100);
		PA_16cText(DS_BTM, 104+10, 60+7,	 255, 191, "Yes",	8, 3, 100);
		PA_16cText(DS_BTM, 104+14, 100+7,  255, 191, "No",	8, 3, 100);
		PA_16cText(DS_BTM, 25, 157, 255, 191, "Unsaved progress will be lost", 2, 3, 100);
		//Select button
		btn[0].Select();

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
			if(Pad.Newpress.B){
				if(btn[1].IsSelected()){
					menuAction = 1;
				}
				else{
					SelectAButton(btn, btnNum, 1);
				}
			}
			ButtonSelectViaPad(btn, btnNum);
			PA_WaitForVBL();
		}

		//clean
		for(u8 curBtn=0; curBtn<btnNum; curBtn++){
			btn[curBtn].Unload();
		}
		pal.Unload();
		PA_ClearTextBg(DS_TOP);
		PA_ClearTextBg(DS_BTM);

		switch(menuAction){
		case 0:
			return MENU_EXIT;
			break;
		case 1:
			return MENU_STAY;
			break;
		default:
			break;
		}
	}
}
u8 ProfileViewStats(){
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
	//text
	char buffer[128]; buffer[127] = '\0';
	PA_16cText(DS_TOP, 16, 16, 255, 191, "View stats", 8, 3, 100);

	snprintf(buffer, 127, "Your profile name is: %s", g_playerProfile->Name());
	PA_16cText(DS_BTM, 8, 8, 255, 191, buffer, 8, 3, 127);
	snprintf(buffer, 127, "You are level: %d", g_playerProfile->Level());
	PA_16cText(DS_BTM, 8, 40, 255, 191, buffer, 8, 3, 127);
	snprintf(buffer, 127, "Your [exp/max]: [%d/%d]", g_playerProfile->Experience(), PLAYER_PROFILE_EXP_FOR_LVLUP);
	PA_16cText(DS_BTM, 8, 72, 255, 191, buffer, 8, 3, 127);

	PA_16cText(DS_BTM, 88+6+8,160+6,	 255, 191, "Return",	8, 3, 100);
	
	//Select button
	btn[0].Select();

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
	return MENU_STAY;
}

u8 ProfileSave(){
	if(g_playerProfiles != NULL && g_playerProfile != NULL && g_playerProfileId < g_nPlayerProfiles){
		g_playerProfiles[g_playerProfileId].Copy(*g_playerProfile);
		SavePlayerProfiles(g_playerProfiles, g_nPlayerProfiles);
	}
	PA_WaitFor(Stylus.Newpress||Pad.Newpress.Anykey);
	return MENU_STAY;
}
u8 ProfileMenu(){
	const u8 btnNum = 4;
	Button btn[btnNum];

	MenuFunction functions[btnNum];
	functions[0] = &ProfileStartLevel;
	functions[1] = &ProfileViewStats;
	functions[2] = &ProfileSave;
	functions[3] = &AreYouSureYouWantToReturn;

	for(u8 curBtn=0; curBtn<btnNum; curBtn++){
		btn[curBtn].Create(DS_BTM, 64, 36+curBtn*32, 8, OBJ_SIZE_16X32, 1, 16, 24, functions[curBtn]);
		btn[curBtn].AddButtonPart((void*)BlueButton_0_Sprite);
		for(u8 i=1; i<7; i++) btn[curBtn].AddButtonPart((void*)BlueButton_1_Sprite);
		btn[curBtn].AddButtonPart((void*)BlueButton_2_Sprite);
	}

    Palette pal(DS_BTM, (void*)ButtonBlue_Pal);
    
	u8 stay = MENU_STAY;
	while(stay == MENU_STAY){
		//Load backgrounds
		PA_Init16cBg(DS_TOP, 0);
		PA_Init16cBg(DS_BTM, 0);
		PA_LoadBackground(DS_BTM, BG_BACKGROUND, &uWars_Bg_Menu_B);
		//Load palettes
        pal.Load();
		//Load buttons
		for(u8 i=0; i<btnNum; i++){
			btn[i].Load(pal.Id());
			btn[i].SetSpriteBackground(3);
		}
		//Button text
		PA_16cText(0, 64+6, 36+6,	 255, 191, "Start level",	8, 3, 100);
		PA_16cText(0, 64+6, 36+6+32, 255, 191, "View stats",	8, 3, 100);
		PA_16cText(0, 64+6, 36+6+64, 255, 191, "Save game",		8, 3, 100);
		PA_16cText(0, 64+6, 36+6+96, 255, 191, "Return",		8, 3, 100);
		
		//Other text
		char buffer[128];
		snprintf(buffer, 127, "Hello %s!\nHow are you doing?",  g_playerProfile->Name());
		buffer[127] = '\0';
		PA_16cText(DS_TOP, 32, 32, 255, 191, buffer, 8, 3, 127);
		
		//Deactivate nonworking buttons
		if(!FAT_INIT_SUCCESS) btn[2].Deactivate();

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
			if(Pad.Newpress.B){
				if(btn[btnNum-1].IsSelected()){
					menuAction = btnNum-1;
				}
				else{
					SelectAButton(btn, btnNum, btnNum-1);
				}
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

		stay = btn[menuAction].Go();
	}
	return MENU_EXIT; //exit the parent menu (profile selection)
}
