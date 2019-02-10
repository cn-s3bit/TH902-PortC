#ifndef SDLEX_MATHEX_VECTOR4_H
#define SDLEX_MATHEX_VECTOR4_H

typedef struct Vector4 {
	float X, Y, Z, W;
} Vector4;

inline Vector4 vector4_create(float x, float y, float z, float w) {
	Vector4 f;
	f.X = x;
	f.Y = y;
	f.Z = z;
	f.W = w;
	return f;
}

#endif
