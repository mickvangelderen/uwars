////////////////////////////
// PAlib project template //
////////////////////////////

#include <PA9.h>
//#include "fat.h"

#define LIBPATH C:/SVN/DS/Libraries
#define LIB(x) <LIBPATH/x>

//#include LIB(efs_lib.h)
#include LIB(DSspecs.h)
#include LIB(STDfunctions.h)
#include LIB(BTNlib.h)

#include LIB(UnitBase.h)
#include LIB(Team.h)
#include LIB(UnitSpriteData.h)

//#include "all_gfx.h"

bool FAT_ENABLED;

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
	
	while(true){
		PA_WaitForVBL();
	}
}
