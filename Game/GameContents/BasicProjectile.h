#ifndef BASIC_PROJECTILE_H
#define BASIC_PROJECTILE_H

#include "RenderManager.h"

typedef struct Projectile {
	short Active;
	short type;
	int WhoAmI;
	Vector2 Position;  // Center
	Vector2 Velocity;
	Vector2 Accel;
	Renderable * RenderablePt;
	void * CustomData;
	void (*AI)(void * proj);
} Projectile;

typedef struct ProjectileTypeDescriptor {
	SDL_Rect Region;
	Vector2 Origin;
	float JudgeRadius;
	short ColorID;
} ProjectileTypeDescriptor;


void update_basic_projectile(Projectile * proj);
Projectile * alloc_projectile();
void free_projectile(Projectile * proj);
void update_projectiles();

#endif
