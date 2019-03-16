#include "Intersector.h"

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
Vector2 intersector_nearest_segment_point(Vector2 start, Vector2 end, Vector2 point, Vector2 nearest) {
	
}
