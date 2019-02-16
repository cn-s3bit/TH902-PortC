#ifndef SDLEX_RANDOM_POOL_H
#define SDLEX_RANDOM_POOL_H
#include "RandomXS128.h"
#define MAX_POOL_INDEX 60

/** Returns a random number between 0 (inclusive) and the specified value (inclusive). */
inline int random_int_r(RandomXS128 * random, int range) {
	return randomxs_next_int_limited(random, range + 1);
}

/** Returns a random number between start (inclusive) and end (inclusive). */
inline int random_int_se(RandomXS128 * random, int start, int end) {
	return start + randomxs_next_int_limited(random, end - start + 1);
}

/** Returns a random number between 0 (inclusive) and the specified value (inclusive). */
inline long long random_ll_r(RandomXS128 * random, long long range) {
	return (long long)(randomxs_next_double(random) * range);
}

/** Returns a random number between start (inclusive) and end (inclusive). */
inline long long random_ll_se(RandomXS128 * random, long long start, long long end) {
	return start + (long long)(randomxs_next_double(random) * (end - start));
}

/** Returns a random boolean value. */
inline SDL_bool random_boolean(RandomXS128 * random) {
	return randomxs_next_boolean(random);
}

/** Returns true if a random integer between 0 and 100 is less than the specified value. */
inline SDL_bool random_boolean_chance(RandomXS128 * random, int chance) {
	return random_int_se(random, 0, 99) < chance;
}

/** Returns random number between 0.0 (inclusive) and 1.0 (exclusive). */
inline float random_float(RandomXS128 * random) {
	return randomxs_next_float(random);
}

/** Returns a random number between 0 (inclusive) and the specified value (exclusive). */
inline float random_float_r(RandomXS128 * random, float range) {
	return randomxs_next_float(random) * range;
}

/** Returns a random number between start (inclusive) and end (exclusive). */
inline float random_float_se(RandomXS128 * random, float start, float end) {
	return start + randomxs_next_float(random) * (end - start);
}

/** Returns -1 or 1, randomly. */
inline int random_sign(RandomXS128 * random) {
	return 1 | (randomxs_next_int(random) >> 31);
}

RandomXS128 * random_pool_get(int index);

#endif
