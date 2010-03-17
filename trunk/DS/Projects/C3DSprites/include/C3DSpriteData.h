#pragma once
#include <PA9.h>
#include "IdGenerator.h"


class C3DSpriteData{
private:
	void * m_spriteData, * m_paletteData;
	u16 m_width, m_height, m_paletteId;
	u8 m_textureType;

public:
	C3DSpriteData(void * sprite, void * palette, u16 width, u16 height, u8 textureType)
		:m_spriteData(sprite), m_paletteData(palette), m_width(width), m_height(height), m_paletteId(MAX_3DPALETTES), m_textureType(textureType)
	{
		//m_textureId = PA_3DCreateTex(m_spriteData, m_width, m_height, m_textureType);
		if(m_textureType != TEX_16BITS){
			m_paletteId = Reserve3DPaletteId();
			if(m_paletteId != MAX_3DPALETTES){
				PA_Load3DSpritePal(m_paletteId, m_spriteData);
			}
		}
	}
	~C3DSpriteData(){
		Reset();
	}

	void Set(void * sprite, void * palette, u16 width, u16 height, u8 textureType){
		m_spriteData = sprite;
		m_paletteData = palette;
		m_width = width;
		m_height = height;
		m_paletteId = MAX_3DPALETTES;
		m_textureType = textureType;

		if(m_textureType != TEX_16BITS){
			m_paletteId = Reserve3DPaletteId();
			if(m_paletteId != MAX_3DPALETTES){
				PA_Load3DSpritePal(m_paletteId, m_spriteData);
			}
		}
	}

	void Reset(){
		if(m_textureType != TEX_16BITS)
			Release3DPaletteId(m_paletteId);
	}

	u16 Create3DSprite(s32 x, s32 y){
		u16 spriteId = Reserve3DSpriteId();

		if(spriteId != MAX_3DSPRITES){
			PA_3DCreateSprite(spriteId, m_spriteData, m_width, m_height, m_textureType, m_paletteId, (x>>8), (y>>8));
		}
		return spriteId;
	}
	static void Destroy3DSprite(u16 id){
		PA_3DDeleteSprite(id);
		Release3DSpriteId(id);
	}
};

