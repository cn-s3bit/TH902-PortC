#include "../SDLWithPlugins.h"
#ifndef SDLEX_RANDOMXS128_H
#define SDLEX_RANDOMXS128_H
#define NORM_DOUBLE (1.0/(1LL << 53))
#define NORM_FLOAT (1.0/(1LL << 24))
typedef unsigned char jbyte;
typedef struct RandomXS128 {
	long long seed0;
	long long seed1;
} RandomXS128;
RandomXS128 * randomxs_create_no_seed(); /*Create a RandomXS128 without seed*/
RandomXS128 * randomxs_create_random_seed(); /*Create a RandomXS128 with two random seeds*/
RandomXS128 * randomxs_create_a_seed(long long seed); /*Create a RandomXS128 with a seed and a random seed*/
RandomXS128 * randomxs_create(long long seed0, long long seed1); /*Create a RandomXS128 with two seeds*/
long long randomxs_next_long(RandomXS128 * thiz);
long long randomxs_next_long_limited(RandomXS128 * thiz, long long n); /*@param n the positive bound on the random number to be returned. n -> final*/
long randomxs_next(RandomXS128 * thiz, long bits); /*protected final*/
long randomxs_next_int(RandomXS128 * thiz);
long randomxs_next_int_limited(RandomXS128 * thiz, long n); /*n -> final*/
double randomxs_next_double(RandomXS128 * thiz);
double randomxs_next_float(RandomXS128 * thiz);
SDL_bool randomxs_next_boolean(RandomXS128 * thiz);
void randomxs_next_bytes(RandomXS128 * thiz, jbyte * bytes, size_t size); /*bytes is an array. size is its length.*/
void randomxs_set_seed(RandomXS128 * thiz, long long seed); /*seed -> final*/
void randomxs_set_state(RandomXS128 * thiz, long long seed0, long long seed1);
long long randomxs_get_state(RandomXS128 * thiz, long seed);
/*long long murmurHash3(long long x);  private static*/
#endif