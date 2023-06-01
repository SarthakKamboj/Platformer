#include "gos.h"
#include "constants.h" 

main_character_t create_main_character(const glm::vec3& pos, const glm::vec3& scale, float rot, glm::vec3& color, const glm::vec2& dims) {
	main_character_t mc;
	mc.transform_handle = create_transform(pos, scale, rot);
	mc.rec_render_handle = create_rectangle_render(mc.transform_handle, color, dims.x, dims.y);
	return mc;
}

void update_main_character(const main_character_t& mc, key_state_t& key_state, float delta_time) {
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
		transform_t& transform = *get_transform(mc.transform_handle);
		transform.position += glm::vec3(glm::normalize(delta_pos_vec), 0.f);
	}
}
