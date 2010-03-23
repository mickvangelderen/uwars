#include <PA9.h>
#include "InitFat.h"
#include <fat.h>

bool FAT_INIT_SUCCESS = false;

bool InitFat(){
	PA_WaitForVBL();PA_WaitForVBL();PA_WaitForVBL();
	FAT_INIT_SUCCESS = fatInitDefault();
	PA_WaitForVBL();
	return FAT_INIT_SUCCESS;
}
