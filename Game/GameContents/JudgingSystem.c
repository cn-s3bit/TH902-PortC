#include "JudgingSystem.h"

#define MAX_COLLIDERS 8102

Collider * colliders[COLLIDER_CLASSES][MAX_COLLIDERS];
Deque * empty_ids[COLLIDER_CLASSES];

void register_judge(Collider * collider) {
	if (empty_ids[collider->Class] == NULL) {
		empty_ids[collider->Class] = create_deque(sizeof(int), 32u);
		for (int i = 0; i < MAX_COLLIDERS; i++) {
			push_deque_tail(empty_ids[collider->Class], &i);
		}
	}
	int index = 0;
	poll_deque_head(empty_ids[collider->Class], &index);
	colliders[collider->Class][index] = collider;
	collider->_internalId = index;
}

void unregister_judge(Collider * collider) {
	colliders[collider->Class][collider->_internalId] = NULL;
	push_deque_tail(empty_ids[collider->Class], &collider->_internalId);
}

void judge_hits() {
	for (int firstLayer = 0; firstLayer < COLLIDER_CLASSES; firstLayer++) {
		for (int firstIndex = 0; firstIndex < MAX_COLLIDERS; firstIndex++) {
			if (colliders[firstLayer][firstIndex] != NULL) {
				short mask = colliders[firstLayer][firstIndex]->CollideMask;
				for (int secondLayer = 0; secondLayer < COLLIDER_CLASSES; secondLayer++) {
					if ((mask & secondLayer) == 0) continue;
					for (int secondIndex = 0; secondIndex < MAX_COLLIDERS; secondIndex++) {
						if (colliders[secondLayer][secondIndex] != NULL) {
							Collider * cdef = colliders[firstLayer][firstIndex];
							Collider * catk = colliders[secondLayer][secondIndex];
							Ellipse ellipse = catk->AttackArea;
							ellipse.width += 2.0f * cdef->HitRadiusPlus;
							ellipse.height += 2.0f * cdef->HitRadiusPlus;
							if (ellipse_contains_scalar(&ellipse, cdef->AttackArea.x, cdef->AttackArea.y)) {
								cdef->HitEvent(catk, SDL_TRUE);
								catk->HitEvent(cdef, SDL_FALSE);
							}
						}
					}
				}
			}
		}
	}
}
