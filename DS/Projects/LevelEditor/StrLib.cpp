#include "StrLib.h"

#include <stdio.h>
#include <sstream>
#include <string>

int S2I(string & s, bool &check){
	istringstream buffer(s);
	int i=0;
	if(buffer>>i) check = true;
	else check = false;
	return i;
}

string I2S(int i){
	ostringstream buffer;
	buffer<<i;
	return buffer.str();
}

string ReadString(FILE * const f){
	strsize strLen = 0;
	fread(&strLen, sizeof(strsize), 1, f);
	char * buffer = new char[strLen+1];
	if(buffer != NULL && strLen > 0)
		fread(buffer, sizeof(char), strLen, f);
	buffer[strLen] = '\0';
	return string(buffer);
}

void WriteString(const string & str, FILE * const f){
	strsize strLen = str.size();
	fwrite(&strLen, sizeof(strsize), 1, f);
	if(strLen > 0)
		fwrite(str.c_str(), sizeof(char), strLen, f);
}