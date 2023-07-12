#include "gos.h"
#include "constants.h" 
#include <iostream>

main_character_t create_main_character(const glm::vec3& pos, const glm::vec3& scale, float rot, glm::vec3& color, const glm::vec2& dims) {
	main_character_t mc;
	mc.transform_handle = create_transform(pos, scale, rot);
	mc.rec_render_handle = create_rectangle_render(mc.transform_handle, color, dims.x, dims.y, false);
	mc.rigidbody_handle = create_rigidbody(mc.transform_handle, true, dims.x, dims.y, false);
	return mc;
}

void update_main_character(const main_character_t& mc, key_state_t& key_state) {
    // get velocity
	const float vel = WINDOW_WIDTH / 4.f;

    // get rigidbody and make sure its valid
    rigidbody_t* rb_ptr = get_rigidbody(mc.rigidbody_handle);
    assert(rb_ptr != NULL);
	rigidbody_t& rb = *rb_ptr;

    // jump
    bool jump_btn_pressed = key_state.key_being_pressed['w'] || key_state.key_being_pressed[' '];
    bool character_falling = rb.vel.y < 0;
	if (jump_btn_pressed && character_falling) {
		rb.vel.y = 2*vel;
	}

    bool left_move_pressed = key_state.key_being_pressed['a'];
    bool right_move_pressed = key_state.key_being_pressed['d'];

	if (left_move_pressed) {
		rb.vel.x = -vel;
	}
	else if (right_move_pressed) {
		rb.vel.x = vel;
	}
	else {
		rb.vel.x = 0;
	}
}

const glm::vec3 ground_block_t::BLOCK_COLOR = glm::vec3(0.58f, 0.29f, 0.f);

ground_block_t create_ground_block(const glm::vec3& pos, const glm::vec3& scale, float rot) {
	ground_block_t block;
	block.transform_handle = create_transform(pos, scale, rot);
	glm::vec3 color = ground_block_t::BLOCK_COLOR;
	block.rec_render_handle = create_rectangle_render(block.transform_handle, color, ground_block_t::WIDTH, ground_block_t::HEIGHT, false);
	block.rigidbody_handle = create_rigidbody(block.transform_handle, false, ground_block_t::WIDTH, ground_block_t::HEIGHT, true);
	return block;
}
