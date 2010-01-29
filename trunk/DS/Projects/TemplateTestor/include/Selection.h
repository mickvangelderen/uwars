#pragma once

#include <PA9.h>
#include "Cell.h"

#include "LIBPATH.h"

//#include LIB(BTNlib.h)
#include LIB(Team.h)

class CellSelect{
private:
	static void* m_sprite; 
	static void* m_palette;
	static u8 m_paletteId;

	Cell * m_target;
	u8 m_spriteId;
public:
	CellSelect();
	~CellSelect();
	static void CreateSpriteData(void * sprite, void * palette);
	static void DestroySpriteData();
	Cell * Target() const;
	void SetTarget(Cell * const target);
	void Hide() const;
};

class CellSelection{
private:
	u8 m_nSelected;
	CellSelect * m_selects;
	TEAM m_selectionTeam;

	bool IsSelected(Cell * const cell);
	void Attack(Cell * const cell);
public:
	CellSelection();
	~CellSelection();

	void Reset();
	void Select(Cell * const cell);
	void Deselect(Cell * const cell);

	void Update();
};