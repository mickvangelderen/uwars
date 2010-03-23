#include <PA9.h>
#include "PlayerProfile.h"
#include "DSspecs.h"

#include "InitFat.h"
#include <fat.h>
#include <unistd.h>
#include <sys/dir.h>


PlayerProfile::PlayerProfile()
	:m_nameLength(0), m_level(0), m_experience(0){
	SetName("");
}

PlayerProfile::PlayerProfile(const PlayerProfile &pp)
	:m_level(pp.m_level), m_experience(pp.m_experience){
	SetName(pp.m_name);
}

PlayerProfile::~PlayerProfile(){};


void PlayerProfile::Copy(const PlayerProfile &pp){
	Set(pp.m_name, pp.m_level, pp.m_experience);
}


void PlayerProfile::Set(const char * const name, u8 level, u16 exp){
	SetName(name);
	SetLevel(level);
	SetExperience(exp);
}

void PlayerProfile::SetName(const char * const name){
	for(m_nameLength=0; m_nameLength<PLAYER_PROFILE_MAX_NAMELENGTH; m_nameLength++){
		m_name[m_nameLength] = name[m_nameLength];
		if(m_name[m_nameLength] == '\0') break;
	}
	m_name[PLAYER_PROFILE_MAX_NAMELENGTH] = '\0';
}


void PlayerProfile::SetLevel(u8 level){
	m_level = level;
}

void PlayerProfile::SetExperience(u16 exp){
	m_experience = exp;
}

void PlayerProfile::AddExperience(u16 exp){
	m_experience += exp;
}

bool PlayerProfile::CheckLevelUp(){
	if(m_experience >= PLAYER_PROFILE_EXP_FOR_LVLUP){
		m_level++;
		m_experience -= PLAYER_PROFILE_EXP_FOR_LVLUP;
		return true;
	}
	return false;
}


const char * const PlayerProfile::Name()const{
	return m_name;
}
u8 PlayerProfile::NameLength()const{
	return m_nameLength;
}
u8 PlayerProfile::Level()const{
	return m_level;
}

u8 PlayerProfile::Experience()const{
	return m_experience;
}

bool PlayerProfile::IsInitialized()const{
	return !(m_nameLength == 0 && m_name[0] == '\0' && m_level == 0 && m_experience == 0);
}

void PlayerProfile::Read(FILE * f){
	fread(&m_nameLength, sizeof(m_nameLength), 1, f);
	fread(m_name, sizeof(char), PLAYER_PROFILE_MAX_NAMELENGTH+1, f);
	fread(&m_level, sizeof(m_level), 1, f);
	fread(&m_experience, sizeof(m_experience), 1, f);
}

void PlayerProfile::Write(FILE * f){
	fwrite(&m_nameLength, sizeof(m_nameLength), 1, f);
	fwrite(m_name, sizeof(char), PLAYER_PROFILE_MAX_NAMELENGTH+1, f);
	fwrite(&m_level, sizeof(m_level), 1, f);
	fwrite(&m_experience, sizeof(m_experience), 1, f);
}

void LoadPlayerProfiles(PlayerProfile * pp, u8 amount){
	if(FAT_INIT_SUCCESS){
		FILE *f = fopen(SAVE_PATH, READ_MODE);
		if(f != NULL){
			for(u8 i=0; i<amount; i++) pp[i].Read(f);
			fclose(f);
			return;
		}
	}
	for(u8 i=0; i<amount; i++) pp[i].Set("", 0, 0); //no success, fill the classes with emptyness
	//debug account
	pp[0].Set("No saving:(", 40, 8);
}

void SavePlayerProfiles(PlayerProfile * pp, u8 amount){
	PA_InitText(DS_TOP, BG_TEXT);
	if(FAT_INIT_SUCCESS){
		PA_OutputSimpleText(DS_TOP, 0, 0, "fatinit success");
		DIR_ITER* dir;
		dir = diropen(SAVE_DIR);
		if(dir == NULL){
			PA_OutputSimpleText(DS_TOP, 0, 3, "can't open directory, creating directory");
			mkdir(SAVE_DIR, 0);
			dir = diropen(SAVE_DIR);
			if(dir == NULL){
				PA_OutputSimpleText(DS_TOP, 0, 4, "can't create directory");
				return;
			}
		}
		PA_OutputSimpleText(DS_TOP, 0, 5, "changing directory");
		chdir(SAVE_DIR);
		PA_OutputSimpleText(DS_TOP, 0, 6, "opening savefile");
		FILE *f = fopen(SAVE_PATH, WRITE_MODE);
		if(f != NULL){
			PA_OutputSimpleText(DS_TOP, 0, 7, "saving");
			for(u8 i=0; i<amount; i++) pp[i].Write(f);
			fclose(f);
		}
		PA_OutputSimpleText(DS_TOP, 0, 8, "saving done");
	}
	else{
		PA_OutputSimpleText(DS_TOP, 0, 0, "saving is not supported on emulators");
	}
}