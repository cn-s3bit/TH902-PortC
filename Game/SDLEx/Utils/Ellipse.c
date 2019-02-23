#include "Ellipse.h"
#include <stdlib.h>

Ellipse * ellipse_create_no_states() {
	Ellipse * thiz = (Ellipse *)calloc(1, sizeof(Ellipse));
	return thiz;
}

Ellipse * ellipse_create_by_ellipse(Ellipse * ellipse) {
	Ellipse *thiz = ellipse_create_no_states();
	thiz->x = ellipse->x;
	thiz->y = ellipse->y;
	thiz->height = ellipse->height;
	thiz->width = ellipse->width;
	return thiz;
}

Ellipse * ellipse_create_by_states(float x, float y, float width, float height) {
	Ellipse * thiz = ellipse_create_no_states();
	thiz->x = x;
	thiz->y = y;
	thiz->height = height;
	thiz->width = width;
	return thiz;
}

Ellipse * ellipse_create_by_position_width_height(Vector2 position, float width, float height) {
	Ellipse * thiz = ellipse_create_no_states();
	thiz->x = position.X;
	thiz->y = position.Y;
	thiz->height = height;
	thiz->width = width;
	return thiz;
}

Ellipse * ellipse_create_by_position_size(Vector2 position, Vector2 size) {
	Ellipse * thiz = ellipse_create_no_states();
	thiz->x = position.X;
	thiz->y = position.Y;
	thiz->width = size.X;
	thiz->height = size.Y;
	return thiz;
}

SDL_bool ellipse_contains_scalar(Ellipse * thiz, float x, float y) {
	x = x - thiz->x;
	y = y - thiz->y;
	return (x * x) / (thiz->width * 0.5f * thiz->width * 0.5f) + (y * y) / (thiz->height * 0.5f * thiz->height * 0.5f) <= 1.0f;
}
SDL_bool ellipse_contains(Ellipse * thiz, Vector2 point) {
	return ellipse_contains_scalar(thiz, point.X, point.Y);
}

void ellipse_set_by_states(Ellipse * thiz, float x, float y, float width, float height) {
	thiz->x = x;
	thiz->y = y;
	thiz->width = width;
	thiz->height = height;
}

void ellipse_set_by_ellipse(Ellipse * thiz, Ellipse * ellipse) {
	thiz->x = ellipse->x;
	thiz->y = ellipse->y;
	thiz->width = ellipse->width;
	thiz->height = ellipse->height;

}

void ellipse_set_by_position_size(Ellipse * thiz, Vector2 position, Vector2 size) {
	thiz->x = position.X;
	thiz->y = position.Y;
	thiz->width = size.X;
	thiz->height = size.Y;
}

Ellipse * ellipse_set_position(Ellipse * thiz, Vector2 position) {
	thiz->x = position.X;
	thiz->y = position.Y;
	return thiz;
}

Ellipse * ellipse_set_position_scalar(Ellipse * thiz, float x, float y) {
	thiz->x = x;
	thiz->y = y;
	return thiz;
}

Ellipse * ellipse_set_size(Ellipse * thiz, float width, float height) {
	thiz->width = width;
	thiz->height = height;
	return thiz;
}

float ellipse_area(Ellipse * thiz) {
	return M_PI * (thiz->width * thiz->height) / 4;
}

float ellipse_circumference(Ellipse * thiz) {
	float a = thiz->width / 2;
	float b = thiz->height / 2;
	if (a * 3 > b || b * 3 > a) {
		/* If one dimension is three times as long as the other... */
		return (float)(M_PI - SDL_sqrtf((3 * a + b) * (a + 3 * b)));
	}
	else {
		/* We can use the simpler approximation, then */
		return (float)(M_PI * 2 * SDL_sqrtf((a * a + b * b) / 2));
	}
}

int ellipse_hash_code(Ellipse * thiz) {
	int prime = 53; /*final*/
	int result = 1;
	return result;
}