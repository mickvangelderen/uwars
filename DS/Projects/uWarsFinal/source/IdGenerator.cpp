#include <PA9.h>
#include "IdGenerator.h"

#define DEBUG //outcomment this line if you don't want error messages

//===GLOBALS===
//---3D sprites---
u16 g_n3DSprites = 0;
u8 g_3DSpriteIds[(MAX_3DSPRITES>>3)] = {0};
//---3D palettes
u16 g_n3DPalettes = 0;
u8 g_3DPaletteIds[(MAX_3DPALETTES>>3)] = {0};
//---sprites---
u16 g_nSprites[2] = {0};
u8 g_SpriteIds[2][(MAX_SPRITES>>3)] = {{0},{0}};
//---palettes---
u16 g_nPalettes[2] = {0};
u8 g_PaletteIds[2][(MAX_PALETTES>>3)] = {{0},{0}};
//---iterator---
u16 g_3DSpriteIter = 0;
u16 g_3DPaletteIter = 0;
u16 g_SpriteIter[2] = {0};
u16 g_PaletteIter[2] = {0};
//=============


#ifdef DEBUG
inline void ErrorMessage(const char * const txt){
	PA_OutputText(1, 0, 0, "IdGenerator.h error - %s", txt);
	PA_WaitForVBL();
	PA_WaitFor(Stylus.Newpress||Pad.Newpress.Anykey);
}
#endif

//===Bit operations===
inline bool GetBit(const u8 & n, u8 pos){
	return (n>>pos & 1);
}
inline void SetBit(u8 & n, u8 pos){
	n = (n | 1<<pos);
}
inline void ResetBit(u8 & n, u8 pos){
	n = (n & ~(1<<pos));
}
//==================

//===Counter variable get's===
u16 GetNumberOf3DSprites(){
	return g_n3DSprites;
}
u16 GetNumberOf3DPalettes(){
	return g_n3DPalettes;
}
u16 GetNumberOfSprites(u8 screen){
	return g_nSprites[screen];
}
u16 GetNumberOfPalettes(u8 screen){
	return g_nPalettes[screen];
}
//==========================

//===3D ids===
u16 Reserve3DSpriteId(){
	u8 bitPos;
	u16 bytePos;
	u16 start = g_3DSpriteIter;
	do{
		bitPos = (g_3DSpriteIter&7);
		bytePos = ((g_3DSpriteIter-bitPos)>>3); 
		if(!GetBit(g_3DSpriteIds[bytePos], bitPos)){
			//it's free
			SetBit(g_3DSpriteIds[bytePos], bitPos);
			g_n3DSprites++;
			return g_3DSpriteIter;
		}
		g_3DSpriteIter = (g_3DSpriteIter+1)%MAX_3DSPRITES;
	}while(g_3DSpriteIter != start);

	#ifdef DEBUG
		ErrorMessage("no more free 3dsprite id's");
	#endif
	return MAX_3DSPRITES; //no free space found
}

void Release3DSpriteId(u16 id){
	u8 bitPos = (id&7);
	u16 bytePos = ((id-bitPos)>>3);
	
	if(GetBit(g_3DSpriteIds[bytePos], bitPos)){
		ResetBit(g_3DSpriteIds[bytePos], bitPos);
		g_n3DSprites--;
	}
	#ifdef DEBUG
	else{
			ErrorMessage("attempting to release an unused 3dsprite id");
	}
	#endif
}

u16 Reserve3DPaletteId(){
	u8 bitPos;
	u16 bytePos;
	u16 start = g_3DPaletteIter;
	do{
		bitPos = (g_3DPaletteIter&7);
		bytePos = ((g_3DPaletteIter-bitPos)>>3); 
		if(!GetBit(g_3DPaletteIds[bytePos], bitPos)){
			//it's free
			SetBit(g_3DPaletteIds[bytePos], bitPos);
			g_n3DPalettes++;
			return g_3DPaletteIter;
		}
		g_3DPaletteIter = (g_3DPaletteIter+1)%MAX_3DPALETTES;
	}while(g_3DPaletteIter != start);

	#ifdef DEBUG
		ErrorMessage("no more free 3dpalette id's");
	#endif
	return MAX_3DPALETTES; //no free space found
}

