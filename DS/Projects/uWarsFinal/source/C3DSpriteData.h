#pragma once
#include <PA9.h>
#include "DSspecs.h"
#include "IdGenerator.h"

class C3DSpriteData{
private:
	void * m_spriteData, * m_paletteData;
	u16 m_width, m_height, m_paletteId, m_textureId;
	u8 m_textureType;

public:
	C3DSpriteData();
	C3DSpriteData(void * sprite, void * palette, u16 width, u16 height, u8 textureType);
	~C3DSpriteData();
	void Create(void * sprite, void * palette, u16 width, u16 height, u8 textureType);
	void Destroy();
	
	void Load();
	void Unload();
	
	void ChangeSpriteGraphics(u16 spriteId)const;

	const void * SpriteData()const;
	const void * PaletteData()const;
	u16 Width()const;
	u16 Height()const;
	u16 PaletteId()const;
	u16 TextureId()const;
	u8 TextureType()const;

	friend u16 Create3DSprite(const C3DSpriteData * const sd, s16 x, s16 y);
};

u16 Create3DSprite(const C3DSpriteData * const sd, s16 x, s16 y);
void Destroy3DSprite(u16 id);