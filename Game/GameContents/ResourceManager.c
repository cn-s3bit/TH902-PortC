#include "ResourceManager.h"
#include <stdio.h>
#include "../Constants.h"
#include "../SDLEx/Vulkan/SDLExVulkan.h"
struct Resources resources;

inline Mix_Chunk * _load_se(const char * name) {
	char buffer[1023];
#ifdef _MSC_VER
	sprintf_s(buffer, 1023u, "%s%s%s", RESOURCE_FOLDER "Game/SE/se_", name, ".mp3");
#else
	sprintf(buffer, "%s%s%s", RESOURCE_FOLDER "Game/SE/", name, ".mp3");
#endif
	return Mix_LoadWAV(buffer);
}

void load_resources() {
	resources.Images.Barrages = load_texture2d(RESOURCE_FOLDER "Game/Image/Barrages.0.png");
	Mix_Init(MIX_INIT_MP3);
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
	resources.BGM.StartingScene = Mix_LoadMUS(RESOURCE_FOLDER "Game/BGM/Starting Scene.mp3");
	resources.SE.Biu = _load_se("pldead00");
	resources.SE.Cat = _load_se("cat00");
	resources.SE.Select = _load_se("select00");
	resources.SE.OK = _load_se("ok00");
}

void release_resources() {
	dispose_texture2d(resources.Images.Barrages);
	Mix_FreeMusic(resources.BGM.StartingScene);
	Mix_FreeChunk(resources.SE.Biu);
	Mix_FreeChunk(resources.SE.Cat);
	Mix_FreeChunk(resources.SE.Select);
	Mix_FreeChunk(resources.SE.OK);
	Mix_CloseAudio();
	Mix_Quit();
}
