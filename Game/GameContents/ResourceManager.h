#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H
#include "SDL_mixer.h"

struct Resources {
	struct {
		long Barrages;
		long Exit;
		long Background[5];
		long StoryMode;
		long ReimuOneFrame;
		long ReimuAnim;
	} Images;
	struct {
		Mix_Chunk * Biu;
		Mix_Chunk * Cat;
		Mix_Chunk * OK;
		Mix_Chunk * Select;
	} SE;
	struct {
		Mix_Music * StartingScene;
	} BGM;
};

extern struct Resources resources;

void load_resources();
void release_resources();
#endif
