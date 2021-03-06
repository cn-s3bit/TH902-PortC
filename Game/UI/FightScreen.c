#include "Screens.h"
#include "../GameContents/RenderManager.h"
#include "../GameContents/ResourceManager.h"
#include "../GameContents/BasicProjectile.h"
#include "../GameContents/CrazyStormWin32Interop.h"
#include "../Contents/Player/Players.h"
#include "../Constants.h"
#include "../SDLEx/MathEx/MathEx.h"
#include "../SDLEx/Utils/RandomPool.h"

Projectile * projs[1024];
enum PlayerType player_type = PLAYER_REIMU_A;

float fight_area_left = 220.0f;
float fight_area_top = 30.0f;
float fight_area_width = 960.0f;
float fight_area_height = 720.0f;

static Renderable * fight_area;

void * player;


void test_ai0(Projectile * proj) {
	// SDL_Log("%d %f", proj->WhoAmI, proj->Accel.Y);
	float x = random_float_se(random_pool_get(1), -0.5f, 0.5f);
	float y = random_float_se(random_pool_get(1), -0.5f, 0.5f);
	proj->Accel = vector2_scl(vector2_unit(vector2_create(x, y)), 0.03f);
}

CrazyStormInstance * csinstance;

Vector2 get_player_position(void) {
	return get_player_interface(player_type).get_position(player);
}

inline void _add_black_tile(Vector2 center) {
	Renderable * renderer = create_empty_renderable();
	renderer->Center = center;
	renderer->Layer = RENDER_LAYER_OVERLAY;
	renderer->Scale = vector2_create(2.0f, 2.0f);
	renderer->Color = vector4_create(0.0f, 0.0f, 0.0f, 1.0f);
	renderer->TextureRegion.TextureID = resources.Images.FightAreaBackground;
	renderer->TextureRegion.Rect = texture_frame_by_id(resources.Images.FightAreaBackground);
	register_renderable(renderer);
}

static void initialize() {
	fight_area = create_empty_renderable();
	fight_area->Center = vector2_create(fight_area_left + fight_area_width / 2, fight_area_top + fight_area_height / 2);
	fight_area->Layer = RENDER_LAYER_BACKGROUND;
	fight_area->TextureRegion.TextureID = resources.Images.FightAreaBackground;
	fight_area->TextureRegion.Rect = texture_frame_by_id(resources.Images.FightAreaBackground);
	register_renderable(fight_area);
	_add_black_tile(vector2_create(fight_area_left + fight_area_width * 2, fight_area_top + fight_area_height / 2));
	_add_black_tile(vector2_create(fight_area_left + fight_area_width / 2, fight_area_top + fight_area_height * 2));
	_add_black_tile(vector2_create(fight_area_left - fight_area_width, fight_area_top + fight_area_height / 2));
	_add_black_tile(vector2_create(fight_area_left + fight_area_width / 2, fight_area_top - fight_area_height));

	free_projectile(alloc_projectile());
	player = get_player_interface(player_type).initialize(vector2_create(280.0f, 620.0f));
	csinstance = crazy_storm_start(RESOURCE_FOLDER "Game/CrazyStormRT.exe");
	crazy_storm_load_mbg(csinstance, RESOURCE_FOLDER "Game/Danmaku/LD.mbg");
	crazy_storm_register_player_pos(csinstance, get_player_position);
	/*for (int i = 0; i < 1024; i++) {
		projs[i] = alloc_projectile();
		projs[i]->Type = rand() % 7 + 9;
		projs[i]->Position = vector2_create(480.0f, 360.0f);
		projs[i]->AI = test_ai0;
		raii_projectile_renderable_layered(projs[i], RENDER_LAYER_ENTITY_6);
	}*/
}

static int update() {
	get_player_interface(player_type).update(player);
	crazy_storm_start_update_async(csinstance);
	update_projectiles();
	unsigned imageid = sdlex_begin_frame();
	render_all_layers(imageid);
	sdlex_end_frame(imageid);
	crazy_storm_wait_update_async(csinstance);
	return 1;
}

static void render() {
}

static short handle_event(SDL_Event ev) {
	get_player_interface(player_type).handle_event(player, ev);
	if (ev.type == SDL_KEYDOWN) {
		return NO_SIGNAL;
	}
	return NO_SIGNAL;
}

static void destroy() {
	crazy_storm_terminate(csinstance);
}

struct Screen fight_screen = {
	.initialize = initialize,
	.update = update,
	.render = render,
	.destroy = destroy,
	.handle_event = handle_event
};
