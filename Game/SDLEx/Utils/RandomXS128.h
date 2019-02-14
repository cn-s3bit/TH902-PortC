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
RandomXS128 * randomxs_create_no_seed();
RandomXS128 * randomxs_create_a_seed(long long seed);
RandomXS128 * randomxs_create(long long seed0, long long seed1);
long long randomxs_next_long();
long long randomxs_next_long_limited(long long n); //@param n the positive bound on the random number to be returned. n -> final
long randomxs_next(long bits); //protected final
long randomxs_next_int();
long randomxs_next_int_limited(long n); //n -> final n�������ҿ��������
double randomxs_next_double();
double randomxs_next_float();
SDL_bool randomxs_next_boolean();
void randomxs_next_bytes(jbyte * bytes);
void randomxs_set_seed(long long seed); //seed -> final
void randomxs_set_state(long seed);
//long long murmurGash3(long long x); //private static
#endif // !SDLEX_RANDOMXS128_H
/*ʵ�ֳ�SDLEx/Utils/RandomXS128.h��SDLEx/Utils/RandomXS128.c ���г�Ա������һ��������RandomXS128 * thiz����������ǰ׺randomxs_ ʵ�ֽṹ���Բο�ArrayList*/