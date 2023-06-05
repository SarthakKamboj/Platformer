#include "input/input.h"
#include "SDL.h"
#include "glad/glad.h"
#include "app.h"
#include <iostream>
#include "constants.h"
#include "renderer/opengl/object_data.h"
#include "renderer/renderer.h"
#include "utils/time.h"
#include "editorItems/gridline.h"
#include <cmath>

/*
Screen coordinates will always being (0,0) in the bottom left and (SCREEN_WIDTH, SCREEN_HEIGHT) in top right
*/

key_state_t key_state;

int main(int argc, char** argv) {
	application_t app;
	mouse_state_t mouse_state;

	init(app);

	int num_cols = ceil(SCREEN_WIDTH / GRID_SQUARE_WIDTH);
	for (int col = 0; col < num_cols; col++) {
		create_gridline(col * GRID_SQUARE_WIDTH, SCREEN_HEIGHT / 2.f, dir_t::COL);
	}

	for (int row = 0; row < SCREEN_HEIGHT / GRID_SQUARE_WIDTH; row++) {
		create_gridline(SCREEN_WIDTH / 2.f, row * GRID_SQUARE_WIDTH, dir_t::ROW);
	}

	glm::vec3 selected_color(0, 1, 1);
	int transform_handle = create_transform(glm::vec3(0.f), glm::vec3(1.f), 0.f);
	transform_t& transform = *get_transform(transform_handle);
	create_rectangle_render(transform_handle, selected_color, GRID_SQUARE_WIDTH, GRID_SQUARE_WIDTH, false);

	while (app.running) {
		float start = platformer::get_time_since_start_in_sec();
		process_input(mouse_state, key_state, app.window);	
		if (key_state.close_event_pressed) {
			app.running = false;
		}
		update(key_state);
		glm::vec2 grid_square;
		grid_square.x = floor(mouse_state.x / GRID_SQUARE_WIDTH);
		grid_square.y = floor(mouse_state.y / GRID_SQUARE_WIDTH);
		transform.position.x = grid_square.x * GRID_SQUARE_WIDTH + GRID_SQUARE_WIDTH/2;
		transform.position.y = grid_square.y * GRID_SQUARE_WIDTH + GRID_SQUARE_WIDTH/2;
		render(app);
		float end = platformer::get_time_since_start_in_sec();
		platformer::time_t::delta_time = end - start;
	}
	return -1;
}