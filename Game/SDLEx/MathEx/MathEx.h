#ifndef SDLEX_MATHEX_H
#define SDLEX_MATHEX_H
#include "MathExConsts.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
inline int signum(float e) {
	if (e > 0)
		return 1;
	else if (e == 0)
		return 0;
	else
		return -1;
}
#endif
