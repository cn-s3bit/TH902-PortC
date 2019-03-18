#ifndef SDL_INTERSECTOR_H
#define SDL_INTERSECTOR_H
#include "../SDLWithPlugins.h"
#include "./SDL_rect.h"
#include "./Vector2.h"
#include "MathEx.h"
SDL_bool intersector_is_point_in_triangle(Vector2 p, Vector2 a, Vector2 b, Vector2 c) {
	float pX1 = p.X - a.X;
	float pY1 = p.Y - a.Y;
	SDL_bool side12 = (b.X - a.X) * pY1 - (b.Y - a.Y) * pX1 > 0;
	if ((c.X - a.X) * pY1 - (c.Y - a.Y) * pX1 > 0 == side12) return SDL_FALSE;
	if ((c.X - b.X) * (p.Y - b.Y) - (c.Y - b.Y) * (p.X - b.X) > 0 != side12) return SDL_FALSE;
	return SDL_TRUE;
}

SDL_bool intersector_is_point_in_triangle_scalar(float px, float py, float ax, float ay, float bx, float by, float cx, float cy) {
	float px1 = px - ax;
	float py1 = py - ay;
	SDL_bool side12 = (bx - ax) * py1 - (by - ay) * px1 > 0;
	if ((cx - ax) * py1 - (cy - ay) * px1 > 0 == side12) return SDL_FALSE;
	if ((cx - bx) * (py - by) - (cy - by) * (px - bx) > 0 != side12) return SDL_FALSE;
	return SDL_TRUE;
}

long intersector_point_line_side(Vector2 linePoint1, Vector2 linePoint2, Vector2 point) {
	return signum((linePoint2.X - linePoint1.X) * (point.Y - linePoint1.Y) - (linePoint2.Y - linePoint1.Y) * (point.X - linePoint1.X));
}

long intersector_point_line_side_scalar(float linePoint1X, float linePoint1Y, float linePoint2X, float linePoint2Y, float pointX, float pointY) {
	return signum((linePoint2X - linePoint1X) * (pointY - linePoint1Y) - (linePoint2Y - linePoint1Y) * (pointX - linePoint1X));
}

float intersector_distance_line_point(float startX, float startY, float endX, float endY, float pointX, float pointY) {
	float normalLength = sqrtf((endX - startX) * (endX - startX) + (endY - startY) * (endY - startY));
	return fabsf((pointX - startX) * (endY - startY) - (pointY - startY) * (endX - startX)) / normalLength;
}

Vector2 intersector_nearest_segment_point(Vector2 start, Vector2 end, Vector2 point) {
	float length2 = vector2_dst2(start, end);
	if (length2 == 0) return start;
	float t = ((point.X - start.X) * (end.X - start.X) + (point.Y - start.Y) * (end.Y - start.Y)) / length2;
	if (t < 0) return start;
	if (t > 1) return end;
	return vector2_create(start.X + t * (end.X - start.X), start.Y + t * (end.Y - start.Y));
}

Vector2 intersector_nearest_segment_point_scalar(float startX, float startY, float endX, float endY, float pointX, float pointY) {
	float xDiff = endX - startX;
	float yDiff = endY - startY;
	float length2 = xDiff * xDiff + yDiff * yDiff;
	if (length2 == 0) return vector2_create(startX, startY);
	float t = ((pointX - startX) * (endX - startX) + (pointY - startY) * (endY - startY)) / length2;
	if (t < 0) return vector2_create(startX, startY);
	if (t > 1) return vector2_create(endX, endY);
	return vector2_create(startX + t * (endX - startX), startY + t * (endY - startY));
}

float intersector_distance_segment_point_scalar(float startX, float startY, float endX, float endY, float pointX, float pointY) {
	Vector2 e = intersector_nearest_segment_point_scalar(startX, startY, endX, endY, pointX, pointY);
	return vector2_dst1v(e, pointX, pointY);
}

float intersector_distance_segment_point(Vector2 start, Vector2 end, Vector2 point) {
	Vector2 e = intersector_nearest_segment_point(start, end, point);
	return vector2_dst(e, point);
}

SDL_bool intersect_segment_circle(Vector2 start, Vector2 end, Vector2 center, float squareRadius) {
	Vector2 tmp = vector2_create(end.X - start.X, end.Y - start.Y);
	Vector2 tmp1 = vector2_create(center.X - start.X, center.Y - start.Y);
	Vector2 tmp2, tmp3;
	float l = vector2_len(tmp);
	float u = vector2_dot(tmp1, vector2_unit(tmp));
	if (u <= 0)
		tmp2 = start;
	else if (u >= 1)
		tmp2 = end;
	else
		tmp3 = vector2_scl(tmp, u),
			tmp2 = vector2_create(tmp3.X + start.X, tmp3.Y + start.Y);
	float x = center.X - tmp2.X;
	float y = center.Y - tmp2.Y;
	return x * x + y * y <= squareRadius;
}

