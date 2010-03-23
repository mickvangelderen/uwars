#include <PA9.h>
#include "Menu.h"

#include "DSspecs.h"
#include "InitFat.h"
#include "all_gfx.h"

#include "Map.h"
#include "Maps.h"
#include "Game.h"

#include "Sound.h"


void UpdateText(Button * btn, u8 btnNum, u8 levelIndex, u8 levelPages){
	PA_16cErase(DS_BTM);
	PA_16cErase(DS_TOP);

	//vars
	char buffer[128]; buffer[127] = '\0';
	u8 sel = GetSelectedButton(btn, btnNum);
	u8 levelIndexStart = 3*levelIndex;

	//static text
	PA_16cText(0, 64+6, 36+6+96, 255, 191, "Cancel",		8, 3, 100);

	//Dynamic text
	snprintf(buffer, 127, "%d/%d", levelIndex+1, levelPages);
	PA_16cText(0, 118, 18, 255, 191, buffer, 8, 3, 127);

	for(u8 i=0; i<3; i++){
		if(i+levelIndexStart < MAP_NUMBER)
			PA_16cText(0, 64+6, 36+6+32*i, 255, 191, g_maps[i+levelIndexStart].name, 8, 3, 100);
	}

	//print level stats
	if(sel >= 0 && sel < 3){
		u8 i = sel+levelIndexStart;
		if(i < MAP_NUMBER){
			snprintf(buffer, 127, "Name:  %s", g_maps[i].name);
			PA_16cText(DS_TOP, 8, 8, 255, 24, buffer, 8, 2, 127);

			snprintf(buffer, 127, "Description:\n%s", g_maps[i].description);
			PA_16cText(DS_TOP, 8, 26, 255, 58, buffer, 8, 2, 127);

			snprintf(buffer, 127, "Map level:  %d", g_maps[i].level);
			PA_16cText(DS_TOP, 8, 78, 255, 191, buffer, 8, 2, 127);

			snprintf(buffer, 127, "Number of cells:  %d", g_maps[i].nCells);
			PA_16cText(DS_TOP, 8, 96, 255, 191, buffer, 8, 2, 127);

			snprintf(buffer, 127, "Map difficulty:  %d", g_maps[i].difficulty);
			PA_16cText(DS_TOP, 8, 114, 255, 191, buffer, 8, 2, 127);

			snprintf(buffer, 127, "Map width:  %d", g_maps[i].width);
			PA_16cText(DS_TOP, 8, 132, 255, 191, buffer, 8, 2, 127);

			snprintf(buffer, 127, "Map height:  %d", g_maps[i].height);
			PA_16cText(DS_TOP, 8, 150, 255, 191, buffer, 8, 2, 127);
		}
	}
	else if(sel == 3){
		PA_16cText(DS_TOP, 8, 8, 256-8, 192-8, "Display the previous page with levels", 8, 2, 127);
	}
	else if(sel == 4){
		PA_16cText(DS_TOP, 8, 8, 256-8, 192-8, "Display the next page with levels", 8, 2, 127);
	}
	else if(sel == btnNum-1){
		PA_16cText(DS_TOP, 8, 8, 256-8, 192-8, "Takes you back to the main menu", 8, 2, 127);
	}
}

