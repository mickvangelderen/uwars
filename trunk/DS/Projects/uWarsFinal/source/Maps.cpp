#include <PA9.h>
#include "Maps.h"
#include "Map.h"
#include "DSspecs.h"
#include "STDfunctions.h"

Map g_maps[MAP_NUMBER];
Map * g_map = NULL;

void InitMaps(){
	ResetMaps(g_maps, MAP_NUMBER);
	
	u8 id=0;
	//level 1 - using the stylus to attack
	SetMap(&g_maps[id], "Tutorial 1", "You own the red cells. Use the stylus to select a red cell, then click on the blue cell. ", 1, MAP_DIFFICULTY_NONE, DS_WIDTH, DS_HEIGHT, 2);
	SetMapCell(&g_maps[id], 0, fixed(64), fixed(DS_HEIGHT>>1),				fixed(30), fixed(128), 2, TEAM_RED);
	SetMapCell(&g_maps[id], 1, fixed(DS_WIDTH-64), fixed(DS_HEIGHT>>1),		fixed(10), fixed(128), 1, TEAM_BLUE);
	id++;

	//level 2 - multiselect
	SetMap(&g_maps[id], "Tutorial 2", "Hold the L button to select multiple cells, then attack the enemy with all your cells. ", 2, MAP_DIFFICULTY_NONE, DS_WIDTH, DS_HEIGHT, 4);
	SetMapCell(&g_maps[id], 0, fixed(64), fixed((DS_HEIGHT>>1)-40),				fixed(20), fixed(128), 2, TEAM_RED);
	SetMapCell(&g_maps[id], 1, fixed(DS_WIDTH-64), fixed((DS_HEIGHT>>1)-40),		fixed(10), fixed(128), 1, TEAM_BLUE);
	SetMapCell(&g_maps[id], 2, fixed(64), fixed((DS_HEIGHT>>1)+40),				fixed(20), fixed(128), 2, TEAM_RED);
	SetMapCell(&g_maps[id], 3, fixed(DS_WIDTH-64), fixed((DS_HEIGHT>>1)+40),		fixed(10), fixed(128), 1, TEAM_BLUE);
	id++;

	//level 3 - neutral cells
	SetMap(&g_maps[id], "Tutorial 3", "The neutral cells don't grow. You can conquer them to accumulate more virusses. ", 3, MAP_DIFFICULTY_NONE, DS_WIDTH+2, DS_HEIGHT+2, 5);
	SetMapCell(&g_maps[id], 0, fixed(64), fixed((DS_HEIGHT>>1)-40), fixed(20),	fixed(128), 2, TEAM_RED);
	SetMapCell(&g_maps[id], 1, fixed(DS_WIDTH-64), fixed((DS_HEIGHT>>1)-40),		fixed(10), fixed(128), 1, TEAM_BLUE);
	SetMapCell(&g_maps[id], 2, fixed(64), fixed((DS_HEIGHT>>1)+40), fixed(20),	fixed(128), 2, TEAM_RED);
	SetMapCell(&g_maps[id], 3, fixed(DS_WIDTH-64), fixed((DS_HEIGHT>>1)+40),		fixed(10), fixed(128), 1, TEAM_BLUE);
	SetMapCell(&g_maps[id], 4, fixed(DS_WIDTH-128), fixed((DS_HEIGHT-96)),		fixed(10), fixed(128), 1, TEAM_NEUTRAL);
	id++;

	//level 4 - growthrate
	SetMap(&g_maps[id], "Tutorial 4", "Cells can have different growth rates. Try to hold control over fast growing cells. ", 4, MAP_DIFFICULTY_NONE, DS_WIDTH+2, DS_HEIGHT+2, 4);
	SetMapCell(&g_maps[id], 0, fixed(64), fixed((DS_HEIGHT>>1)-40), fixed(20),	fixed(128), 10, TEAM_RED);
	SetMapCell(&g_maps[id], 1, fixed(DS_WIDTH-64), fixed((DS_HEIGHT>>1)-40),		fixed(10), fixed(128), 5, TEAM_BLUE);
	SetMapCell(&g_maps[id], 2, fixed(64), fixed((DS_HEIGHT>>1)+40), fixed(20),	fixed(128), 2, TEAM_RED);
	SetMapCell(&g_maps[id], 3, fixed(DS_WIDTH-64), fixed((DS_HEIGHT>>1)+40),		fixed(10), fixed(128), 1, TEAM_BLUE);
	id++;

	//level 5 - capacity
	SetMap(&g_maps[id], "Tutorial 5", "Cells can be smaller and bigger. This affects the maximum amount of virusses they can hold. ", 5, MAP_DIFFICULTY_NONE, DS_WIDTH+2, DS_HEIGHT+2, 4);
	SetMapCell(&g_maps[id], 0, fixed(64), fixed((DS_HEIGHT>>1)-40), fixed(20),	fixed(64), 2, TEAM_RED);
	SetMapCell(&g_maps[id], 1, fixed(DS_WIDTH-64), fixed((DS_HEIGHT>>1)-40),		fixed(10), fixed(32), 2, TEAM_BLUE);
	SetMapCell(&g_maps[id], 2, fixed(64), fixed((DS_HEIGHT>>1)+40),				fixed(20), fixed(64), 2, TEAM_RED);
	SetMapCell(&g_maps[id], 3, fixed(DS_WIDTH-64), fixed((DS_HEIGHT>>1)+40),		fixed(10), fixed(200), 2, TEAM_BLUE);
	id++;

	//level 6 - it would be boring without an enemy
	SetMap(&g_maps[id], "Tutorial 6", "This level is a mixup of what you've learned. It would be boring if you had no enemy would it now...", 6, MAP_DIFFICULTY_EASY, DS_WIDTH+2, DS_HEIGHT+2, 4);
	SetMapCell(&g_maps[id], 0, fixed(45), fixed((DS_HEIGHT>>1)-40),			fixed(10), fixed(80), 3, TEAM_RED);
	SetMapCell(&g_maps[id], 1, fixed(DS_WIDTH-20), fixed((DS_HEIGHT>>1)-60), fixed(10), fixed(32), 3, TEAM_BLUE);
	SetMapCell(&g_maps[id], 2, fixed(70), fixed((DS_HEIGHT>>1)+44),			fixed(10), fixed(64), 3, TEAM_RED);
	SetMapCell(&g_maps[id], 3, fixed(DS_WIDTH-64), fixed((DS_HEIGHT>>1)+40), fixed(10), fixed(60), 3, TEAM_BLUE);
	id++;

	//level 7 - scrolling
	SetMap(&g_maps[id], "Tutorial 7", "This time you have to find your enemy. (Woooh look at those shiny arrow buttons on your DS)", 7, MAP_DIFFICULTY_EASY, 512, 384, 6);
	SetMapCell(&g_maps[id], 0, fixed(120), fixed(100), fixed(40), fixed(60),  3, TEAM_RED);
	SetMapCell(&g_maps[id], 1, fixed(380), fixed(96),  fixed(10), fixed(80),  2, TEAM_BLUE);
	SetMapCell(&g_maps[id], 2, fixed(384), fixed(288), fixed(10), fixed(100), 2, TEAM_BLUE);
	SetMapCell(&g_maps[id], 3, fixed(300), fixed(220), fixed(10), fixed(88),  1, TEAM_NEUTRAL);
	SetMapCell(&g_maps[id], 4, fixed(64),  fixed(200), fixed(10), fixed(140), 1, TEAM_BLUE);
	SetMapCell(&g_maps[id], 5, fixed(180), fixed(320), fixed(10), fixed(200), 1, TEAM_NEUTRAL);
	id++;

	//level 7 - Fair fight 1
	SetMap(&g_maps[id], "Fair fight 1", "It's a fair fight", 8, MAP_DIFFICULTY_EASY, DS_WIDTH, DS_HEIGHT, 5);
	SetMapCell(&g_maps[id], 0, fixed(64), fixed((DS_HEIGHT>>1)-40),			fixed(10), fixed(128), 2, TEAM_RED);
	SetMapCell(&g_maps[id], 1, fixed(DS_WIDTH-64), fixed((DS_HEIGHT>>1)-40),	fixed(10), fixed(128), 1, TEAM_BLUE);
	SetMapCell(&g_maps[id], 2, fixed(64), fixed((DS_HEIGHT>>1)+40),			fixed(10), fixed(128), 2, TEAM_RED);
	SetMapCell(&g_maps[id], 3, fixed(DS_WIDTH-64), fixed((DS_HEIGHT>>1)+40),	fixed(10), fixed(128), 1, TEAM_BLUE);
	SetMapCell(&g_maps[id], 4, fixed(DS_WIDTH-128), fixed((DS_HEIGHT-96)),	fixed(10), fixed(256), 1, TEAM_NEUTRAL);
	id++;

	//level 8 - Fair fight 2
	SetMap(&g_maps[id], "Fair fight 2", "Still a fair fight", 9, MAP_DIFFICULTY_EASY, DS_WIDTH, DS_HEIGHT+2, 5);
	SetMapCell(&g_maps[id], 0, fixed(64), fixed((DS_HEIGHT>>1)-50),			fixed(5), fixed(128), 4, TEAM_RED);
	SetMapCell(&g_maps[id], 1, fixed(DS_WIDTH-80), fixed((DS_HEIGHT>>1)-40), fixed(5), fixed(128), 4, TEAM_BLUE);
	SetMapCell(&g_maps[id], 2, fixed(64), fixed((DS_HEIGHT>>1)+50),			fixed(10), fixed(128), 2, TEAM_RED);
	SetMapCell(&g_maps[id], 3, fixed(DS_WIDTH-70), fixed((DS_HEIGHT>>1)+40), fixed(10), fixed(128), 2, TEAM_BLUE);
	SetMapCell(&g_maps[id], 4, fixed(DS_WIDTH>>1), fixed(DS_HEIGHT>>1),		fixed(2), fixed(256), 3, TEAM_NEUTRAL);
	id++;

	//level 9 - Fair fight 3
	SetMap(&g_maps[id], "Fair fight 3", "It really is", 10, MAP_DIFFICULTY_EASY, DS_WIDTH, DS_HEIGHT, 5);
	SetMapCell(&g_maps[id], 0, fixed(64),			fixed((DS_HEIGHT>>1)-50),	fixed(50), fixed(70), 20, TEAM_RED);
	SetMapCell(&g_maps[id], 1, fixed(DS_WIDTH-80),	fixed((DS_HEIGHT>>1)-40),	fixed(50), fixed(70), 20, TEAM_BLUE);
	SetMapCell(&g_maps[id], 2, fixed(64),			fixed((DS_HEIGHT>>1)+50),	fixed(50), fixed(70), 10, TEAM_RED);
	SetMapCell(&g_maps[id], 3, fixed(DS_WIDTH-70),	fixed((DS_HEIGHT>>1)+40),	fixed(50), fixed(70), 10, TEAM_BLUE);
	SetMapCell(&g_maps[id], 4, fixed(DS_WIDTH>>1),	fixed(DS_HEIGHT>>1),		fixed(2), fixed(256), 13, TEAM_NEUTRAL);
	id++;

	//level 10 - boss stage 1
	SetMap(&g_maps[id], "Boss stage 1", "This time, you don't get no tips!", 11, MAP_DIFFICULTY_MEDIUM, 512, 384, 6);
	SetMapCell(&g_maps[id], 0, fixed(120), fixed(100), fixed(20), fixed(60),  3, TEAM_RED);
	SetMapCell(&g_maps[id], 1, fixed(380), fixed(96), fixed(10), fixed(80),  2, TEAM_BLUE);
	SetMapCell(&g_maps[id], 2, fixed(384), fixed(288), fixed(10), fixed(100), 2, TEAM_BLUE);
	SetMapCell(&g_maps[id], 3, fixed(300), fixed(220), fixed(10), fixed(88),  1, TEAM_NEUTRAL);
	SetMapCell(&g_maps[id], 4, fixed(64), fixed(200), fixed(10), fixed(140), 1, TEAM_BLUE);
	SetMapCell(&g_maps[id], 5, fixed(180), fixed(320), fixed(10), fixed(200), 1, TEAM_NEUTRAL);
	id++;
}