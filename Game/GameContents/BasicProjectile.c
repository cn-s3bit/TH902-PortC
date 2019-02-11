#include "BasicProjectile.h"

#define MAX_PROJ 8192

Deque * free_projectile_ids;
Projectile projectiles[MAX_PROJ];

inline void initialize_globals() {
	if (free_projectile_ids == NULL) {
		free_projectile_ids = create_deque(sizeof(int), 128u);
		for (int i = MAX_PROJ - 1; i >= 0; i--) {
			push_deque_tail(free_projectile_ids, &i);
		}
	}
}

void update_basic_projectile(Projectile * proj) {
	if (proj->AI)
		proj->AI(proj);
	proj->Velocity = vector2_add(proj->Velocity, proj->Accel);
	proj->Position = vector2_add(proj->Position, proj->Velocity);
	if (proj->RenderablePt) {
		proj->RenderablePt->Center = proj->Position;
	}
}

Projectile * alloc_projectile() {
	initialize_globals();
	if (get_deque_size(free_projectile_ids) == 0) {
		SDL_Log("Error: Too Many Bullets!");
		return &projectiles[0];
	}
	Projectile * proj;
	int id;
	poll_deque_tail(free_projectile_ids, &id);
	proj = &projectiles[id];
	proj->AI = NULL;
	proj->RenderablePt = NULL;
	proj->Accel = proj->Velocity = proj->Position = vector2_zero();
	proj->Active = 1;
	proj->WhoAmI = id;
	return proj;
}

void free_projectile(Projectile * proj) {
	proj->Active = 0;
	push_deque_tail(free_projectile_ids, &proj->WhoAmI);
}

void update_projectiles() {
	for (int i = 0; i < MAX_PROJ; i++) {
		if (projectiles[i].Active) {
			update_basic_projectile(&projectiles[i]);
		}
	}
}