u8 LevelSelectMenu(void){
	const u8 btnNum = 6;
	Button btn[btnNum];

	for(u8 curBtn=0; curBtn<3; curBtn++){
		btn[curBtn].Create(DS_BTM, 64, 36+curBtn*32, 8, OBJ_SIZE_16X32, 1, 16, 24, NULL);
		btn[curBtn].AddButtonPart((void*)BlueButton_0_Sprite);
		for(u8 i=1; i<7; i++) btn[curBtn].AddButtonPart((void*)BlueButton_1_Sprite);
		btn[curBtn].AddButtonPart((void*)BlueButton_2_Sprite);
	}
	//previous button
	btn[3].Create(DS_BTM, 16, 36+32*1, 2, OBJ_SIZE_16X32, 1, 16, 24, NULL);
	btn[3].AddButtonPart((void*)BluePrevButton_0_Sprite);
	btn[3].AddButtonPart((void*)BluePrevButton_1_Sprite);

	//next button
	btn[4].Create(DS_BTM, 256-32-16, 36+32*1, 2, OBJ_SIZE_16X32, 1, 16, 24, NULL);
	btn[4].AddButtonPart((void*)BlueNextButton_0_Sprite);
	btn[4].AddButtonPart((void*)BlueNextButton_1_Sprite);

	//cancel button
	btn[5].Create(DS_BTM, 64, 36+32*3, 8, OBJ_SIZE_16X32, 1, 16, 24, NULL);
	btn[5].AddButtonPart((void*)BlueButton_0_Sprite);
	for(u8 i=1; i<7; i++) btn[5].AddButtonPart((void*)BlueButton_1_Sprite);
	btn[5].AddButtonPart((void*)BlueButton_2_Sprite);

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

		u8 levelIndex = 0;
		u8 levelPages;
		if(MAP_NUMBER%3 == 0){
			levelPages = MAP_NUMBER/3;
		}
		else{
			levelPages = (MAP_NUMBER-(MAP_NUMBER%3))/3 + 1;
		}
		
		//Deactivate nonworking buttons
		btn[3].Deactivate();
		if(levelIndex+1 == levelPages) btn[4].Deactivate();
		for(u8 i=3*levelIndex; i<3*levelIndex+3; i++){
			if(i >= MAP_NUMBER || g_maps[i].level > g_playerProfile->Level() || g_maps[i].nCells == 0)
			  btn[i].Deactivate();
		}

		//Select a button
		for(u8 i=0; i<btnNum; i++){
			if(btn[i].IsActivated()){
				btn[i].Select();
				break;
			}
		}

		UpdateText(btn, btnNum, levelIndex, levelPages);
		UpdateButtons(btn, btnNum);

		u8 menuAction = btnNum;

		PA_WaitForVBL();

		while(menuAction == btnNum){
			//stylus
			for(u8 curBtn=0; curBtn<btnNum; curBtn++){
				if(btn[curBtn].IsTouched() && btn[curBtn].IsActivated()){
					if(btn[curBtn].IsSelected()) menuAction = curBtn;
					else SelectAButton(btn, btnNum, curBtn);
				}
			}
			//pad
			if(Pad.Newpress.A){
				menuAction = GetSelectedButton(btn, btnNum);
			}
			if(Pad.Newpress.B && btn[btnNum-1].IsActivated()){
				if(btn[btnNum-1].IsSelected()) menuAction = btnNum-1;
				else SelectAButton(btn, btnNum, btnNum-1);
			}
			if(Pad.Newpress.Up){
				s8 i = GetSelectedButton(btn, btnNum);
				if(i>=0 && i<3){
					i--;
					while(!btn[i].IsActivated() && i>=0) i--;
				}
				else if(i==5){
					i=2;
					while(!btn[i].IsActivated() && i>=0) i--;
					
				}
				if(i>=0 && i<btnNum) SelectAButton(btn, btnNum, i);
				UpdateText(btn, btnNum, levelIndex, levelPages);
			}
			else if(Pad.Newpress.Down){
				s8 i = GetSelectedButton(btn, btnNum)+1;
				while((!btn[i].IsActivated() || i==3|| i==4) && i<btnNum) i++;
				if(i>=0 && i<btnNum)	SelectAButton(btn, btnNum, i);
				UpdateText(btn, btnNum, levelIndex, levelPages);
			}
			else if(Pad.Newpress.Left){
				s8 i = GetSelectedButton(btn, btnNum);
				if(((i>=0 && i<3)||(i==5)) && btn[3].IsActivated()) SelectAButton(btn, btnNum, 3);
				else if(i==4){
					if(btn[0].IsActivated()) SelectAButton(btn, btnNum, 0);
					else if(btn[1].IsActivated()) SelectAButton(btn, btnNum, 1);
					else if(btn[2].IsActivated()) SelectAButton(btn, btnNum, 2);
					else if(btn[btnNum-1].IsActivated()) SelectAButton(btn, btnNum, btnNum-1);
				}
				UpdateText(btn, btnNum, levelIndex, levelPages);
			}
			else if(Pad.Newpress.Right){
				s8 i = GetSelectedButton(btn, btnNum);
				if(((i>=0 && i<3)||(i==5)) && btn[4].IsActivated()) SelectAButton(btn, btnNum, 4);
				else if(i==3){
					if(btn[0].IsActivated()) SelectAButton(btn, btnNum, 0);
					else if(btn[1].IsActivated()) SelectAButton(btn, btnNum, 1);
					else if(btn[2].IsActivated()) SelectAButton(btn, btnNum, 2);
					else if(btn[btnNum-1].IsActivated()) SelectAButton(btn, btnNum, btnNum-1);
				}
				UpdateText(btn, btnNum, levelIndex, levelPages);
			}
			//next prev button menuaction filter
			if(menuAction == 3 || menuAction == 4){
				if(menuAction == 3){ //pressed previous
					if(levelIndex+1 == levelPages){ //next button is deactivated
						btn[4].Activate(); //make useable again
					}
					levelIndex--; //go to the previous page
				}
				else if(menuAction == 4){ //next button was clicked
					if(levelIndex == 0){ //prev button was deactivated
						btn[3].Activate(); //activate prev button
					}
					levelIndex++; //next page
				}
				
				for(u8 i=0; i<3; i++){
					if((i+levelIndex*3) >= MAP_NUMBER || g_maps[i+levelIndex*3].level > g_playerProfile->Level() || g_maps[i+levelIndex*3].nCells == 0)
					  btn[i].Deactivate();
					else
					  btn[i].Activate();
				}

				if(levelIndex == 0){ //we are one the first page
					btn[3].Deactivate(); //deactivate previous
					if(btn[0].IsActivated()) SelectAButton(btn, btnNum, 0); //select level if possible
					else if(btn[1].IsActivated()) SelectAButton(btn, btnNum, 1);
					else if(btn[2].IsActivated()) SelectAButton(btn, btnNum, 2);
					else if(btn[4].IsActivated() && levelIndex+1 != levelPages) SelectAButton(btn, btnNum, 3);//select next if not on the last page
					else SelectAButton(btn, btnNum, btnNum-1); //select cancel
				}
				if(levelIndex+1 == levelPages){
					btn[4].Deactivate();
					if(btn[0].IsActivated()) SelectAButton(btn, btnNum, 0);
					else if(btn[1].IsActivated()) SelectAButton(btn, btnNum, 1);
					else if(btn[2].IsActivated()) SelectAButton(btn, btnNum, 2);
					else if(btn[3].IsActivated() && levelIndex != 0) SelectAButton(btn, btnNum, 3);
					else SelectAButton(btn, btnNum, btnNum-1);
				}
				UpdateText(btn, btnNum, levelIndex, levelPages);
				UpdateButtons(btn, btnNum);

				menuAction = btnNum;//don't quit the menu loop
			}
			PA_WaitForVBL();
		}//menu loop


		//clean
		for(u8 curBtn=0; curBtn<btnNum; curBtn++){
			btn[curBtn].Unload();
		}
		pal.Unload();
		PA_ClearTextBg(DS_TOP);
		PA_ClearTextBg(DS_BTM);



		if(menuAction >= 0 && menuAction < 3){
			mmStop();
			mmUnload(MOD_UWARSGAMEHYPER);

			g_map = &g_maps[menuAction+levelIndex*3];
			stay = Game();

			mmStop();
			mmLoad(MOD_UWARSGAMEHYPER);
			mmStart(MOD_UWARSGAMEHYPER, MM_PLAY_LOOP);
		}
		else{
			stay = MENU_EXIT; //cancel button
		}
	}
	return MENU_STAY; //stay in the parent menu
}