#include <PA9.h>
#include "Palette.h"
#include "IdGenerator.h"
#include "DSspecs.h"

Palette::Palette()
	:m_screen(0), m_data(NULL), m_id(MAX_PALETTES){
}

Palette::Palette(u8 screen, void *paletteData){
	Set(screen, paletteData);
}

Palette::~Palette(){
	Unload();
}

void Palette::Set(u8 screen, void * paletteData){
	m_id = MAX_PALETTES; //need to unload first
	m_data = paletteData;
	m_screen = screen;
}

u8 Palette::Screen()const{
	return m_screen;
}
void * Palette::PaletteData()const{
	return m_data;
}
u8 Palette::Id()const{
	return m_id;
}

bool Palette::Load(){
	if(m_id == MAX_PALETTES){
		s16 tid = ReservePaletteId(m_screen);
		if(tid >= 0 && tid < MAX_PALETTES){
			m_id = tid;
			PA_LoadSpritePal(m_screen, m_id, m_data);
			return true;
		}
	}
	return false;
}

bool Palette::Unload(){
	if(m_id >= 0 && m_id < MAX_PALETTES){
		ReleasePaletteId(m_screen, m_id);
		m_id = MAX_PALETTES; //make it loadable again
		return true;
	}
	return false;
}


/*
class Button{
private:
	u16 x, y;
	void *sprite[]; u8 spriteNum;
	u8 spriteId, paletteId;
*/