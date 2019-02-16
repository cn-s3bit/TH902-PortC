#include "Screens.h"
#include "../GameContents/RenderManager.h"
#include "../GameContents/ResourceManager.h"
#include "../GameContents/BasicProjectile.h"
#include "../Constants.h"
#include "../SDLEx/MathEx/MathEx.h"
#include "../SDLEx/Utils/RandomPool.h"

Projectile * projs[1024];


void test_ai0(Projectile * proj) {
	// SDL_Log("%d %f", proj->WhoAmI, proj->Accel.Y);
	float x = random_float_se(random_pool_get(1), -0.5f, 0.5f);
	float y = random_float_se(random_pool_get(1), -0.5f, 0.5f);
	proj->Accel = vector2_scl(vector2_unit(vector2_create(x, y)), 0.03f);
}

static void initialize() {
	for (int i = 0; i < 1024; i++) {
		projs[i] = alloc_projectile();
		projs[i]->Type = rand() % 7 + 9;
		projs[i]->Position = vector2_create(480.0f, 360.0f);
		projs[i]->AI = test_ai0;
		raii_projectile_renderable_layered(projs[i], RENDER_LAYER_ENTITY_6);
	}
}

static int update() {
	update_projectiles();
}

static void render() {
	unsigned imageid = sdlex_begin_frame();
	render_all_layers(imageid);
	sdlex_end_frame(imageid);
}

static short handle_event(SDL_Event ev) {
	if (ev.type == SDL_KEYDOWN) {
		return NO_SIGNAL;
	}
	return NO_SIGNAL;
}

static void destroy() {

}

struct Screen fight_screen = {
	.initialize = initialize,
	.update = update,
	.render = render,
	.destroy = destroy,
	.handle_event = handle_event
};
