#ifndef RENDER_MANAGER_H
#define RENDER_MANAGER_H
#include "../SDLEx/SDLWithPlugins.h"
#include "../SDLEx/MathEx/MathEx.h"
enum RenderLayer {
	RENDER_LAYER_BACKGROUND = 0,
	RENDER_LAYER_ENTITY_0 = 10,  // Bullet of Player 1
	RENDER_LAYER_ENTITY_1 = 20,  // Bullet of Player 2
	RENDER_LAYER_ENTITY_2 = 30,  // Player & Player Wing
	RENDER_LAYER_ENTITY_3 = 40,  // NPC, except Boss
	RENDER_LAYER_ENTITY_4 = 50,  // Enemy Bullets Lower Layer 1
	RENDER_LAYER_ENTITY_5 = 60,  // Enemy Bullets Lower Layer 2
	RENDER_LAYER_ENTITY_6 = 70,  // Enemy Bullets Additive
	RENDER_LAYER_ENTITY_7 = 80,  // Enemy Bullets High Layer
	RENDER_LAYER_ENTITY_8 = 90,  // Boss
	RENDER_LAYER_ENTITY_9 = 100,  // Particles
	RENDER_LAYER_UI_0 = 201,  // UI 1
	RENDER_LAYER_UI_1 = 202,  // UI 2
	RENDER_LAYER_UI_2 = 203,  // UI 3
	RENDER_LAYER_MAX = 255,  // Array Length
};
typedef struct TextureRegion {
	long TextureID;
	SDL_Rect Rect;
} TextureRegion;
typedef struct Renderable {
	enum RenderLayer Layer;
	TextureRegion TextureRegion;
	Vector4 Color;
	Vector2 Center;
	float Rotation;
	Vector2 Scale;
} Renderable;

#endif
