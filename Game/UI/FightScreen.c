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

void * player;


void test_ai0(Projectile * proj) {
	// SDL_Log("%d %f", proj->WhoAmI, proj->Accel.Y);
	float x = random_float_se(random_pool_get(1), -0.5f, 0.5f);
	float y = random_float_se(random_pool_get(1), -0.5f, 0.5f);
	proj->Accel = vector2_scl(vector2_unit(vector2_create(x, y)), 0.03f);
}

CrazyStormInstance * csinstance;

static Vector2 _t(void) {
	return get_player_interface(player_type).get_position(player);
}

static void initialize() {
	free_projectile(alloc_projectile());
	player = get_player_interface(player_type).initialize(vector2_create(280.0f, 620.0f));
	csinstance = crazy_storm_start(RESOURCE_FOLDER "Game/CrazyStormRT.exe");
	crazy_storm_load_mbg(csinstance, RESOURCE_FOLDER "Game/Image/Danmaku/LD.mbg");
	crazy_storm_register_player_pos(csinstance, _t);
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
	crazy_storm_wait_update_async(csinstance);
	return 1;
}

static void render() {
	unsigned imageid = sdlex_begin_frame();
	render_all_layers(imageid);
	sdlex_end_frame(imageid);
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
