#include "RenderManager.h"

ArrayList * layers[RENDER_LAYER_MAX];
Deque * layer_empty_ids[RENDER_LAYER_MAX];

void initialize_layer(int layerId) {
	if (layer_empty_ids[layerId] == NULL) {
		layers[layerId] = create_array_list(sizeof(Renderable *), 32u);
		layer_empty_ids[layerId] = create_deque(sizeof(int), 32u);
	}
}

void register_renderable(Renderable * renderable) {
	initialize_layer(renderable->Layer);
	if (get_deque_size(layer_empty_ids[renderable->Layer]) > 0) {
		unsigned int index;
		poll_deque_tail(layer_empty_ids[renderable->Layer], &index);
		assign_array_list_element(layers[renderable->Layer], index, &renderable);
		renderable->_internalId = index;
	}
	else {
		renderable->_internalId = layers[renderable->Layer]->Size;
		append_array_list(layers[renderable->Layer], &renderable);
	}
}

void unregister_renderable(Renderable * renderable) {
	Renderable * null_handler = NULL;
	assign_array_list_element(layers[renderable->Layer], renderable->_internalId, &null_handler);
	push_deque_tail(layer_empty_ids[renderable->Layer], &renderable->_internalId);
}

static int qsort_compare_renderable(const void * a, const void * b) {
	if ((*(Renderable **)a) == NULL)
		return -1;
	if ((*(Renderable **)b) == NULL)
		return 1;
	if ((*(Renderable **)a)->TextureRegion.TextureID != (*(Renderable **)b)->TextureRegion.TextureID)
		return (*(Renderable **)a)->TextureRegion.TextureID - (*(Renderable **)b)->TextureRegion.TextureID;
	return (*(Renderable **)a)->_internalId - (*(Renderable **)b)->_internalId;
}

void sort_layer_for_batching(int layerId) {
	SDL_qsort(layers[layerId]->_data, layers[layerId]->Size, layers[layerId]->ElementSize, qsort_compare_renderable);
	destroy_deque(layer_empty_ids[layerId]);
	layer_empty_ids[layerId] = create_deque(sizeof(int), 32u);
	for (unsigned i = 0; i < layers[layerId]->Size; i++) {
		Renderable * item;
		get_element_from_array_list(layers[layerId], i, &item);
		if (item == NULL) push_deque_tail(layer_empty_ids[layerId], &i);
		else item->_internalId = i;
	}
}

void render_layer(unsigned imageId, int layerId) {
	sort_layer_for_batching(layerId);
	long current_texture_id = -1;
	long count = 0;
	for (unsigned i = 0; i < layers[layerId]->Size; i++) {
		Renderable * item;
		get_element_from_array_list(layers[layerId], i, &item);
		if (item == NULL) continue;
		if (item->TextureRegion.TextureID != current_texture_id) {
			current_texture_id = item->TextureRegion.TextureID;
			bind_texture2d(imageId, current_texture_id);
			count = 0;
		}
		++count;
		if (count >= 128) {
			sdlex_render_flush(imageId);
			count = 0;
		}
		Vector2 origin;
		origin = vector2_create((float)item->TextureRegion.Rect.w, (float)item->TextureRegion.Rect.h);
		origin = vector2_scl(origin, 0.5f);
		sdlex_render_texture_region_ex(imageId, item->Center, origin, item->Rotation, item->Scale, item->Color, item->TextureRegion.Rect);
	}
}

void render_all_layers(unsigned imageId) {
	for (int i = 0; i < RENDER_LAYER_MAX; i++) {
		if (layers[i] != NULL) {
			render_layer(imageId, i);
		}
	}
}
