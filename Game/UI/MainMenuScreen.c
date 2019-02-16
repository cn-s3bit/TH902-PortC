#include "Screens.h"
#include "../GameContents/RenderManager.h"
#include "../GameContents/ResourceManager.h"
#include "../Constants.h"
#include "../SDLEx/MathEx/MathEx.h"

static Renderable * bg;
static Renderable * start;
static Renderable * quit;

static int current_select;
static int effect_timer;


static void initialize() {
	current_select = 0;
	effect_timer = 0;
	Mix_PlayMusic(resources.BGM.StartingScene, SDL_MAX_SINT16);
	bg = create_empty_renderable();
	bg->Center = vector2_create(480.0f, 360.0f);
	bg->Layer = RENDER_LAYER_BACKGROUND;
	bg->TextureRegion.TextureID = resources.Images.Background[0];
	bg->TextureRegion.Rect = texture_frame_by_id(resources.Images.Background[0]);
	register_renderable(bg);
	start = create_empty_renderable();
	start->Center = vector2_create(620.0f, 540.0f);
	start->Layer = RENDER_LAYER_UI_0;
	start->TextureRegion.TextureID = resources.Images.StoryMode;
	start->TextureRegion.Rect = texture_frame_by_id(resources.Images.StoryMode);
	register_renderable(start);
	quit = create_empty_renderable();
	quit->Center = vector2_create(620.0f, 590.0f);
	quit->Layer = RENDER_LAYER_UI_0;
	quit->TextureRegion.TextureID = resources.Images.Exit;
	quit->TextureRegion.Rect = texture_frame_by_id(resources.Images.Exit);
	register_renderable(quit);
}

static int update() {
	effect_timer++;
	float colorFactor = SDL_cosf(effect_timer * 5.0f * DEGREE_TO_RADIANS) * 0.2f + 0.8f;
	switch (current_select)
	{
	case 0:
		quit->Color = vector4_create(0.18f, 0.18f, 0.18f, 1.0f);
		start->Color = vector4_create(colorFactor, colorFactor, colorFactor, 1.0f);
		break;
	case 1:
		start->Color = vector4_create(0.18f, 0.18f, 0.18f, 1.0f);
		quit->Color = vector4_create(colorFactor, colorFactor, colorFactor, 1.0f);
		break;
	case -100:
		return 1;
	default:
		break;
	}
	return 0;
}

static void render() {
	unsigned imageid = sdlex_begin_frame();
	render_all_layers(imageid);
	sdlex_end_frame(imageid);
}

static short handle_event(SDL_Event ev) {
	if (ev.type == SDL_KEYDOWN) {
		if (ev.key.keysym.scancode == SDL_SCANCODE_Z) {
			if (current_select == 0) {
				current_select = -100;
				Mix_PlayChannel(-1, resources.SE.OK, 0);
			}
			else if (current_select == 1)
				return EXIT_SIGNAL;
		}
		if (ev.key.keysym.scancode == SDL_SCANCODE_DOWN) {
			if (current_select == 0) {
				current_select = 1;
				Mix_PlayChannel(-1, resources.SE.Select, 0);
			}
		}
		if (ev.key.keysym.scancode == SDL_SCANCODE_UP) {
			if (current_select == 1) {
				current_select = 0;
				Mix_PlayChannel(-1, resources.SE.Select, 0);
			}
		}
	}
	return NO_SIGNAL;
}

static void destroy() {
	Mix_PauseMusic();
	unregister_renderable(bg);
	free(bg);
	bg = NULL;
	unregister_renderable(start);
	free(start);
	start = NULL;
	unregister_renderable(quit);
	free(quit);
	quit = NULL;
}

struct Screen main_menu_screen = {
	.initialize = initialize,
	.update = update,
	.render = render,
	.destroy = destroy,
	.handle_event = handle_event
};
