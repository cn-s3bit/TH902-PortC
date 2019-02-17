#ifndef PLAYERS_DEF_H
#define PLAYERS_DEF_H
#include "../../GameContents/ResourceManager.h"
#include "../../GameContents/RenderManager.h"
#include "../../GameContents/BasicProjectile.h"
typedef struct PlayerReimuA {
	unsigned long Action;
	short _anim;
	short _animTimer;
	Vector2 Position;
	Vector2 Velocity;
	Renderable Renderer;
	Renderable JudgeDisp;
} PlayerReimuA;

typedef struct PlayerInterface {
	void(*update)(void * thiz);
	void *(*initialize)(Vector2 position);
	void(*handle_event)(void * thiz, SDL_Event ev);
} PlayerInterface;

enum PlayerType {
	PLAYER_REIMU_A = 0,
	PLAYER_TYPE_COUNT = 1
};
extern const PlayerInterface ReimuA;

inline const PlayerInterface get_player_interface(const enum PlayerType t) {
	switch (t)
	{
	case PLAYER_REIMU_A:
		return ReimuA;
	default:
		SDL_SetError("Player Type Invalid: %d", (int)t);
		return ReimuA;
	}
}

#endif