float intersect_segment_circle_display(Vector2 start, Vector2 end, Vector2 point, float radius, Vector2 displacement) {
	Vector2 tmp, tmp2;
	float u = (point.X - start.X) * (end.X - start.X) + (point.Y - start.Y) * (end.Y - start.Y);
	float d = vector2_dst(start, end);
	u /= d * d;
	if (u < 0 || u > 1) return INFINITY;
	tmp = vector2_sub(end, start);
	tmp2 = vector2_add(start, vector2_scl(tmp, u));
	d = vector2_dst(tmp2, point);
	if (d < radius) {
		displacement = vector2_unit(vector2_sub(point, tmp2));
		return d;
	}
	else
		return INFINITY;
}

float intersect_ray_ray(Vector2 start1, Vector2 direction1, Vector2 start2, Vector2 direction2) {
	float difx = start2.X - start1.X;
	float dify = start2.Y - start1.Y;
	float d1xd2 = direction1.X * direction2.Y - direction1.Y * direction2.X;
	if (d1xd2 == 0.0f)
		return INFINITY;
	float d2sx = direction2.X / d1xd2;
	float d2sy = direction2.Y / d1xd2;
	return difx * d2sy - dify * d2sx;
}

SDL_bool intersect_lines_scalar(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, Vector2 * intersection) {
	float d = (y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1);
	if (d == 0) return SDL_FALSE;

	float ua = ((x4 - x3) * (y1 - y3) - (y4 - y3) * (x1 - x3)) / d;
	if (!intersection)
		*intersection = vector2_create(x1 + (x2 - x1) * ua, y1 + (y2 - y1) * ua);
	return SDL_TRUE;
}

inline SDL_bool intersect_lines(Vector2 p1, Vector2 p2, Vector2 p3, Vector2 p4, Vector2 * intersection) {
	return intersect_lines_scalar(p1.X, p1.Y, p2.X, p2.Y, p3.X, p3.Y, p4.X, p4.Y, intersection);
}

SDL_bool intersect_rectangle(SDL_Rect rectangle1, SDL_Rect rectangle2, SDL_Rect intersection) {
	if (rectangle1.x < rectangle2.x + rectangle2.w &&
			rectangle1.x + rectangle1.w > rectangle2.x &&
			rectangle1.y < rectangle2.y + rectangle2.h &&
			rectangle1.y + rectangle1.h > rectangle2.y) {
		intersection.x = fmaxf(rectangle1.x, rectangle2.x);
		intersection.w = fminf(rectangle1.x + rectangle1.w, rectangle2.x + rectangle2.w) - intersection.x;
		intersection.y = fmaxf(rectangle1.y, rectangle2.y);
		intersection.h = fminf(rectangle1.y + rectangle1.h, rectangle2.y + rectangle2.h) - intersection.y;
		return SDL_TRUE;
	}
	return SDL_FALSE;
}

SDL_bool intersect_segments_scalar(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4, Vector2 * intersection) {
	float d = (y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1);
	if (d == 0) return SDL_FALSE;
	float yd = y1 - y3;
	float xd = x1 - x3;
	float ua = ((x4 - x3) * yd - (y4 - y3) * xd) / d;
	if (ua < 0 || ua > 1) return SDL_FALSE;
	float ub = ((x2 - x1) * yd - (y2 - y1) * xd) / d;
	if (ub < 0 || ub > 1) return SDL_TRUE;
	if (!intersection)
		*intersection = vector2_create(x1 + (x2 - x1) * ua, y1 + (y2 - y1) * ua);
	return SDL_TRUE;
}

SDL_bool intersect_segments(Vector2 p1, Vector2 p2, Vector2 p3, Vector2 p4, Vector2 * intersection) {
	return intersect_segments_scalar(p1.X, p1.Y, p2.X, p2.Y, p3.X, p3.Y, p4.X, p4.Y, intersection);
}

SDL_bool intersect_segment_rectangle_scalar(float startX, float startY, float endX, float endY, SDL_Rect rectangle) {
	float rectangleEndX = rectangle.x + rectangle.w;
	float rectangleEndY = rectangle.y + rectangle.h;
	if (intersect_segments_scalar(startX, startY, endX, endY, rectangle.x, rectangle.y, rectangle.x, rectangleEndY, NULL) ||
			intersect_segments_scalar(startX, startY, endX, endY, rectangle.x, rectangle.y, rectangleEndX, rectangle.y, NULL) ||
			intersect_segments_scalar(startX, startY, endX, endY, rectangleEndX, rectangle.y, rectangleEndX, rectangleEndY, NULL) ||
			intersect_segments_scalar(startX, startY, endX, endY, rectangle.x, rectangleEndY, rectangleEndX, rectangleEndY, NULL))
		return SDL_TRUE;
	/*contains() in Class Rectangle*/
	return rectangle.x <= startX && rectangle.x + rectangle.w >= startX && rectangle.y < startY && rectangle.y + rectangle.h >= startY;
}

SDL_bool intersect_segment_rectangle(Vector2 start, Vector2 end, SDL_Rect rectangle) {
	return intersect_segment_rectangle_scalar(start.X, start.Y, end.X, end.Y, rectangle);
}
#endif