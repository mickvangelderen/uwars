#pragma once

#include <string>
#include "Types.h"

using namespace std;

typedef u16 strsize;

// string to int
int S2I(string & s, bool &check);
string I2S(int i);

string ReadString(FILE * const f);
void WriteString(const string & str, FILE * const f);