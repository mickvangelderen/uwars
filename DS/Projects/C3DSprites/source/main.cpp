#include <PA9.h>
#include "all_gfx.h"

//#include "C3DSpriteData.h"
#include "IdGenerator.h"
#include "Cell.h"
#include "Blob.h"

#include "STDfunctions.h"

int main(){
	PA_Init();
	PA_InitVBL();

	PA_Init3D();
	PA_Reset3DSprites();

    PA_InitText(DS_TOP, 1);
	PA_Init16cBg(DS_BTM, 1);

	PA_SetBgPrio(DS_BTM, 1, 0);
	PA_SetBgPrio(DS_BTM, 0, 1);

	PA_LoadBackground(DS_TOP, 3, &BgGameStatic);
	PA_LoadBackground(DS_BTM, 3, &BgGameStatic);

	Cell::CreateSpriteData((void*)Cell_Texture, NULL, 32, 32, TEX_16BITS);
	Blob::CreateSpriteData((void*)Blob_Texture, NULL, 32, 32, TEX_16BITS);

	Cell * g_cells = new Cell[MAX_CELLS];
	Blob * g_blobs = new Blob[MAX_BLOBS];

	g_cells[0].Set(32<<8, 64<<8, TEAM_BLUE, 10<<8, 130<<8, 256);
	
	g_cells[1].Set(96<<8, 64<<8, TEAM_RED, 10<<8, 50<<8, 5);
	
	g_cells[2].Set(160<<8, 64<<8, TEAM_RED, 10<<8, 1<<16, 30);
	
	g_blobs[0].Set(200<<8, 64<<8, TEAM_BLUE, &g_cells[1], fixed(1), 200, TRAVEL);
	g_blobs[1].Set(210<<8, 64<<8, TEAM_RED, &g_cells[1], fixed(1), 200, TRAVEL);
	g_blobs[2].Set(220<<8, 64<<8, TEAM_NEUTRAL, &g_cells[1], fixed(1), 200, TRAVEL);

	s32 t=0;
	while(true){
		PA_16cClearZone(DS_BTM, 0, 0, DS_WIDTH, DS_HEIGHT);
		//update cells
		for(u16 i=0; i<MAX_CELLS; i++) g_cells[i].Update();
		for(u16 i=0; i<MAX_BLOBS; i++) g_blobs[i].Update();

		for(u8 i=0;i<3;i++){
			if(g_cells[i].IsTouched()){
				if(i != 0){
					g_cells[0].Send(&g_cells[i], g_blobs);
				}
				PA_OutputText(DS_TOP, 0, 0, "TOUCHED cell %d (ID)", g_cells[i].SpriteId());
				PA_OutputText(DS_TOP, 0, 1, " str = %d", g_cells[i].Strength());
				PA_OutputText(DS_TOP, 0, 2, " capacity = %d", g_cells[i].Capacity());
				PA_OutputText(DS_TOP, 0, 3, " team = %d", Team2Id(g_cells[i].Team()));
				PA_OutputText(DS_TOP, 0, 4, " growth rate = %d", g_cells[i].GrowthRate());
				t=0;
			}
		}
		if(t>=60){
			PA_ClearTextBg(DS_TOP);
		}
		else{
			t++;
		}
		PA_Clear16bitBg(DS_BTM);
		PA_WaitForVBL();
		PA_3DProcess();
	}
}
