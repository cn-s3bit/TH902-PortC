#ifndef SDLEX_ELLIPSE_H
#define SDLEX_ELLIPSE_H
#include "../MathEx/Vector2.h"
#include "../SDLWithPlugins.h"
typedef struct Ellipse {
	float x, y;
	float width, height;
} Ellipse;
Ellipse * ellipse_create_no_states();
Ellipse * ellipse_create_by_ellipse(Ellipse * ellipse);
Ellipse * ellipse_create_by_states(float x, float y, float width, float height);
Ellipse * ellipse_create_by_position_width_height(Vector2 position, float width, float height);
Ellipse * ellipse_create_by_position_size(Vector2 position, Vector2 size);
SDL_bool ellipse_contains_scalar(Ellipse * thiz ,float x, float y);
SDL_bool ellipse_contains(Ellipse * thiz, Vector2 point);
void ellipse_set_by_states(Ellipse * thiz, float x, float y, float width, float height);
void ellipse_set_by_ellipse(Ellipse * thiz, Ellipse * ellipse);
void ellipse_set_by_position_size(Ellipse * thiz, Vector2 position, Vector2 size);
Ellipse * ellipse_set_position(Ellipse * thiz, Vector2 position);
Ellipse * ellipse_set_position_scalar(Ellipse * thiz, float x, float y);
Ellipse * ellipse_set_size(Ellipse * thiz, float width, float height);
float ellipse_area(Ellipse * thiz);
float ellipse_circumference(Ellipse * thiz);
int ellipse_hash_code(Ellipse * thiz);
#endif
