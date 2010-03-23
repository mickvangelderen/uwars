#include <PA9.h>
#include "DSspecs.h"
#include "Scroll.h"

s32 scrollx = 0, scrolly = 0, mapwidth = 256, mapheight = 192;
u8 scrollspeed = 0;
u8 doublePressSpeed = 20; //withing 4 frames for double press;
struct pad{
	u8 timer;
	bool doublepress;
}Left, Right, Up, Down;

void InitScroll(s32 mapWidth, s32 mapHeight, u8 scrollSpeed){
	mapwidth = mapWidth;
	mapheight = mapHeight;
	scrollspeed = scrollSpeed;
}

void UpdateDoublepress(){
	if(Left.timer>0) Left.timer++;
	if(Right.timer>0) Right.timer++;
	if(Down.timer>0) Down.timer++;
	if(Up.timer>0) Up.timer++;

	if(Left.timer>doublePressSpeed) Left.timer = 0;
	if(Right.timer>doublePressSpeed) Right.timer = 0;
	if(Down.timer>doublePressSpeed) Down.timer = 0;
	if(Up.timer>doublePressSpeed) Up.timer = 0;

	if(Pad.Released.Left ) Left.doublepress = false;
	if(Pad.Released.Right) Right.doublepress = false;
	if(Pad.Released.Down ) Down.doublepress = false;
	if(Pad.Released.Up   ) Up.doublepress = false;

	if(Pad.Newpress.Left){
		if(Left.timer > 0)Left.doublepress = true;
		else Left.timer = 1;
	}
	if(Pad.Newpress.Right){
		if(Right.timer > 0)Right.doublepress = true;
		else Right.timer = 1;
	}
	if(Pad.Newpress.Down){
		if(Down.timer > 0)Down.doublepress = true;
		else Down.timer = 1;
	}
	if(Pad.Newpress.Up){
		if(Up.timer > 0)Up.doublepress = true;
		else Up.timer = 1;
	}

	/*
	PA_OutputText(DS_TOP, 0, 4, "Left.timer:%d  Left.d: %d ", Left.timer, Left.doublepress);
	PA_OutputText(DS_TOP, 0, 5, "Right.timer:%d Right.d:%d ", Right.timer, Right.doublepress);
	PA_OutputText(DS_TOP, 0, 6, "Down.timer:%d  Down.d: %d ", Down.timer, Down.doublepress);
	PA_OutputText(DS_TOP, 0, 7, "Up.timer:%d    Up.d:   %d ", Up.timer, Up.doublepress);
	*/
}


void UpdateScroll(){
	UpdateDoublepress();
	scrollx += scrollspeed*(Pad.Held.Right - Pad.Held.Left)*((Right.doublepress||Left.doublepress)+1);
	scrolly += scrollspeed*(Pad.Held.Down - Pad.Held.Up)*((Down.doublepress||Up.doublepress)+1);
	if(scrollx > mapwidth - DS_WIDTH) scrollx = mapwidth - DS_WIDTH;
	if(scrollx < 0) scrollx = 0;
	if(scrolly > mapheight - DS_HEIGHT) scrolly = mapheight - DS_HEIGHT;
	if(scrolly < 0) scrolly = 0;

	/*
	PA_OutputText(DS_TOP, 0, 0, "scroll x: %d   ", scrollx);
	PA_OutputText(DS_TOP, 0, 1, "scroll y: %d   ", scrolly);
	*/
}