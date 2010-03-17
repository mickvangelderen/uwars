#pragma once

#include "CLevelDataCell.h"
#include "Types.h"
#include "Team.h"

#include <vector>
#include <string>

using namespace std;

class CLevelDataMap{
public:
	string m_name;
	string m_description;
	u32 m_width;
	u32 m_height;
	vector<CLevelDataCell> m_cells;
public:
	CLevelDataMap();
	~CLevelDataMap();
	void Set(string & name, string & description, u32 width, u32 height, vector<CLevelDataCell> & cells);
	void Reset();
	void Destroy();
	void Write();
	void Read(string & mapname);
};