#include "RandomPool.h"

static RandomXS128 * randoms[MAX_POOL_INDEX + 1];
static RandomXS128 * seeder;

RandomXS128 * random_pool_get(int index) {
	if (seeder == NULL)
		seeder = randomxs_create_random_seed();
	if (randoms[index] == NULL)
		randoms[index] = randomxs_create(random_ll_r(seeder, (1LL << 62LL)), random_ll_r(seeder, (1LL << 62LL)));
	return randoms[index];
}
