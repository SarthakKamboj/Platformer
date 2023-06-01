#include "renderer.h"
#include <vector>


// will add spritesheet renderers as well in the future
static std::vector<rectangle_render_t> rectangles;

void render(application_state_t& app_state) {
	glClearColor(0.f, 0.f, 0.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (const rectangle_render_t& rectangle : rectangles) {
		draw_rectangle_render(rectangle);
	}

	SDL_GL_SwapWindow(app_state.window);
}

int add_rectangle_to_renderer(const rectangle_render_t& rectangle) {
	rectangles.push_back(rectangle);
	return rectangles.size() - 1;
}