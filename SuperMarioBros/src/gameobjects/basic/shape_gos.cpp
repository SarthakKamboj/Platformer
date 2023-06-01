#include "shape_gos.h"
#include "constants.h" 

rectangle_t create_rectangle(const glm::vec3& pos, const glm::vec3& scale, float rot, glm::vec3& color, const glm::vec2& dims) {
	rectangle_t rectangle;
	rectangle.transform_handle = create_transform(pos, scale, rot);
	rectangle.rec_render_handle = create_rectangle_render(rectangle.transform_handle, color, dims.x, dims.y);
	return rectangle;
}

void update_rectangle(const rectangle_t& rectangle, key_state_t& key_state, float delta_time) {
	const float vel = SCREEN_WIDTH / 4.f;
	const float delta_pos = vel * delta_time;
	glm::vec2 delta_pos_vec(0.f, 0.f);
	if (key_state.key_being_pressed['w']) {
		delta_pos_vec.y = delta_pos;
	} else if (key_state.key_being_pressed['s']) {
		delta_pos_vec.y = -delta_pos;
	} 
	if (key_state.key_being_pressed['a']) {
		delta_pos_vec.x = -delta_pos;
	} 
	else if (key_state.key_being_pressed['d']) {
		delta_pos_vec.x = delta_pos;
	}

	if (delta_pos_vec != glm::vec2(0.f, 0.f)) {
		transform_t& transform = *get_transform(rectangle.transform_handle);
		transform.position += glm::vec3(glm::normalize(delta_pos_vec), 0.f);
	}
}
