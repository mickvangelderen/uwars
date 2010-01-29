#include <PA9.h>
#include "Selection.h"
#include "Cell.h"
#include "Blob.h"

#define LIBPATH C:/SVN/DS/Libraries
#define LIB(x) <LIBPATH/x>
#include LIB(DSspecs.h)
#include LIB(STDfunctions.h)

#include "all_gfx.h"

extern Cell * cells;

CellSelect::CellSelect()
: m_target(NULL){
	m_spriteId = CreateSprite(DS_BTM, (void*)CellSelector_Sprite, OBJ_SIZE_32X32, COLOR_MODE_256, m_paletteId, DS_WIDTH, 0);
	PA_SetSpriteExtPrio(DS_BTM, m_spriteId, 0);
}
CellSelect::~CellSelect(){
	DestroySprite(DS_BTM, m_spriteId);
}
void CellSelect::CreateSpriteData(void * sprite, void * palette){
	m_sprite = sprite;
	m_palette = palette;
	m_paletteId = CreateSpritePalette(DS_BTM, m_palette);
}
void CellSelect::DestroySpriteData(){
	DestroySpritePalette(DS_BTM, m_paletteId);
	m_paletteId = MAX_PALETTES;
}
Cell * CellSelect::Target() const {
	return m_target;
}
void CellSelect::SetTarget(Cell * const target){
	m_target = target;
	if(target!=NULL) PA_SetSpriteXY(DS_BTM, m_spriteId, unfix(m_target->X()), unfix(m_target->Y()));
	else Hide();
}
void CellSelect::Hide() const {
	PA_SetSpriteXY(DS_BTM, m_spriteId, DS_WIDTH, 0);
}

void* CellSelect::m_sprite = NULL;
void* CellSelect::m_palette = NULL;
u8 CellSelect::m_paletteId = 0;

/*CellSelection*/

const u8 MAX_CELLS_SELECTED = 5;

bool CellSelection::IsSelected(Cell * const cell){
	for(u8 i=0; i<m_nSelected; i++) if(cell == m_selects[i].Target()) return true;
	return false;
}
void CellSelection::Attack(Cell * const cell){
	for(u8 i=0; i<m_nSelected; i++){
		m_selects[i].Target()->Send(cell);
	}
}

CellSelection::CellSelection()
:m_nSelected(0), 
m_selectionTeam(TEAM_NONE)
{
	CellSelect::CreateSpriteData((void*)CellSelector_Sprite, (void*)CellSelector_Pal);
	m_selects = new CellSelect[MAX_CELLS_SELECTED];
}
CellSelection::~CellSelection(){
	delete[]m_selects;
	CellSelect::DestroySpriteData();
}

void CellSelection::Reset(){
	m_selectionTeam = TEAM_NONE;
	for(u8 i=0; i<m_nSelected; i++){
		m_selects[i].SetTarget(NULL);
		m_selects[i].Hide();
	}
	m_nSelected = 0;
}

void CellSelection::Select(Cell * const cell){
	if(m_nSelected<MAX_CELLS_SELECTED && !IsSelected(cell)){
		m_selects[m_nSelected++].SetTarget(cell);
		m_selectionTeam = cell->Team();
	}
}
void CellSelection::Deselect(Cell * const cell){
	for(u8 i=0; i<m_nSelected; i++){
		if(m_selects[i].Target() == cell){
			m_selects[i].Hide();

			while(i++<(m_nSelected-1)){
				m_selects[i-1].SetTarget(m_selects[i].Target());
			}

			m_selects[--m_nSelected].SetTarget(NULL);
			break;
		}
	}
}

void CellSelection::Update(){
	if(Stylus.Newpress){
		Cell * touchedCell = NULL;
		for(u8 i=0; i<MAX_CELLS; i++) if(cells[i].IsTouched()) touchedCell = &cells[i]; //Loop trough cells and get the one that was touched
		if(touchedCell!=NULL){
			if(Pad.Held.L){ //multi select mode
				if(IsSelected(touchedCell)) Deselect(touchedCell);
				else{
					switch(touchedCell->Team()){
					case TEAM_NONE:
						break;
					case TEAM_NEUTRAL: //neutral cell touched with L held
						Reset();
						Select(touchedCell);
						break;
					case TEAM_RED: //red cell touched with L held
						switch(m_selectionTeam){
						case TEAM_NONE:
							Select(touchedCell);
							break;
						case TEAM_NEUTRAL:
							Reset();
							Select(touchedCell);
							break;
						case TEAM_RED:
							Select(touchedCell);
							break;
						case TEAM_BLUE:
							Reset();
							Select(touchedCell);
							break;
						}
						break;
					case TEAM_BLUE://blue cell touched with L held
						Reset();
						Select(touchedCell);
						break;
					}
				}
			}
			else{ //L not held
				switch(m_selectionTeam){
				case TEAM_NONE:
					Select(touchedCell);
					break;
				case TEAM_NEUTRAL://neutral cells selected, touched neutral without multiselect mode
					Reset();
					Select(touchedCell);
					break;
				case TEAM_RED://red cells selected, touched neutral without multiselect mode
					Attack(touchedCell);
					break;
				case TEAM_BLUE://blue cells selected, touched neutral without multiselect mode
					Reset();
					Select(touchedCell);
					break;
				}
			}
		}//a cell was touched
		else{//YOU MISSED
			Reset();
		}
	}//stylus.newpress
}