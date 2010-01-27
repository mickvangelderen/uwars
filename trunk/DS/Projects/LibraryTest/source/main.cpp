////////////////////////////
// PAlib project template //
////////////////////////////

#include <PA9.h>
//#include "efs_lib.h"
//#include <fat.h>

//Hahahah xD jelle check dit: 
#define LIBPATH C:/SVN/DS/Libraries //../../../Libraries
#define LIB(x) <LIBPATH/x>
#include LIB(DSspecs.h)
#include LIB(STDfunctions.h)
#include LIB(BTNlib.h)

#include "all_gfx.h" //gfx

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
	
	buttonGfx btnGfx(DS_BOTTOM, (void*)TextButtonBlue_L_Sprite, 
								(void*)TextButtonBlue_M_Sprite, 
								(void*)TextButtonBlue_R_Sprite, 
								(void*)TextButtonBlue_Pal);

	char * name[5];
	for(u8 i=0; i<5; i++){
		name[i] = new char[33];
		name[0] = '\0';
	}

	while(true){
		u8 action = 0;
		{
			button b[5];
			for(u8 i=0; i<5; i++) b[i].Set(2, 2+3*i, name[i], 0, &btnGfx);

			//u8 action = 0;
			
			SetFadeSpeed(256);
			FadeIn();
			while(!action){
				for(u8 i=0; i<5; i++){
					if(b[i].IsTouched() && Stylus.Newpress){
						action = i+1;
					}
				}
				PA_WaitForVBL();
			}
			SetFadeType(-1);
			
			FadeOut();
		}//code block to get rid of the button variables xD

		//for(u8 i=0; i<5; i++)b[i].Reset();
		
		if(action>0 && action<6){
			SetFadeSpeed(128);
			FadeIn();
			GetName(name[action-1], 32);
			SetFadeType(1);
			FadeOut();
		}
		else{
			ErrorMsg(DS_TOP, "Main input error");
		}
	}
}
