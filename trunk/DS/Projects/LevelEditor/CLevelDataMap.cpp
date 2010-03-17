#include "CLevelDataMap.h"

#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>

#include "Types.h"
#include "Team.h"
#include "CLevelDataCell.h"
#include "StrLib.h"

CLevelDataMap::CLevelDataMap()
: m_name(""), m_description(""), 
m_width(0), m_height(0), m_cells(){
}

CLevelDataMap::~CLevelDataMap(){
	Destroy();
}

void CLevelDataMap::Set(string & name, string & description, u32 width, u32 height, vector<CLevelDataCell> & cells){
	Destroy();
	this->m_name = name;
	this->m_description = description;
	this->m_width = width;
	this->m_height = height;
	this->m_cells = cells;
}

void CLevelDataMap::Reset(){
	Destroy();
	m_width = 0;
	m_height = 0;
}

void CLevelDataMap::Destroy(){
	m_name.clear();
	m_description.clear();
	m_cells.clear();
}

void CLevelDataMap::Write(){
	if(m_name.empty()) m_name = "Unnamed";
	if(m_description.empty()) m_description = "No description specified";
	string levelName = m_name + ".lvl";
	FILE * f = fopen(levelName.c_str(), "wb");
	u16 nCells = m_cells.size();
	if(f){
		WriteString(m_name, f);
		WriteString(m_description, f);
		fwrite(&m_width, sizeof(m_width), 1, f);
		fwrite(&m_height, sizeof(m_height), 1, f);
		fwrite(&nCells, sizeof(nCells), 1, f);
		for(u8 i=0; i<nCells; i++){
			m_cells[i].Write(f);
		}
	}
	fclose(f);
}


void CLevelDataMap::Read(string & mapname){
	Destroy();
	string levelName = mapname + ".lvl";

	FILE * f = fopen(levelName.c_str(), "rb");
	if(f){
		m_name = ReadString(f);
		m_description = ReadString(f);
		fread(&m_width, sizeof(m_width), 1, f);
		fread(&m_height, sizeof(m_height), 1, f);
		u16 numberOfCells = 0;
		fread(&numberOfCells, sizeof(numberOfCells), 1, f);
		if(numberOfCells > 0){
			CLevelDataCell temp;
			for(u8 i=0; i<numberOfCells; i++){
				temp.Read(f);
				m_cells.push_back(temp);
			}
		}
	}
	else{
		std::cout<<"Couln't find/open file for reading\n";
	}
}