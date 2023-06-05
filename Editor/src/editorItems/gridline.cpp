#include "gridline.h"
#include <vector>
#include "transform/transform.h"
#include "renderer/basic/shape_renders.h"
#include "constants.h"

std::vector<gridline_t> gridlines;

void create_gridline(float x, float y, dir_t dir) {
	gridline_t gridline;
	glm::vec3 pos(x, y, 0.f);
	gridline.dir = dir;
	gridline.transform_handle = create_transform(pos, glm::vec3(1.f), (dir == dir_t::COL) ? 0.f : 90.f);
	glm::vec3 color = GRIDLINE_COLOR;
	gridline.rec_render_handle = create_rectangle_render(gridline.transform_handle, color, 1.f, (gridline.dir == dir_t::COL) ? SCREEN_HEIGHT : SCREEN_WIDTH, false);
	gridlines.push_back(gridline);
}

void draw_gridlines() {
	
}
