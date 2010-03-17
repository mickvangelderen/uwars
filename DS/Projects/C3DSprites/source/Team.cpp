#include <ndstypes.h>
#include "Team.h"

TEAM Id2Team(u8 id){
	if(id>2)id=2;
	return static_cast<TEAM>(id+1);
}

u8 Team2Id(TEAM t){
	u8 id = static_cast<u8>(t);
	if(id<=0)id=1;
	return (id-1);
}