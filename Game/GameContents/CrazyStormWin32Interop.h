#ifndef CRAZY_STORM_WIN32_INTEROP
#define CRAZY_STORM_WIN32_INTEROP
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>
#include "RenderManager.h"
#include "BasicProjectile.h"
#include "ResourceManager.h"

typedef struct CrazyStormInstance {
	PROCESS_INFORMATION ProcessInformation;
	HANDLE StdInRead, StdInWrite, StdOutRead, StdOutWrite;
	Renderable _additive[4096];
	Renderable _alphablend[4096];
	int _additiveActive;
	int _alphablendActive;
	Vector2(*_playerPosGenerator)(void);
} CrazyStormInstance;

CrazyStormInstance * crazy_storm_start(const char * path2exe);
void crazy_storm_load_mbg(CrazyStormInstance * instance, const char * path2mbg);
void crazy_storm_update(CrazyStormInstance * instance);
void crazy_storm_terminate(CrazyStormInstance * instance);
void crazy_storm_register_player_pos(CrazyStormInstance * instance, Vector2(*generator)(void));
void crazy_storm_start_update_async(CrazyStormInstance * instance);
void crazy_storm_wait_update_async(CrazyStormInstance * instance);
#endif
