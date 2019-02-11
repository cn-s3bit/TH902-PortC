#ifndef BASIC_PROJECTILE_H
#define BASIC_PROJECTILE_H

#include "RenderManager.h"

typedef struct Projectile {
	short Active;
	int WhoAmI;
	Vector2 Position;  // Center
	Vector2 Velocity;
	Vector2 Accel;
	Renderable * RenderablePt;
	void (*AI)(void * proj);
} Projectile;

void update_basic_projectile(Projectile * proj);
Projectile * alloc_projectile();

#endif
