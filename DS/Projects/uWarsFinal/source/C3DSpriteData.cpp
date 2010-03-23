#include <PA9.h>
#include "C3DSpriteData.h"
#include "IdGenerator.h"

C3DSpriteData::C3DSpriteData()
	:m_spriteData(NULL), m_paletteData(NULL), 
	m_width(0), m_height(0), m_paletteId(MAX_3DPALETTES), m_textureId(MAX_3DTEXTURES),  
	m_textureType(0){
}

C3DSpriteData::C3DSpriteData(void * sprite, void * palette, u16 width, u16 height, u8 textureType)
	:m_spriteData(sprite), m_paletteData(palette), 
	m_width(width), m_height(height), m_paletteId(MAX_3DPALETTES), m_textureId(MAX_3DTEXTURES), 
	m_textureType(textureType){
}

C3DSpriteData::~C3DSpriteData(){
	Destroy();
}

void C3DSpriteData::Create(void * sprite, void * palette, u16 width, u16 height, u8 textureType){
	m_spriteData = sprite;
	m_paletteData = palette;
	m_width = width;
	m_height = height;
	m_paletteId = MAX_3DPALETTES;
	m_textureId = MAX_3DTEXTURES;
	m_textureType = textureType;
}

void C3DSpriteData::Destroy(){
}

void C3DSpriteData::Load(){
	if(m_textureId == MAX_3DTEXTURES){
		m_textureId = PA_3DCreateTex(m_spriteData, m_width, m_height, m_textureType);
		//PA_OutputText(DS_TOP, 0, 4, "created tex, id: %d  ", m_textureId);
	}
	if(m_textureType != TEX_16BITS && m_paletteId == MAX_3DPALETTES){
		m_paletteId = Reserve3DPaletteId();
		if(m_paletteId != MAX_3DPALETTES){ //successfully got an palette id
			PA_Load3DSpritePal(m_paletteId, m_paletteData);
			//PA_OutputText(DS_TOP, 0, 5, "loaded palette, id: %d  ", m_paletteId);
		}
	}
}

void C3DSpriteData::Unload(){
	PA_3DDeleteTex(m_textureId);
	m_textureId = MAX_3DTEXTURES;
	if(m_textureType != TEX_16BITS){
		Release3DPaletteId(m_paletteId);
		m_paletteId = MAX_3DPALETTES;
	}
}

void C3DSpriteData::ChangeSpriteGraphics(u16 spriteId)const{
	PA_3DSetSpriteTex(spriteId, m_textureId);
	if(m_textureType != TEX_16BITS){
		PA_3DSetSpritePal(spriteId, m_paletteId);
	}
}

const void * C3DSpriteData::SpriteData()const{
	return m_spriteData;
}
const void * C3DSpriteData::PaletteData()const{
	return m_paletteData;
}
u16 C3DSpriteData::Width()const{
	return m_width;
}
u16 C3DSpriteData::Height()const{
	return m_height;
}
u16 C3DSpriteData::PaletteId()const{
	return m_paletteId;
}
u16 C3DSpriteData::TextureId()const{
	return m_textureId;
}
u8 C3DSpriteData::TextureType()const{
	return m_textureType;
}

u16 Create3DSprite(const C3DSpriteData * const sd, s16 x, s16 y){
	u16 spriteId = Reserve3DSpriteId();
	if(spriteId != MAX_3DSPRITES){
		PA_3DCreateSpriteFromTex(spriteId, sd->m_textureId, sd->m_width, sd->m_height, sd->m_paletteId, x, y);
	}
	//PA_OutputText(DS_TOP, 0, 6, "created sprite, id: %d  ", spriteId);
	return spriteId;
}

void Destroy3DSprite(u16 id){
	PA_3DDeleteSprite(id);
	Release3DSpriteId(id);
}