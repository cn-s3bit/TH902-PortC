#include "RandomXS128.h"
#include <stdlib.h>
#include <time.h>

long long murmurHash3(long long x);

RandomXS128 * randomxs_create_no_seed() {
	RandomXS128 * thiz = (RandomXS128 *)calloc(1, sizeof(RandomXS128));
	return thiz;
}

RandomXS128 * randomxs_create_random_seed() {
	RandomXS128 * thiz = randomxs_create_no_seed();
	randomxs_set_seed(thiz, (long long)time(NULL));
	return thiz;
}

RandomXS128 * randomxs_create_a_seed(long long seed) { /*Anthor seed is random*/
	RandomXS128 * thiz = randomxs_create_no_seed();
	randomxs_set_seed(thiz, seed);
	return thiz;
}

RandomXS128 * randomxs_create(long long seed0, long long seed1) {
	RandomXS128 * thiz = randomxs_create_no_seed();
	randomxs_set_state(thiz, seed0, seed1);
	return thiz;
}

long long randomxs_next_long(RandomXS128 * thiz) {
	long long s1 = thiz->seed0;
	long long s0 = thiz->seed1; /*s0 -> final*/
	thiz->seed0 = s0;
	s1 ^= s1 << 23;
	return (thiz->seed1 = (s1 ^ s0 ^ ((unsigned long long)s1 >> 17) ^ ((unsigned long long)s0 >> 26))) + s0;
}

long long randomxs_next_long_limited(RandomXS128 * thiz, long long n) {
	/*if (n <= 0) throw new IllegalArgumentException("n must be positive");*/
	for (;;) {
		long long bits = (unsigned long long)randomxs_next_long(thiz) >> 1; /*final*/
		long long value = bits % n; /*final*/
		if (bits - value + (n - 1) >= 0)
			return value;
	}
}

long randomxs_next(RandomXS128 * thiz, long bits) {
	return (long)(randomxs_next_long(thiz) & ((1LL << bits) - 1));
}

long randomxs_next_int(RandomXS128 * thiz) {
	return (long)randomxs_next_long(thiz);
}

long randomxs_next_int_limited(RandomXS128 * thiz, long n) {
	return (long)randomxs_next_long_limited(thiz, n);
}

double randomxs_next_double(RandomXS128 * thiz) {
	return ((unsigned long long)randomxs_next_long(thiz) >> 11) * NORM_DOUBLE;
}

float randomxs_next_float(RandomXS128 * thiz) {
	return (float)(((unsigned long long)randomxs_next_long(thiz) >> 40) * NORM_FLOAT);
}

SDL_bool randomxs_next_boolean(RandomXS128 * thiz) {
	return (randomxs_next_long(thiz) & 1) != 0;
}

void randomxs_next_bytes(RandomXS128 * thiz, jbyte * bytes, size_t size) {
	int n = 0;
	int i = size;
	while (i != 0) {
		n = i < 8 ? i : 8; /*min(i, 8);*/
		for (long long bits = randomxs_next_long(thiz); n-- != 0; bits >>= 8)
			bytes[--i] = (jbyte)bits;
	}
}

void randomxs_set_seed(RandomXS128 * thiz, long long seed) {
	long long seed0 = murmurHash3(seed == 0 ? SDL_MIN_SINT64 : seed);
	randomxs_set_state(thiz, seed0, murmurHash3(seed0));
}

void randomxs_set_state(RandomXS128 * thiz, long long seed0, long long seed1) {
	thiz->seed0 = seed0;
	thiz->seed1 = seed1;
}

long long randomxs_get_state(RandomXS128 * thiz, long seed) {
	return seed == 0 ? thiz->seed0 : thiz->seed1;
}

long long murmurHash3(long long x) {
	x ^= (unsigned long long)x >> 33;
	x *= 0xff51afd7ed558ccdL;
	x ^= x >> 33;
	x *= 0xc4ceb9fe1a85ec53L;
	x ^= x >> 33;
	return (long long)x;
}