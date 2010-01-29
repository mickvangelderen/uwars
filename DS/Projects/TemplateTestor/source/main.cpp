////////////////////////////
// PAlib project template //
////////////////////////////

#include <PA9.h>
//#include "fat.h"

#define LIBPATH C:/SVN/DS/Libraries
#define LIB(x) <LIBPATH/x>

//#include LIB(efs_lib.h)
//#include LIB(efs_lib.c)
#include LIB(DSspecs.h)
#include LIB(STDfunctions.h)
#include LIB(STDfunctions.cpp)
#include LIB(BTNlib.h)
#include LIB(BTNlib.cpp)

#include LIB(UnitBase.cpp)
#include LIB(Team.cpp)

/*
#include LIB(UnitBase.h)
#include LIB(Team.h)
#include LIB(UnitSpriteData.h)
*/

#include "all_gfx.h"

#include "Blob.h"
#include "Cell.h"
#include "Selection.h"

bool FAT_ENABLED;

Cell * cells;
Blob * blobs;

int main(){
	ScreenOut(DS_BOTTOM);
	ScreenOut(DS_TOP);

	PA_Init();
	PA_InitVBL();

    PA_InitText(DS_TOP, BG_TEXT);
	PA_InitText(DS_BOTTOM, BG_TEXT);

	//PA_InitRand();

	/*
	FAT_ENABLED = fatInitDefault();
	PA_WaitForVBL();PA_WaitForVBL();PA_WaitForVBL();

	if(!FAT_ENABLED){
		ErrorMsg(DS_TOP, "Fat init error");
		ClearText();
	}
*/
	PA_SetBgColor(DS_BTM,PA_RGB(19,31,24));
	PA_InitSpriteExtPrio(true);

	Blob::CreateSpriteData((void*)Blob_Sprite, (void*)Blob_Pal, fixed(16), fixed(16), OBJ_SIZE_32X32, COLOR_MODE_256);
	Cell::CreateSpriteData((void*)Cell_Sprite, (void*)Cell_Pal, fixed(16), fixed(16), OBJ_SIZE_32X32, COLOR_MODE_256);

	cells = new Cell[MAX_CELLS];
	blobs = new Blob[MAX_BLOBS];



	for(u8 i=0; i<5; i++){
		cells[i].Set(fixed(32*i+16), fixed(16), fixed(16), TEAM_RED, fixed(10), fixed(10), 10);
	}
	for(u8 i=5; i<8; i++){
		cells[i].Set(fixed(32*(i-5)+16), fixed(60), fixed(16), TEAM_BLUE, fixed(10), fixed(10), 10);
	}
	for(u8 i=8; i<12; i++){
		cells[i].Set(fixed(32*(i-8)+16), fixed(110), fixed(16), TEAM_NEUTRAL, fixed(10), fixed(10), 10);
	}

	CellSelection cellSelection;

	while(true){
		cellSelection.Update();
		for(u8 i=0; i<MAX_BLOBS; i++)
			blobs[i].Update();
		
		PA_WaitForVBL();
	}
}

