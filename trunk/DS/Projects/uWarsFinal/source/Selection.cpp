#include <PA9.h>
#include "Selection.h"
#include "Cell.h"
#include "Blob.h"
#include "DSspecs.h"
#include "STDfunctions.h"
#include "all_gfx.h"
#include "C3DSpriteData.h"
#include "Scroll.h"

#include "Sound.h"

C3DSpriteData g_selectGfx = C3DSpriteData((void*)Cell_Selector_Texture, (void*)Cell_Selector_Pal, 64, 64, TEX_256COL); 

CellSelect::CellSelect()
	:m_target(NULL), m_id(MAX_3DSPRITES){
}

CellSelect::CellSelect(Cell * const target)
	:m_target(target), m_id(MAX_3DSPRITES){
}
CellSelect::~CellSelect(){
	Destroy();
}

void CellSelect::Create(Cell * const target){ //not... very usefull
	m_target = target; m_id = MAX_3DSPRITES;
}

void CellSelect::Destroy(){
}

void CellSelect::Load(){
	m_id = Create3DSprite(&g_selectGfx, DS_WIDTH+64, 0);
	PA_3DSetSpritePrio(m_id, PRIORITY_SELECTOR);
	SetTarget(m_target);
}

void CellSelect::Unload(){
	Destroy3DSprite(m_id); m_id = MAX_3DSPRITES;
}

void CellSelect::SetTarget(Cell * const target){
	m_target = target; 
	if(target != NULL){
		PA_3DSetSpriteXY(m_id, unfix(target->X())-scrollx, unfix(target->Y())-scrolly);
		u32 radius = RADIUS_MIN + ((RADIUS_DELTA*(m_target->Capacity()>>8))>>8); //fancy scaling calculation
		PA_3DSetSpriteWidthHeight(m_id, (radius<<1), (radius<<1));
	}
	else{
		Hide();
	}
}

Cell * const CellSelect::Target() const{
	return m_target;
}

bool CellSelect::Valid(TEAM team) const{
	return m_target != NULL && m_target->Team() == team;
}

void CellSelect::Hide() const{
	PA_3DSetSpriteXY(m_id, DS_WIDTH+64, 0);
}














/*CellSelection*/

CellSelection::CellSelection()
	:m_selectionTeam(TEAM_NONE){
}

CellSelection::~CellSelection(){
}

//void Create(); //init members
//void Destroy(); //uninit members

void CellSelection::Load(){
	g_selectGfx.Load();
	for(u8 i=0; i<SELECTION_MAX_CELLS; i++){
		m_selects[i].Load();
	}
}

void CellSelection::Unload(){
	for(u8 i=0; i<SELECTION_MAX_CELLS; i++){
		m_selects[i].Unload();
	}
	g_selectGfx.Unload();
}

void CellSelection::Select(Cell * const cell){
	if(!IsSelected(cell)){
		for(u8 i=0; i<SELECTION_MAX_CELLS; i++){
			if(m_selects[i].Target()==NULL){
				m_selects[i].SetTarget(cell);
				if(m_selectionTeam == TEAM_NONE){
					m_selectionTeam = cell->Team();
				}
				break;
			}
		}
	}

}

void CellSelection::Deselect(Cell * const cell){
	u8 count=0;
	for(u8 i=0; i<SELECTION_MAX_CELLS; i++){
		if(m_selects[i].Target() == cell){
			m_selects[i].SetTarget(NULL);
		}
		else if(m_selects[i].Target() != NULL) count++;
	}
	if(count==0) m_selectionTeam = TEAM_NONE;
}

void CellSelection::Clear(){
	for(u8 i=0; i<SELECTION_MAX_CELLS; i++){
		m_selects[i].SetTarget(NULL);
	}
	m_selectionTeam = TEAM_NONE;
}

bool CellSelection::IsSelected(Cell * const cell)const{
	for(u8 i=0; i<SELECTION_MAX_CELLS; i++){
		if(m_selects[i].Target() == cell) return true;
	}
	return false;
}

TEAM CellSelection::SelectionTeam()const{
	return m_selectionTeam;
}

void CellSelection::MassAttack(Cell * const cell, Blob * const blobs)const{
	mmEffectRelease(mmEffect(SFX_VROEMM));
	for(u8 i=0; i<SELECTION_MAX_CELLS; i++){
		if(m_selects[i].Target() != NULL && m_selects[i].Target() != cell){
			m_selects[i].Target()->AttackCell(cell, blobs);
		}
	}
}

void CellSelection::Update(Cell * const cells, Blob * const blobs){
	if(Stylus.Newpress){
		Cell * touchedCell = NULL;
		for(u8 i=0; i<MAX_CELLS; i++){
			if(cells[i].IsTouched()){
				touchedCell = &cells[i]; //Loop trough cells and get the one that was touched
				break;
			}
		}
		if(touchedCell!=NULL){
			if(Pad.Held.L){ //multi select mode
				if(IsSelected(touchedCell)) Deselect(touchedCell);
				else{
					switch(touchedCell->Team()){
					case TEAM_NONE:
						break;
					case TEAM_NEUTRAL: //neutral cell touched with L held
						Clear();
						Select(touchedCell);
						break;
					case TEAM_RED: //red cell touched with L held
						switch(m_selectionTeam){
						case TEAM_NONE:
							Clear();
							Select(touchedCell);
							break;
						case TEAM_NEUTRAL:
							Clear();
							Select(touchedCell);
							break;
						case TEAM_RED:
							Select(touchedCell);
							break;
						case TEAM_BLUE:
							Clear();
							Select(touchedCell);
							break;
						}
						break;
					case TEAM_BLUE://blue cell touched with L held
						Clear();
						Select(touchedCell);
						break;
					}
				}
			}
			else{ //L not held
				switch(m_selectionTeam){//current selection team
				case TEAM_NONE:
					Select(touchedCell);
					break;
				case TEAM_NEUTRAL://neutral cells selected, touched neutral without multiselect mode
					Clear();
					Select(touchedCell);
					break;
				case TEAM_RED://red cells selected, touched other cell
					MassAttack(touchedCell, blobs);
					break;
				case TEAM_BLUE://player has blue cells selected, touched neutral without multiselect mode
					Clear();
					Select(touchedCell);
					break;
				}
			}
		}//a cell was touched
		else{//YOU MISSED
			Clear();
		}
	}//stylus.newpress
	for(u8 i=0; i<SELECTION_MAX_CELLS; i++) m_selects[i].SetTarget(m_selects[i].Target());
}