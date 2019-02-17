#include <stdlib.h>
#include "Players.h"

#define set_action(v, bit) v |= (1 << bit)
#define clear_action(v, bit) v &= (0xffffffffu - (1 << bit))
#define get_action(v, bit) (v & (1 << bit))

inline void sync_renderer_player_reimu_a(PlayerReimuA * thiz) {
	thiz->Renderer.Center = thiz->Position;
}

PlayerReimuA * initialize_player_reimu_a(Vector2 position) {
	PlayerReimuA * p = malloc(sizeof(PlayerReimuA));
	p->Position = position;
	p->Velocity = vector2_zero();
	p->Renderer.Color = vector4_create(1.0f, 1.0f, 1.0f, 1.0f);
	p->Renderer.Layer = RENDER_LAYER_ENTITY_2;
	p->Renderer.Rotation = 0.0f;
	p->Renderer.Scale = vector2_create(0.5f, 0.5f);
	p->Renderer.TextureRegion.TextureID = resources.Images.ReimuAnim;
	p->Renderer.TextureRegion.Rect.w = 128;
	p->Renderer.TextureRegion.Rect.h = 190;
	p->Renderer.TextureRegion.Rect.x = 0;
	p->Renderer.TextureRegion.Rect.y = 0;
	sync_renderer_player_reimu_a(p);
	register_renderable(&p->Renderer);
	return p;
}

void update_player_reimu_a(void * thiz) {
	PlayerReimuA * p = (PlayerReimuA *)thiz;
	p->Velocity = vector2_zero();
	if (get_action(p->Action, 2))
		p->Velocity = vector2_adds(p->Velocity, 0.0f, -1.0f);
	if (get_action(p->Action, 3))
		p->Velocity = vector2_adds(p->Velocity, 0.0f, 1.0f);
	if (get_action(p->Action, 4))
		p->Velocity = vector2_adds(p->Velocity, -1.0f, 0.0f);
	if (get_action(p->Action, 5))
		p->Velocity = vector2_adds(p->Velocity, 1.0f, 0.0f);
	p->Velocity = vector2_scl(vector2_unit(p->Velocity), 6.0f);
	p->Position = vector2_add(p->Position, p->Velocity);
	sync_renderer_player_reimu_a(p);
}

void handle_event_reimu_a(void * thiz, SDL_Event ev) {
#define update_action(v, bit) if (ev.type == SDL_KEYDOWN) set_action(v, bit); else clear_action(v, bit)
	PlayerReimuA * p = (PlayerReimuA *)thiz;
	if (ev.type == SDL_KEYDOWN || ev.type == SDL_KEYUP) {
		if (ev.key.keysym.scancode == SDL_SCANCODE_DOWN) {
			update_action(p->Action, 3);
		}
		else if (ev.key.keysym.scancode == SDL_SCANCODE_UP) {
			update_action(p->Action, 2);
		}
		else if (ev.key.keysym.scancode == SDL_SCANCODE_LEFT) {
			update_action(p->Action, 4);
		}
		else if (ev.key.keysym.scancode == SDL_SCANCODE_RIGHT) {
			update_action(p->Action, 5);
		}
	}
#undef update_action
}

const PlayerInterface ReimuA = {
	.update = update_player_reimu_a,
	.initialize = initialize_player_reimu_a,
	.handle_event = handle_event_reimu_a
};
