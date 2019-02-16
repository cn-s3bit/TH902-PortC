#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "SDLEx/SDLWithPlugins.h"
#include "SDLEx/Utils/MathUtils.h"
#include "SDLEx/Utils/FileUtils.h"
#include "SDLEx/Utils/RandomPool.h"
#include "SDLEx/MathEx/MathEx.h"
#include "Constants.h"
#include "SDLEx/Vulkan/SDLExVulkan.h"
#include "GameContents/BasicProjectile.h"
#include "GameContents/ResourceManager.h"

#include "SDLEx/Utils/HashMap.h"


CODEGEN_CUCKOO_HASHMAP(intintmap, int, int, sdlex_hash_int, sdlex_equal_int, memorypool_free_4bytes, memorypool_free_4bytes)

void init_sdl(void) {
	SDL_Init(SDL_INIT_EVERYTHING);
	IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);
	TTF_Init();
}

void cleanup_sdl(void) {
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

short handle_event(void) {
	SDL_Event e;
	if (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) {
			return EXIT_SIGNAL;
		}
	}
	return NO_SIGNAL;
}

void test_ai0(Projectile * proj) {
	// SDL_Log("%d %f", proj->WhoAmI, proj->Accel.Y);
	float x = random_float_se(random_pool_get(1), -0.5f, 0.5f);
	float y = random_float_se(random_pool_get(1), -0.5f, 0.5f);
	proj->Accel = vector2_scl(vector2_unit(vector2_create(x, y)), 0.03f);
}

int main(int argc, char ** argv) {
	if (argc > 0)
		SDL_Log("Working Path: %s\n", argv[0]);
	init_sdl();
	SDL_Window * window = SDL_CreateWindow("TH902", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 960, 720, SDL_WINDOW_VULKAN | SDL_WINDOW_ALLOW_HIGHDPI);
	if (window == NULL) {
		window = SDL_CreateWindow("TH902", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 960, 720, SDL_WINDOW_ALLOW_HIGHDPI);
	}
	SDL_Surface * icon = IMG_Load(RESOURCE_FOLDER "Game/Image/icon32.png");
	SDL_SetWindowIcon(window, icon);
	TTF_Font * testFont = TTF_OpenFont(DEFAULT_FONT_PATH, 32);

	initialize_vulkan(window, VK_MAKE_VERSION(0, 1, 0));
	create_graphics_pipeline_f(RESOURCE_FOLDER "Shaders/default.vert.spv", RESOURCE_FOLDER "Shaders/default.frag.spv");
	load_resources();
	Mix_VolumeMusic(127);
	Mix_PlayMusic(resources.BGM.StartingScene, 0);
	SDL_Log("C: %d", Mix_PlayChannel(-1, resources.SE.Biu, 0));
	int t = 0;
	Projectile * projs[1024];
	for (int i = 0; i < 1024; i++) {
		projs[i] = alloc_projectile();
		projs[i]->Type = rand() % 7 + 9;
		projs[i]->Position = vector2_create(480.0f, 360.0f);
		projs[i]->AI = test_ai0;
		raii_projectile_renderable(projs[i]);
	}
	/* Main Loop */
	while (1) {
		t++;
		if (handle_event() == EXIT_SIGNAL)
			goto LABEL_EXIT;
		clock_t b = clock();
		sdlex_set_blend_mode(SDLEX_BLEND_MODE_ADDITIVE);
		update_projectiles();
		unsigned imageid = sdlex_begin_frame();
		render_all_layers(imageid);
		sdlex_end_frame(imageid);
		SDL_Log("%d", clock() - b);
		SDL_Delay(SDL_max(16 - clock() + b, 0));
	}

LABEL_EXIT:
	release_resources();
	/* TTF_CloseFont(testFont); */
	if (!sdlex_is_software_fallback_enabled()) {
		vkDeviceWaitIdle(get_vk_device());
		cleanup_vulkan_pipeline();
		cleanup_vulkan();
	}
	cleanup_sdl();
	printf("Press Enter to Exit...");
	getchar();
	return 0;
}
