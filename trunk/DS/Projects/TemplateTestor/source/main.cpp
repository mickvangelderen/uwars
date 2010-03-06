////////////////////////////
// PAlib project template //
////////////////////////////

#include <PA9.h>
//#include "fat.h"
#include <maxmod9.h> // MaxMod include

// Include the soundbank
#include "soundbank_bin.h"
#include "soundbank.h"

#define LIBPATH C:/SVN/DS/Libraries
#define LIB(x) <LIBPATH/x>

//#include LIB(efs_lib.h)
//#include LIB(efs_lib.c)
#include LIB(DSspecs.h)

#include LIB(STDfunctions.h)
#include LIB(STDfunctions.cpp)

#include LIB(BTNlib.h)
#include LIB(BTNlib.cpp)

#include LIB(UnitBase.h)
#include LIB(UnitBase.cpp)

#include LIB(Team.h)
#include LIB(Team.cpp)

#include LIB(UnitSpriteData.h)


#include "all_gfx.h"

#include "Blob.h"
#include "Cell.h"
#include "Selection.h"

bool FAT_ENABLED;

Cell * cells;
Blob * blobs;

int main(){
	ScreenOut(DS_BTM);
	ScreenOut(DS_TOP);

	PA_Init();
	PA_InitVBL();

    PA_InitText(DS_TOP, BG_TEXT);
	PA_Init16cBg(DS_BTM, BG_TEXT);

	mmInitDefaultMem((mm_addr)soundbank_bin);
	mmLoad(MOD_ZELDA);
	//mmStart(MOD_ZELDA, MM_PLAY_LOOP);

	//PA_InitRand();

	/*
	FAT_ENABLED = fatInitDefault();
	PA_WaitForVBL();PA_WaitForVBL();PA_WaitForVBL();

	if(!FAT_ENABLED){
		ErrorMsg(DS_TOP, "Fat init error");
		ClearText();
	}
	*/

	//PA_SetBgColor(DS_BTM,PA_RGB(19,31,24));
	
	PA_InitSpriteExtPrio(true);

	Blob::CreateSpriteData((void*)Blob_Sprite, (void*)Blob_Pal, fixed(16), fixed(16), OBJ_SIZE_32X32, COLOR_MODE_256);
	Cell::CreateSpriteData((void*)Cell_Sprite, (void*)Cell_Pal, fixed(16), fixed(16), OBJ_SIZE_32X32, COLOR_MODE_256);
	

	cells = new Cell[MAX_CELLS];
	blobs = new Blob[MAX_BLOBS];

	for(u8 i=0; i<5; i++){
		cells[i].Set(fixed(32*i+16), fixed(16), fixed(16), TEAM_RED, fixed(10), fixed(100), 50);
	}
	for(u8 i=5; i<8; i++){
		cells[i].Set(fixed(32*(i-5)+16), fixed(60), fixed(16), TEAM_BLUE, fixed(10), fixed(200), 5);
	}
	for(u8 i=8; i<12; i++){
		cells[i].Set(fixed(32*(i-8)+16), fixed(110), fixed(16), TEAM_NEUTRAL, fixed(15), fixed(30), 5);
	}

	CellSelection cellSelection;

	u8 nRedBlobs, nBlueBlobs, nRedCells, nBlueCells;
	u32 redGR, blueGR;

	while(true){
		
		PA_16cClearZone(DS_BTM, 0, 0, DS_WIDTH, DS_HEIGHT);

		//update selection
		cellSelection.Update();

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
		for(u8 i=0; i<MAX_CELLS; i++){
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

		//statistics
		PA_OutputText(DS_TOP, 0, 0, "Red Cells #: %d  ", nRedCells);
		PA_OutputText(DS_TOP, 0, 1, "Red Blobs #: %d  ", nRedBlobs);

		PA_OutputText(DS_TOP, 0, 3, "Blue Cells #: %d  ", nBlueCells);
		PA_OutputText(DS_TOP, 0, 4, "Blue Blobs #: %d  ", nBlueBlobs);

		PA_OutputText(DS_TOP, 0, 6, "Red Growthrate: %d  ", redGR);
		PA_OutputText(DS_TOP, 0, 7, "Blue Growthrate: %d  ", blueGR);

		//ai
		if(blueGR<redGR){
			bool send = false;

			for(u8 i=0; i<MAX_CELLS && send==false; i++){
				if(cells[i].Team() == TEAM_BLUE){
					for(u8 j=0; j<MAX_CELLS && send==false;j++){
						if(cells[j].Team() == TEAM_RED and cells[j].Str() + 100 < cells[i].Str()){
							cells[i].Send(&cells[j]);
							send = true;
						}
					}
				}
			}
			if(send == false){
				for(u8 i=0, j=0; i<MAX_CELLS && send==false; i++){
					if(cells[i].Team() == TEAM_BLUE){
						for(; j<MAX_CELLS && send==false;j++){
							if(cells[j].Team() == TEAM_RED){
								cells[i].Send(&cells[j]);
								}
						}
					}
				}
			}
		}



		//check for win
		if(nBlueCells == 0 && nBlueBlobs == 0){
			ErrorMsg(DS_TOP, "Red WINS!");
			cells[0].SetTeam(TEAM_BLUE);
			cells[2].SetTeam(TEAM_BLUE);
			cells[4].SetTeam(TEAM_BLUE);
			cells[6].SetTeam(TEAM_BLUE);
			cells[8].SetTeam(TEAM_BLUE);
			cells[10].SetTeam(TEAM_BLUE);
			cellSelection.Reset();
		}
		else if(nRedCells == 0 && nRedBlobs == 0){
			ErrorMsg(DS_TOP, "Blue WINS!");
		}
		PA_WaitForVBL();
	}
	
}

