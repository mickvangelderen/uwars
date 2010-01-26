#include <PA9.h>
#include "DSspecs.h"
#include "STDfunctions.h"

bool SPRITE_IDS[2][MAX_SPRITES];
bool SPRITE_PALS[2][MAX_PALLETS];

//--Screen brightness--
s8 FADE_TYPE = -1;
u32 FADE_SPEED = fixed(1);
void SetFadeType(s8 type){
	if(type>0) FADE_TYPE = 1;
	else FADE_TYPE = -1;
}
void SetFadeSpeed(u32 speed){
	FADE_SPEED = speed;
}
void FadeIn(){
	for(s32 i = fixed(FADE_TYPE<<5); //from 32/-32
		unfix(FADE_TYPE*i) >= 0; //32, 31, ..., 1 >= 0
		i -= FADE_TYPE*FADE_SPEED){ //32-=speed or -32+=speed
		PA_SetBrightness(DS_BOTTOM, unfix(i)); 
		PA_SetBrightness(DS_TOP, unfix(i)); 
		PA_WaitForVBL();
	}
}
void FadeOut(){
	for(s32 i=0; unfix(FADE_TYPE*i) < 32; i += FADE_TYPE*FADE_SPEED){
		PA_SetBrightness(DS_BOTTOM, unfix(i)); 
		PA_SetBrightness(DS_TOP, unfix(i)); 
		PA_WaitForVBL();
	}
}
void ScreenOut(){
	PA_SetBrightness(1, -32);
	PA_SetBrightness(0, -32);
}
void ScreenOut(u8 screen){
	PA_SetBrightness(screen, -32);
}
void ScreenIn(){
	PA_SetBrightness(1, 0);
	PA_SetBrightness(0, 0);
}
void ScreenIn(u8 screen){
	PA_SetBrightness(screen, 0);
}

//--Sprite/palette id's--
u8 GetSpriteId(bool screen){
	u16 i;
	for(i=0; i<MAX_SPRITES; i++){
		if(!SPRITE_IDS[screen][i]){
			SPRITE_IDS[screen][i] = true;
			return i;
		}
	}
	if(i == MAX_SPRITES) PA_OutputText(screen, 1, 1, "Sprite limit on screen %d", screen);
	return 0;
}

bool FreeSpriteId(bool screen, u16 id){
	if(SPRITE_IDS[screen][id]){
		SPRITE_IDS[screen][id] = false;
		return true;
	}
	else return false;
}

u8 GetPalId(bool screen){
	u16 i;
	for(i=0; i<MAX_PALLETS; i++){
		if(!SPRITE_PALS[screen][i]){
			SPRITE_PALS[screen][i] = true;
			return i;
		}
	}
	if(i == MAX_PALLETS) PA_OutputText(screen, 1, 1, "Pallete limit on screen %d", screen);
	return 0;
}

bool FreePalId(bool screen, u16 id){
	if(SPRITE_PALS[screen][id]){
		SPRITE_PALS[screen][id] = false;
		return true;
	}
	else return false;
}

//--Creating sprites / palettes--
u8 CreateSpriteSimple(u8 screen, void* obj_data, u8 obj_shape, u8 obj_size, u8 palette){
	u8 id = GetSpriteId(screen);
	PA_CreateSprite(screen, id, obj_data, obj_shape, obj_size, COLOR_MODE_256, palette, 256, 0);
	return id;
}
u8 CreateSprite(u8 screen,  void* obj_data, u8 obj_shape, u8 obj_size, u8 color_mode, u8 palette, s16 x, s16 y){
	u8 id = GetSpriteId(screen);
	PA_CreateSprite(screen, id, obj_data, obj_shape, obj_size, color_mode, palette, x, y);
	return id;
}
u8 CreateSpriteEx(u8 screen, void* obj_data, u8 obj_shape, u8 obj_size, u8 color_mode, u8 palette, u8 obj_mode, u8 mosaic, u8 hflip, u8 vflip, u8 prio, u8 dblsize, s16 x, s16 y){
	u8 id = GetSpriteId(screen);
	PA_CreateSpriteEx(screen, id, obj_data, obj_shape, obj_size, color_mode, palette, obj_mode, mosaic, hflip, vflip, prio, dblsize, x, y);
	return id;
}
void RemoveSprite(u8 screen, u8 obj_id){
	FreeSpriteId(screen, obj_id);
	PA_DeleteSprite(screen, obj_id);
}
u8 LoadSpritePal(u8 screen, void* palette){
	u8 id = GetPalId(screen);
	PA_LoadSpritePal(screen, id, palette);
	return id;
}
void RemovePal(u8 screen, u8 obj_id){
	FreePalId(screen, obj_id);
}

