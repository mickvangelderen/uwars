#include <PA9.h>
#include "LevelData.h"

#include "LIBPATH.h"

#include LIB(Team.h)
#include LIB(DSspecs.h)
#include LIB(STDfunctions.h)

const u16 NUMBER_OF_LEVELS = 1;
LevelData levels[NUMBER_OF_LEVELS];

void InitLevels(){
/*Level 1*/
levels[0].mapName = "TestLevel";
levels[0].mapDescription = "No description";
levels[0].mapWidth = fixed(DS_WIDTH);
levels[0].mapHeight = fixed(DS_HEIGHT);

levels[0].cells[0].x = fixed(16);
levels[0].cells[0].y = fixed(16);
levels[0].cells[0].str = fixed(10);
levels[0].cells[0].capacity = fixed(60);
levels[0].cells[0].growthRate = 5;
levels[0].cells[0].team = TEAM_BLUE;

levels[0].cells[1].x = fixed(24);
levels[0].cells[1].y = fixed(96);
levels[0].cells[1].str = fixed(10);
levels[0].cells[1].capacity = fixed(60);
levels[0].cells[1].growthRate = 5;
levels[0].cells[1].team = TEAM_RED;

levels[0].cells[2].x = fixed(60);
levels[0].cells[2].y = fixed(16);
levels[0].cells[2].str = fixed(10);
levels[0].cells[2].capacity = fixed(60);
levels[0].cells[2].growthRate = 5;
levels[0].cells[2].team = TEAM_NEUTRAL;
/* End of Level 1 */
}

u16 GetNumberOfLevels(){
	return NUMBER_OF_LEVELS;
}

const LevelData * const GetLevel(u16 id){
	if(id<0 || id>= NUMBER_OF_LEVELS) return NULL;
	return &levels[id];
}
