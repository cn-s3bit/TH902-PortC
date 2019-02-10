#ifndef RENDER_MANAGER_H
#define RENDER_MANAGER_H
#include "../SDLEx/SDLWithPlugins.h"
#include "../SDLEx/MathEx/MathEx.h"
#include "../SDLEx/Utils/Deque.h"
enum RenderLayer {
	RENDER_LAYER_BACKGROUND = 1,  // Background
	RENDER_LAYER_ENTITY_0 = 4,  // Bullet of Player 1
	RENDER_LAYER_ENTITY_1 = 8,  // Bullet of Player 2
	RENDER_LAYER_ENTITY_2 = 12,  // Player & Player Wing
	RENDER_LAYER_ENTITY_3 = 16,  // NPC, except Boss
	RENDER_LAYER_ENTITY_4 = 20,  // Enemy Bullets Lower Layer 1
	RENDER_LAYER_ENTITY_5 = 24,  // Enemy Bullets Lower Layer 2
	RENDER_LAYER_ENTITY_6 = 28,  // Enemy Bullets Additive
	RENDER_LAYER_ENTITY_7 = 32,  // Enemy Bullets High Layer
	RENDER_LAYER_ENTITY_8 = 36,  // Boss
	RENDER_LAYER_ENTITY_9 = 40,  // Particles
	RENDER_LAYER_UI_0 = 51,  // UI 1
	RENDER_LAYER_UI_1 = 52,  // UI 2
	RENDER_LAYER_UI_2 = 53,  // UI 3
	RENDER_LAYER_MAX = 64,  // Array Length
};
typedef struct TextureRegion {
	long TextureID;
	SDL_Rect Rect;
} TextureRegion;
typedef struct Renderable {
	TextureRegion TextureRegion;
	// _internalId should not be changed as long as the Renderable is registered
	long long _internalId;
	// Layer should not be changed as long as the Renderable is registered
	enum RenderLayer Layer;
	Vector4 Color;
	Vector2 Center;
	float Rotation;
	Vector2 Scale;
} Renderable;

void register_renderable(Renderable * renderable);
void unregister_renderable(Renderable * renderable);

#endif
