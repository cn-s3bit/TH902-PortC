#include "BasicProjectile.h"

#define MAX_PROJ 8192
#define MAX_PROJ_TYPE 256

Deque * free_projectile_ids;
Projectile projectiles[MAX_PROJ];
ProjectileTypeDescriptor projectileTypes[MAX_PROJ_TYPE];

inline void add_type(short type, int x, int y, int w, int h, float originX, float originY, float pdr0, int colorId) {
	projectileTypes[type].Region.x = x;
	projectileTypes[type].Region.y = y;
	projectileTypes[type].Region.w = w;
	projectileTypes[type].Region.h = h;
	projectileTypes[type].Origin.X = originX;
	projectileTypes[type].Origin.Y = originY;
	projectileTypes[type].JudgeRadius = pdr0;
	projectileTypes[type].ColorID = colorId;
}

inline void add_types() {
	add_type(1, 0, 0, 12, 16, 6, 8, 2, 0);
	add_type(2, 20, 0, 12, 16, 6, 8, 2, 1);
}

inline void initialize_globals() {
	if (free_projectile_ids == NULL) {
		add_types();
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

const int bulletTypes[25][8] = {
	{ 1,2,3,4,5,6,7,8 }, // 0
	{ 9,10,11,12,13,14,15,16 }, // 1
	{ 17,18,19,20,21,22,23,24 }, // 2
	{ 25,26,27,28,29,30,31,32 }, // 3
	{ 33,34,35,36,37,38,39,40 }, // 4
	{ 41,42,43,44,45,46,47,48 }, // 5
	{ 73,74,75,76,77,78,79,80 }, // 6
	{ 49,50,51,52,53,54,55,56 }, // 7
	{ 57,58,59,60,61,62,63,64 }, // 8
	{ 65,66,67,68,69,70,71,72 }, // 9
	{ 81,82,83,84,85,86,87,88 }, // 10
	{ 89,90,91,92,93,94,95,96 }, // 11
	{ 136,137,138,139,140,141,142 }, // 12
	{ 97,98,99,100,101,102,103 }, // 13
	{ 104,105,106,107,108,109,110,111 }, // 14
	{ 112,113,114,115,116,117,118,119 }, // 15
	{ 120,121,122,123,124,125,126,127 }, // 16
	{ 128,129,130,131,132,133,134,135 }, // 17
	{ 143,144,145,146,147,148,149,150 }, // 18
	{ 151,152,153,154,155,156,157,158 }, // 19
	{ 159,160,161,162,163,164,165,166 }, // 20

	{ 167,168,169,170,171,172,173 }, // 21

	{ 203,204,205,206,207,208,209,210 }, // 22
	{ 210,211,212,213,214,215,216,217 }, // 23
	{ 221,222,223,224,225,226,227,228 } // 24
};
