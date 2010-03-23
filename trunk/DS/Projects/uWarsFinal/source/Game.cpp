#include <PA9.h>
#include "DSspecs.h"
#include "C3DSpriteData.h"
#include "all_gfx.h"
#include "Cell.h"
#include "Blob.h"
#include "Scroll.h"
#include "Map.h"
#include "Maps.h"
#include "Selection.h"

#include "ProfileMenu.h"
#include "PlayerProfile.h"

#include "Sound.h"

CellSelection * g_select = NULL;
Blob * g_blobs = NULL;
Cell * g_cells = NULL;

u8 Game(){
	if(g_map == NULL) return 0;
	
	PA_ResetBgSys();

	//PA_ResetSpriteSys();
	PA_Init3D();
	PA_Reset3DSprites();
	
	PA_Init16cBg(DS_TOP, 1);
	PA_Init16cBg(DS_BTM, 1);

	PA_SetBgPrio(DS_BTM, 1, 0); //text before 3dsprites
	PA_SetBgPrio(DS_BTM, 0, 1); //3dsprites behind text :)

	mmLoad(MOD_UWARSMENUHAPPY);
	mmStart(MOD_UWARSMENUHAPPY, MM_PLAY_LOOP);

	PA_LoadBackground(DS_BTM, BG_BACKGROUND, &BgGameStatic);

	//load cell & blob palettes
	for(u8 i=0; i<3; i++){
		g_cellGfx[i].Load();
		g_blobGfx[i].Load();
	}
	
	//create cells and blobs
	Cell cells[MAX_CELLS];
	Blob blobs[MAX_BLOBS];
	CellSelection select;

	g_cells = cells;
	g_blobs = blobs;
	g_select = &select;

	//initialize scrolling
	InitScroll(g_map->width, g_map->height, 2);

	//create cells
	for(u16 i=0; i<g_map->nCells; i++){
		cells[i].Create(
			g_map->cells[i].x, 
			g_map->cells[i].y, 
			g_map->cells[i].team, 
			g_map->cells[i].strength, 
			g_map->cells[i].capacity, 
			g_map->cells[i].growthRate);
	}

	//load cells and blobs (create sprites etc.)
	for(u16 i=0; i<MAX_CELLS; i++) cells[i].Load();
	for(u16 i=0; i<MAX_BLOBS; i++) blobs[i].Load();
	select.Load();
	
	u8 nRedBlobs, nBlueBlobs, nRedCells, nBlueCells;
	u32 redGR, blueGR;

	//level data
	char buffer[128]; buffer[127] = '\0';
	snprintf(buffer, 127, "Name:  %s", g_map->name);
	PA_16cText(DS_TOP, 8, 8, 255, 24, buffer, 8, 2, 127);

	snprintf(buffer, 127, "Description:\n%s", g_map->description);
	PA_16cText(DS_TOP, 8, 26, 255, 58, buffer, 8, 2, 127);

	snprintf(buffer, 127, "Map level:  %d", g_map->level);
	PA_16cText(DS_TOP, 8, 78, 255, 191, buffer, 8, 2, 127);

	snprintf(buffer, 127, "Number of cells:  %d", g_map->nCells);
	PA_16cText(DS_TOP, 8, 96, 255, 191, buffer, 8, 2, 127);

	snprintf(buffer, 127, "Map difficulty:  %d", g_map->difficulty);
	PA_16cText(DS_TOP, 8, 114, 255, 191, buffer, 8, 2, 127);

	snprintf(buffer, 127, "Map width:  %d", g_map->width);
	PA_16cText(DS_TOP, 8, 132, 255, 191, buffer, 8, 2, 127);

	snprintf(buffer, 127, "Map height:  %d", g_map->height);
	PA_16cText(DS_TOP, 8, 150, 255, 191, buffer, 8, 2, 127);

	mmEffectCancelAll();
	
	TEAM winner = TEAM_NONE;
	//gameloop
	while(winner == TEAM_NONE){
		UpdateScroll();

		//count variables
		nRedCells = 0; nRedBlobs = 0;
		nBlueCells = 0; nBlueBlobs = 0;
		redGR = 0; blueGR = 0;

		//update blobs
		for(u8 i=0; i<MAX_BLOBS; i++){
			blobs[i].Update();
			switch(blobs[i].Team()){
			case TEAM_RED:
				nRedBlobs++;
				break;
			case TEAM_BLUE:
				nBlueBlobs++;
				break;
			default:
				break;
			}
		}

		//update cells
		for(u16 i=0; i<MAX_CELLS; i++){
			cells[i].Update();
			switch(cells[i].Team()){
			case TEAM_RED:
				nRedCells++;
				redGR+=cells[i].GrowthRate();
				break;
			case TEAM_BLUE:
				nBlueCells++;
				blueGR+=cells[i].GrowthRate();
				break;
			default:
				break;
			}
		}
		select.Update(cells, blobs);

		/*//statistics
		PA_OutputText(DS_TOP, 0, 0, "Red Cells #:  %d  ", nRedCells);
		PA_OutputText(DS_TOP, 0, 1, "Red Blobs #:  %d  ", nRedBlobs);

		PA_OutputText(DS_TOP, 0, 3, "Blue Cells #: %d  ", nBlueCells);
		PA_OutputText(DS_TOP, 0, 4, "Blue Blobs #: %d  ", nBlueBlobs);

		PA_OutputText(DS_TOP, 0, 6, "Red Growthrate:  %d  ", redGR);
		PA_OutputText(DS_TOP, 0, 7, "Blue Growthrate: %d  ", blueGR);*/

		u16 tempInt;
		//ai
		switch(g_map->difficulty){
		case MAP_DIFFICULTY_NONE:
			break;
		case MAP_DIFFICULTY_EASY:
			tempInt = PA_RandMax(g_map->nCells); 
			if(PA_RandMax(80) == 0 && cells[tempInt].Team() == TEAM_BLUE){
				cells[tempInt].AttackCell(&cells[(tempInt+1)%g_map->nCells], blobs);
			}
			break;
		case MAP_DIFFICULTY_MEDIUM:
			tempInt = PA_RandMax(g_map->nCells-1); 
			if(PA_RandMax(40) == 0 && cells[tempInt].Team() == TEAM_BLUE){
				cells[tempInt].AttackCell(&cells[PA_RandMax(g_map->nCells-1)], blobs);
			}
			break;
		case MAP_DIFFICULTY_HARD:

			break;
		case MAP_DIFFICULTY_INSANE:

			break;
		default:
			break;
		}

		PA_3DProcess();//update 3D

		//check for win
		if(nBlueCells == 0 && nBlueBlobs == 0){
			winner = TEAM_RED;
		}
		else if(nRedCells == 0 && nRedBlobs == 0){
			winner = TEAM_BLUE;
		}

		PA_InfLargeScrollXY(DS_BTM, BG_BACKGROUND, scrollx, scrolly);//scroll bg
		PA_WaitForVBL();//wait
		PA_16cErase(DS_BTM);//clear text
	}
	mmLoad(MOD_UWARSHIGHSCORES);
	mmLoad(MOD_UWARSFAIL);


	PA_16cErase(DS_TOP);
	if(winner == TEAM_RED){
		mmStart(MOD_UWARSHIGHSCORES, MM_PLAY_LOOP);
		PA_16cText(DS_TOP, 20, 8, 255, 191, "WE HAVE A WINNER!!!", 4, 4, 100);
		PA_16cText(DS_TOP, 8, 28, 255, 191, "congratulations, you have completed this level succesfully :)", 1, 2, 100);

		if(g_playerProfile->Level() + 5 > g_map->level && g_playerProfile->Level() < g_map->level + 5){
			u8 exp = 10;
			if(g_playerProfile->Level() > g_map->level) exp = ((5 + g_map->level - g_playerProfile->Level())<<1);
			char buffer[128]; buffer[127] = '\0';
			snprintf(buffer, 127, "Because you are level %d you have earned %d experience points!", g_playerProfile->Level(), exp);
			PA_16cText(DS_TOP, 8, 72, 255, 191, buffer, 1, 2, 127);
			g_playerProfile->AddExperience(exp);
			if(g_playerProfile->CheckLevelUp()){
				snprintf(buffer, 127, "You have advanced to level %d with %d experience! You've unlocked more levels to play!", g_playerProfile->Level(), g_playerProfile->Experience());
				PA_16cText(DS_TOP, 8, 124, 255, 191, buffer, 1, 2, 127);
			}
		}
		
	}
	else if(winner == TEAM_BLUE){
		mmStart(MOD_UWARSFAIL, MM_PLAY_LOOP);
		PA_16cText(DS_TOP, 8, 8, 255, 191, "You actually managed to lose...", 2, 4, 100);
		PA_16cText(DS_TOP, 8, 40, 255, 191, "Please try a new tactic", 1, 2, 100);
	}

	for(u8 i=0; i<60; i++) PA_WaitForVBL();
	PA_16cText(DS_TOP, 46, 170, 255, 191, "Press anything to continue", 8, 2, 100);
	PA_WaitFor(Stylus.Newpress||Pad.Newpress.Anykey);

	mmUnload(MOD_UWARSHIGHSCORES);
	mmUnload(MOD_UWARSFAIL);

	for(u8 i=0; i<3; i++){
		g_cellGfx[i].Unload();
		g_blobGfx[i].Unload();
	}
	
	for(u16 i=0; i<MAX_CELLS; i++) cells[i].Unload();
	for(u16 i=0; i<MAX_BLOBS; i++) blobs[i].Unload();
	select.Unload();

	PA_SetBgPrio(DS_BTM, 1, 1);
	PA_SetBgPrio(DS_BTM, 0, 0);

	PA_DeleteBg(DS_BTM, BG_BACKGROUND);

	PA_ClearTextBg(DS_TOP);
	PA_ClearTextBg(DS_BTM);

	PA_Reset3DSprites();
	PA_Init2D();
	return 0;
}
