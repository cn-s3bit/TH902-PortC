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
