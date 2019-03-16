#ifndef SDL_INTERSECTOR_H
#define SDL_INTERSECTOR_H
#include "../SDLWithPlugins.h"
#include "./SDL_rect.h"
#include "./Vector2.h"
#include "MathEx.h"
SDL_bool intersector_is_point_in_triangle(Vector2 p, Vector2 a, Vector2 b, Vector2 c);
SDL_bool intersector_is_point_in_triangle_scalar(float px, float py, float ax, float ay, float bx, float by, float cx, float cy);
long intersector_point_line_side(Vector2 linePoint1, Vector2 linePoint2, Vector2 point);
long intersector_point_line_side_scalar(float linePoint1X, float linePoint1Y, float linePoint2X, float linePoint2Y, float pointX, float pointY);
float intersector_distance_line_point(float startX, float startY, float endX, float endY, float pointX, float pointY);
/*float intersector_distance_segment_point();*/
Vector2 intersector_nearest_segment_point(Vector2 start, Vector2 end, Vector2 point, Vector2 nearest);
#endif