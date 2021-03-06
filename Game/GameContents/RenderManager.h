#ifndef RENDER_MANAGER_H
#define RENDER_MANAGER_H
#include <stdlib.h>
#include "../SDLEx/SDLWithPlugins.h"
#include "../SDLEx/MathEx/MathEx.h"
#include "../SDLEx/Utils/Deque.h"
#include "../SDLEx/Vulkan/SDLExVulkan.h"
enum RenderLayer {
	RENDER_LAYER_BACKGROUND = 1,  /* Background */
	RENDER_LAYER_ENTITY_0 = 4,  /* Bullet of Player 1 */
	RENDER_LAYER_ENTITY_1 = 8,  /* Bullet of Player 2 */
	RENDER_LAYER_ENTITY_2 = 12,  /* Player */
	RENDER_LAYER_ENTITY_2_ADDITIONAL = 13,  /* Judge Display & Player Wing */
	RENDER_LAYER_ENTITY_3 = 16,  /* NPC, except Boss */
	RENDER_LAYER_ENTITY_4 = 20,  /* Enemy Bullets Lower Layer 1 */
	RENDER_LAYER_ENTITY_5 = 24,  /* Enemy Bullets Lower Layer 2 */
	RENDER_LAYER_ENTITY_6 = 28,  /* Enemy Bullets Additive */
	RENDER_LAYER_ENTITY_7 = 32,  /* Enemy Bullets High Layer */
	RENDER_LAYER_ENTITY_8 = 36,  /* Boss */
	RENDER_LAYER_ENTITY_9 = 40,  /* Particles */
	RENDER_LAYER_UI_0 = 51,  /* UI 1 */
	RENDER_LAYER_UI_1 = 52,  /* UI 2 */
	RENDER_LAYER_UI_2 = 53,  /* UI 3 */
	RENDER_LAYER_OVERLAY = 63,  /* Screen Overlay */
	RENDER_LAYER_MAX = 64,  /* Array Length */
};
typedef struct TextureRegion {
	long TextureID;
	SDL_Rect Rect;
} TextureRegion;
typedef struct Renderable {
	TextureRegion TextureRegion;
	/* _internalId should not be changed as long as the Renderable is registered */
	unsigned _internalId;
	/* Layer should not be changed as long as the Renderable is registered */
	enum RenderLayer Layer;
	Vector4 Color;
	Vector2 Center;
	float Rotation;
	Vector2 Scale;
} Renderable;

void register_renderable(Renderable * renderable);
void unregister_renderable(Renderable * renderable);
void sort_layer_for_batching(int layerId);
void render_layer(unsigned imageId, int layerId);
void render_all_layers(unsigned imageId);

inline Renderable * create_empty_renderable() {
	Renderable * r = malloc(sizeof(Renderable));
	r->Color = vector4_create(1.0f, 1.0f, 1.0f, 1.0f);
	r->Rotation = 0.0f;
	r->Scale = vector2_one();
	return r;
}

#endif
