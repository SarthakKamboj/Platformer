#pragma once

#include "renderer/basic/shape_renders.h"
#include "transform/transform.h"
#include "input/input.h"

struct rectangle_t {
	// create transform first
	int transform_handle = -1;
	int rec_render_handle = -1;
	glm::vec3 color;
	glm::vec2 dims;
};

rectangle_t create_rectangle(const glm::vec3& pos, const glm::vec3& scale, float rot, glm::vec3& color, const glm::vec2& dims);
void update_rectangle(const rectangle_t& rectangle, input::key_state_t& key_state, float delta_time);
