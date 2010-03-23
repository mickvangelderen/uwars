#include <PA9.h>
#include "Menu.h"
#include "DSspecs.h"
#include "all_gfx.h"

#include "InitFat.h"
#include "Maps.h"

#include "Sound.h"

void SplashScreens(){
	PA_LoadBackground(DS_TOP, BG_BACKGROUND, &uWars_Splash_Development);
	PA_Init16cBg(DS_BTM, BG_TEXT);
	PA_16cText(DS_BTM, 10, 76, 255, 191, "Touch (this) screen to continue", 1, 3, 100);
	mmLoadEffect(SFX_BFHHALLO);
	//FadeIn();
	mmEffect(SFX_BFHHALLO);
	PA_WaitForVBL();PA_WaitForVBL();PA_WaitForVBL();
	PA_WaitFor(Stylus.Newpress||Pad.Newpress.Anykey);
	PA_WaitForVBL();PA_WaitForVBL();PA_WaitForVBL();
	//FadeOut();
	mmEffectCancelAll();
	mmUnloadEffect(SFX_BFHHALLO);
	PA_DeleteBg(DS_TOP,BG_BACKGROUND);
}

int main(){
	PA_Init();
	InitFat();
	InitMaps();
	mmInitDefaultMem((mm_addr)soundbank_bin);

	SplashScreens();

	mmLoad(MOD_UWARSGAMEHYPER);
	mmStart(MOD_UWARSGAMEHYPER, MM_PLAY_LOOP);

	mmLoadEffect(SFX_KLIK);
	mmLoadEffect(SFX_BLUB);
	mmLoadEffect(SFX_VROEMM);

	mmSetModuleVolume(200);
	mmSetEffectsVolume(255);

	MainMenu();
}
