#pragma once

#include "glm/glm.hpp"

namespace conversion {
	glm::vec2 ndc_coord_to_screen_coord(const glm::vec2& ndc_coord);
	glm::vec2 ndc_size_to_screen_size(const glm::vec2& ndc_size);
	glm::vec2 screen_coord_to_ndc_coord(const glm::vec2& screen_coord);
}
