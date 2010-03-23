#pragma once
#include <PA9.h>

class Palette{
private:
	u8 m_screen;
	void *m_data;
	u8 m_id;

public:
	Palette();
	Palette(u8 screen, void *paletteData);
	~Palette();

	void Set(u8 screen, void *paletteData);

	u8 Screen()const;
	void * PaletteData()const;
	u8 Id()const;

	bool Load();
	bool Unload();
};