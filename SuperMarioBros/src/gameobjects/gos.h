#pragma once

#include "renderer/basic/shape_renders.h"
#include "transform/transform.h"
#include "input/input.h"

struct main_character_t {
	// create transform first
	int transform_handle = -1;
	// character currently renders a rectangle, will later change to sprite
	int rec_render_handle = -1;
	glm::vec3 color;
	glm::vec2 dims;
};

main_character_t create_main_character(const glm::vec3& pos, const glm::vec3& scale, float rot, glm::vec3& color, const glm::vec2& dims);
void update_main_character(const main_character_t& mc, key_state_t& key_state, float delta_time);
