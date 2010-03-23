#pragma once
#include <PA9.h>

extern bool FAT_INIT_SUCCESS;

#define SAVE_DIR "/DATA/uWars"
#define SAVE_PATH "/DATA/uWars/PlayerProfiles.SAV"
#define READ_MODE "rb"
#define WRITE_MODE "wb"

bool InitFat();