#ifndef JUDGING_SYSTEM_H
#define JUDGING_SYSTEM_H
#include "../SDLEx/SDLWithPlugins.h"
#include "../SDLEx/MathEx/MathEx.h"
#include "../SDLEx/Utils/Ellipse.h"
#include "../SDLEx/Utils/Deque.h"
#define COLLIDER_ACTION int
#define COLLIDER_CLASSES 8

typedef struct Collider {
    Ellipse AttackArea;  /* Hit Others */
	float HitRadiusPlus;  /* Being Hit */
	float Damage;
	float Defense;  /* Multiply with Damage Taken */
	short Class;
	short CollideMask;
	unsigned _internalId;
	COLLIDER_ACTION(*HitEvent)(struct Collider *, SDL_bool);  /* Another Collider; Is Hit */
} Collider;

void register_judge(Collider * collider);
void unregister_judge(Collider * collider);
void judge_hits();
#endif
