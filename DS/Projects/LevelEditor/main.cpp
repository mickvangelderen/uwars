#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <stdio.h>

#include <string>

#include "Types.h"
#include "Team.h"
#include "CLevelDataMap.h"
#include "CLevelDataCell.h"
#include "StrLib.h"

//#include "CLevelDataMap.cpp"
//#include "StrLib.cpp"

#define null 0

using namespace std;

string QueryString(const char * const question){
	string s;
	cout<<"Please enter "<<question<<"\n > ";
	getline(cin, s, '\n');
	return s;
}
s32 QueryNumber(const char * const question){
	string s;
	s32 i = 0;
	bool good = false;
	while(!good){
		cout<<"Please enter "<<question<<"\n > ";
		getline(cin, s, '\n');
		i = S2I(s, good);
		if(!good){
			cout<<"Please enter a number"<<endl;
		}
	}
	return i;
}

int main(){
	int input = 3;
	while(input!=0){
		input = 3;
		while(input<0 || input>2){
			cout<<"PWS uWars - Level editor v1.0 unstable version\n";
			cout<<"By Jelle Licht and Mick van Gelderen\n\n";
			cout<<"What do you want to do?\n";
			cout<<" 1 Load\n 2 Create\n 0 Exit\n";
			input = QueryNumber("an action");
		}
		CLevelDataMap lvl;
		string loadMapName;
		u16 nCells;
		vector<CLevelDataCell> cells;
		CLevelDataCell tempCell;

		switch(input){
		case 1: {
				loadMapName = QueryString("the map name");
				lvl.Read(loadMapName);
				if(!lvl.m_name.empty()){
					cout<<"Level name: "<<lvl.m_name<<endl;
					cout<<"Level description: "<<lvl.m_description<<endl;
				}
			}
			break;
		case 2: {
			lvl.m_name = QueryString("map name");
			lvl.m_description = QueryString("map description");
			lvl.m_width = QueryNumber("map width");
			lvl.m_height = QueryNumber("map height");
			nCells = QueryNumber("the number of cells");

			for(u8 i=0; i<nCells; i++){
				cout<<"\n--- Cell "<<(int)i+1<<"/"<<(int)nCells<<" ---\n";
				tempCell.x = QueryNumber(" x: ");
				tempCell.y = QueryNumber(" y: ");
				tempCell.strength = QueryNumber(" strength: ");
				tempCell.capacity = QueryNumber(" capacity: ");
				tempCell.growthRate = QueryNumber(" growth rate: ");
				tempCell.team = (TEAM)QueryNumber(" team: ");
				cells.push_back(tempCell);
			}
			lvl.m_cells = cells;
			cout<<"Attempting save level to "<<lvl.m_name<<".lvl ..."<<endl;
			lvl.Write();
			cout<<"Saved successfully :)"<<endl;
			system("PAUSE");
			}
			break;
		default:{
			}
			break;
		}
		cout<<"\n\n";
	}
	return 0;
}