void Release3DPaletteId(u16 id){
	u8 bitPos = (id&7);
	u16 bytePos = ((id-bitPos)>>3);
	if(GetBit(g_3DPaletteIds[bytePos], bitPos)){
		ResetBit(g_3DPaletteIds[bytePos], bitPos);
		g_n3DPalettes--;
	}
	#ifdef DEBUG
	else{
			ErrorMessage("attempting to release an unused 3dpalette id");
	}
	#endif
}


//===sprite and palette id's===
u16 ReserveSpriteId(u8 screen){
	u8 bitPos;
	u16 bytePos;
	#ifdef DEBUG
		if(screen < 0 || screen > 1) ErrorMessage("bad screen parameter");
	#endif
	u16 start = g_SpriteIter[screen];
	do{
		bitPos = (g_SpriteIter[screen]&7);
		bytePos = ((g_SpriteIter[screen]-bitPos)>>3); 
		if(!GetBit(g_SpriteIds[screen][bytePos], bitPos)){
			//it's free
			SetBit(g_SpriteIds[screen][bytePos], bitPos);
			g_nSprites[screen]++;
			return g_SpriteIter[screen];
		}
		g_SpriteIter[screen] = (g_SpriteIter[screen]+1)%MAX_SPRITES;
	}while(g_SpriteIter[screen] != start);

	#ifdef DEBUG
		ErrorMessage("no more free sprite id's");
	#endif
	return MAX_SPRITES; //no free space found
}

void ReleaseSpriteId(u8 screen, u16 id){
	u8 bitPos = (id&7);
	u16 bytePos = ((id-bitPos)>>3);
	#ifdef DEBUG
		if(screen < 0 || screen > 1) ErrorMessage("bad screen parameter");
	#endif
	if(GetBit(g_SpriteIds[screen][bytePos], bitPos)){
		ResetBit(g_SpriteIds[screen][bytePos], bitPos);
		g_nSprites[screen]--;
	}
	#ifdef DEBUG
	else{
			ErrorMessage("attempting to release an unused sprite id");
	}
	#endif
}

u16 ReservePaletteId(u8 screen){
	u8 bitPos;
	u16 bytePos;
	#ifdef DEBUG
		if(screen < 0 || screen > 1) ErrorMessage("bad screen parameter");
	#endif
	u16 start = g_PaletteIter[screen];
	do{
		bitPos = (g_PaletteIter[screen]&7);
		bytePos = ((g_PaletteIter[screen]-bitPos)>>3); 
		if(!GetBit(g_PaletteIds[screen][bytePos], bitPos)){
			//it's free
			SetBit(g_PaletteIds[screen][bytePos], bitPos);
			g_nPalettes[screen]++;
			return g_PaletteIter[screen];
		}
		g_PaletteIter[screen] = (g_PaletteIter[screen]+1)%MAX_PALETTES;
	}while(g_PaletteIter[screen] != start);
	#ifdef DEBUG
		ErrorMessage("no more free sprite id's");
	#endif
	return MAX_PALETTES; //no free space found
}

void ReleasePaletteId(u8 screen, u16 id){
	u8 bitPos = (id&7);
	u16 bytePos = ((id-bitPos)>>3);
	#ifdef DEBUG
		if(screen < 0 || screen > 1) ErrorMessage("bad screen parameter");
	#endif
	if(GetBit(g_PaletteIds[screen][bytePos], bitPos)){
		ResetBit(g_PaletteIds[screen][bytePos], bitPos);
		g_nPalettes[screen]--;
	}
	#ifdef DEBUG
	else{
			ErrorMessage("attempting to release an unused sprite id");
	}
	#endif
}