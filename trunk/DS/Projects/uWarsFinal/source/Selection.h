#pragma once
#include <PA9.h>
#include "Cell.h"
#include "Team.h"
#include "C3DSpriteData.h"

#define SELECTION_MAX_CELLS	15
#define PRIORITY_SELECTOR 1080
//extern C3DSpriteData g_selectGfx;

class CellSelect{
private:
	Cell * m_target;
	u16 m_id;
public:
	CellSelect();
	CellSelect(Cell * const target);
	~CellSelect();

	void Create(Cell * const target);
	void Destroy();

	void Load();
	void Unload();

	void SetTarget(Cell * const target);
	
	Cell * const Target() const;
	bool Valid(TEAM team) const;

	void Hide() const;
};

class CellSelection{
private:
	CellSelect m_selects[SELECTION_MAX_CELLS];
	TEAM m_selectionTeam;
public:
	CellSelection(); //init members
	~CellSelection(); //uninit members

	//void Create(); //init members
	//void Destroy(); //uninit members

	void Load(); //load sprites etc. 
	void Unload(); //unload sprites etc.

	void Select(Cell * const cell);
	void Deselect(Cell * const cell);
	void Clear();

	bool IsSelected(Cell * const cell)const;
	TEAM SelectionTeam() const;

	void MassAttack(Cell * const cell, Blob * const blobs) const;

	void Update(Cell * const cells, Blob * const blobs);
};