#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "SDLEx/SDLWithPlugins.h"
#include "SDLEx/Utils/MathUtils.h"
#include "SDLEx/Utils/FileUtils.h"
#include "SDLEx/MathEx/MathEx.h"
#include "Constants.h"
#include "SDLEx/Vulkan/SDLExVulkan.h"

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

int main(int argc, char ** argv) {
	if (argc > 0)
		SDL_Log("Working Path: %s\n", argv[0]);
	init_sdl();
	SDL_Window * window = SDL_CreateWindow("TH902", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 960, 720, SDL_WINDOW_VULKAN);

	TTF_Font * testFont = TTF_OpenFont(DEFAULT_FONT_PATH, 32);

	initialize_vulkan(window, VK_MAKE_VERSION(0, 1, 0));
	create_graphics_pipeline_f(RESOURCE_FOLDER "Shaders/default.vert.spv", RESOURCE_FOLDER "Shaders/default.frag.spv");
	int texture_id = load_texture2d(RESOURCE_FOLDER "Game/Image/Ming/Ming.png");
	for (unsigned m = 0; m < get_vk_swap_chain()->ImageCount; m++)
		bind_texture2d(m, texture_id);
	int t = 0;
	// Main Loop
	while (1) {
		t++;
		if (handle_event() == EXIT_SIGNAL)
			goto LABEL_EXIT;
		clock_t b = clock();
		unsigned imageid = sdlex_begin_frame();
		for (int i = 0; i < SDL_max(200 - t / 3, 1); i++) {
			SDL_Rect p1 = { 0, 0, 200, 400 };
			p1.x += t + i;
			sdlex_render_texture(imageid, p1);
			sdlex_render_texture_ex(imageid,
				vector2_scl(vector2_one(), (float)t),
				vector2_adds(vector2_zero(), 200, 400),
				1.0f * (float)(t + i),
				vector2_scl(vector2_one(), 0.4f));
		}
		sdlex_end_frame(imageid);
		SDL_Log("%d", clock() - b);
	}

LABEL_EXIT:
	vkDeviceWaitIdle(get_vk_device());
	TTF_CloseFont(testFont);
	cleanup_vulkan_pipeline();
	cleanup_vulkan();
	// SDL_DestroyWindow(window);
	cleanup_sdl();
	printf("Press Enter to Exit...");
	getchar();
	return 0;
}