//--misc--
void ErrorMsg(u8 screen, const char * const text){
	static u8 y[2] = {0, 0};

	ScreenIn(screen);
	u8 textcolor = PAtext_pal[screen];
	PAtext_pal[screen] = 1;
	PA_OutputText(screen, 0, y[screen]++, "Error: %s", text); y[screen]%=24;
	PAtext_pal[screen] = textcolor;

	PA_WaitForVBL();
	while(!(Stylus.Newpress||Pad.Newpress.Anykey)) PA_WaitForVBL();
}

void GetName(char * const n, const u16 maxNameLen){
	PA_InitKeyboard(2);
	PA_KeyboardIn(20,95);
	PA_OutputSimpleText(DS_BOTTOM, 1, 4, "Tell me, ");
	PA_OutputSimpleText(DS_BOTTOM, 1, 5, "how you would like be called: "); 
	
	//Set name to default
	u16 i;
	s16 nLetter = 0; //cursor position
	for(i=0; i<maxNameLen; i++){
		if(PA_UserInfo.Name[i] != '\0'){
			n[i] = PA_UserInfo.Name[i];
			nLetter++;
		}
		else n[i] = '\0';
	}
	
	char chInput = 0; //input character
	bool bDone = 0;
	bool cursorOn = false;
	u8 t=0;
	bool firstInput = true;
	while (!bDone)
	{
		chInput = PA_CheckKeyboard();
		if ((chInput > 31) && (nLetter <(maxNameLen - 2))) // there is a new letter and there is space enough to input more
		{
			n[nLetter] = chInput;
			nLetter++;
			if(firstInput){
				n[0] = chInput;
				for(i = 1; i < (maxNameLen); i++) n[i] = '\0';
				nLetter = 1;
				firstInput = false;
			}
		}
		else if ((chInput == PA_BACKSPACE) && nLetter) //backspace pressed
		{
			nLetter--;
			n[nLetter] = '\0'; // Erase the last letter
			if(firstInput){
				for(i = 0; i < (maxNameLen); i++) n[i] = '\0';
				nLetter = 0;
				firstInput = false;
			}
		}
		else if (((chInput == '\n') || (Pad.Newpress.Start)) && nLetter>0)//enter pressed
		{
			bDone = 1;
		}
		PA_OutputSimpleText(DS_BOTTOM, 0, 7, "                                ");
		if(cursorOn) PA_OutputText(DS_BOTTOM, 1, 7, "%s_", n); // Write the text
		else PA_OutputText(DS_BOTTOM, 1, 7, "%s", n); // Write the text
		t = (t+1)%30;
		if(t==0) cursorOn = !cursorOn;
		PA_WaitForVBL();
	}
	PA_KeyboardOut();

	u8 length=0;
	while(n[length]!='\0') length++;
	PA_OutputSimpleText(0, length+1, 7, " it is. ");
	PA_WaitForAnything();
	ClearText();
}

void ClearText(){
	PA_ClearTextBg(DS_BOTTOM);
	PA_ClearTextBg(DS_TOP);
}
inline void ClearText(u8 screen){
	PA_ClearTextBg(screen);
}

s32 abs(s32 n){
	if(n<0) return -n;
	else return n;
}

bool IsRectInScreen(s32 x, s32 y, s32 width, s32 height){
	if(	x+width >= 0 &&
		x < DS_WIDTH &&
		y+height >= 0 &&
		y < DS_HEIGHT) return true;
	return false;
}
