#pragma once
#include <PA9.h>

#define PLAYER_PROFILE_MAX_NAMELENGTH 16
#define PLAYER_PROFILE_EXP_FOR_LVLUP 10



class PlayerProfile{
private:
	char m_name[PLAYER_PROFILE_MAX_NAMELENGTH+1]; //+1 for terminating 0
	s8 m_nameLength;

	u8 m_level;
	u16 m_experience;

	//m_playtime;
	//m_lastsaved;
public:
	PlayerProfile();
	PlayerProfile(const PlayerProfile &pp);
	~PlayerProfile();
	void Copy(const PlayerProfile &pp);

	void Set(const char * const name, u8 level, u16 exp);
	void SetName(const char * const name);
	void SetLevel(u8 level);
	void SetExperience(u16 exp);
	void AddExperience(u16 exp);
	bool CheckLevelUp();

	const char * const Name()const;
	u8 NameLength()const;
	u8 Level()const;
	u8 Experience()const;

	bool IsInitialized()const;

	void Read(FILE * f);
	void Write(FILE * f);
};


void LoadPlayerProfiles(PlayerProfile * pp, u8 amount);
void SavePlayerProfiles(PlayerProfile * pp, u8 amount);