#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "SDLEx/SDLWithPlugins.h"
#include "SDLEx/Utils/MathUtils.h"
#include "SDLEx/Utils/FileUtils.h"
#include "SDLEx/Utils/RandomPool.h"
#include "SDLEx/Utils/Ellipse.h"
#include "SDLEx/MathEx/MathEx.h"
#include "Constants.h"
#include "SDLEx/Vulkan/SDLExVulkan.h"
#include "GameContents/BasicProjectile.h"
#include "GameContents/ResourceManager.h"
#include "UI/Screens.h"

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

int activeScene = 0;

short handle_event(void) {
	SDL_Event e;
	if (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) {
			return EXIT_SIGNAL;
		}
		switch (activeScene)
		{
		case 0:
			return main_menu_screen.handle_event(e);
		case 1:
			return fight_screen.handle_event(e);
		default:
			break;
		}
	}
	return NO_SIGNAL;
}

static void new_active_scene(int id) {
	switch (id)
	{
	case 0:
		main_menu_screen.initialize();
		break;
	case 1:
		fight_screen.initialize();
		break;
	default:
		break;
	}
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
	int t = 0;
	main_menu_screen.initialize();
	/* Main Loop */
	int r0;
	while (1) {
		t++;
		if (handle_event() == EXIT_SIGNAL)
			goto LABEL_EXIT;
		clock_t b = clock();
		switch (activeScene)
		{
		case 0:
			r0 = main_menu_screen.update();
			main_menu_screen.render();
			if (r0 != 0) {
				activeScene = r0;
				main_menu_screen.destroy();
				new_active_scene(activeScene);
			}
			break;
		case 1:
			fight_screen.update();
			fight_screen.render();
		default:
			break;
		}
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
