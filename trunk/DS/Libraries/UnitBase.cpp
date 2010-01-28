#include <PA9.h>
#include "UnitBase.h"
#include "Team.h"

UnitBase::UnitBase():m_x(0), m_y(0), m_radius(0), m_team(TEAM_NONE){}
UnitBase::UnitBase(s32 x, s32 y, u32 radius, TEAM team): m_x(x), m_y(y), m_radius(radius), m_team(team){}
UnitBase::~UnitBase(){}

void UnitBase::SetX(s32 x){m_x = x;}
void UnitBase::SetY(s32 y){m_y = y;}
void UnitBase::SetXY(s32 x, s32 y){m_x = x; m_y = y;}

void UnitBase::SetXCenter(s32 x){m_x = x-XOffset();}
void UnitBase::SetYCenter(s32 y){m_y = y-YOffset();}
void UnitBase::SetXYCenter(s32 x, s32 y){m_x = x-XOffset(); m_y = y-YOffset();}

void UnitBase::SetRadius(u32 radius){m_radius = radius;}
void UnitBase::SetTeam(TEAM team){m_team = team;}

s32 UnitBase::X()const {return m_x;}
s32 UnitBase::Y()const {return m_y;}

s32 UnitBase::XCenter()const {return m_x+XOffset();}
s32 UnitBase::YCenter()const {return m_y+YOffset();}

u32 UnitBase::Radius()const{return m_radius;}
TEAM UnitBase::Team()const{return m_team;}

u32 UnitBase::GetDist(UnitBase * const a)const{
    s32 dx = this->XCenter()-((UnitBase*)a)->XCenter();
    s32 dy = this->YCenter()-((UnitBase*)a)->YCenter();
    return dx*dx+dy*dy;
}

u32 UnitBase::GetDist(s32 x, s32 y)const{
    s32 dx = this->XCenter()-x;
    s32 dy = this->YCenter()-y;
    return dx*dx+dy*dy;
}

bool UnitBase::IsCollision(UnitBase * const u)const{
	u32 range = u->Radius()+this->Radius();
	return (GetDist(u) < range*range);
}
