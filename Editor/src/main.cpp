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
#include "imgui.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_opengl3.h"
#include "renderer/camera.h"

/*
Screen coordinates will always being (0,0) in the bottom left and (SCREEN_WIDTH, SCREEN_HEIGHT) in top right
*/

// TODO: add imgui

key_state_t key_state;

int main(int argc, char** argv) {
	application_t app;
	mouse_state_t mouse_state;

	init(app);

	int num_cols = ceil(2*SCREEN_WIDTH / GRID_SQUARE_WIDTH);
	for (int col = 0; col < num_cols; col++) {
		create_gridline(col * GRID_SQUARE_WIDTH, SCREEN_HEIGHT / 2.f, dir_t::COL);
	}

	int num_rows = ceil(2*SCREEN_HEIGHT / GRID_SQUARE_WIDTH);
	for (int row = 0; row < num_rows; row++) {
		create_gridline(SCREEN_WIDTH, row * GRID_SQUARE_WIDTH, dir_t::ROW);
	}

	glm::vec3 selected_color(0, 1, 1);
	int transform_handle = create_transform(glm::vec3(0.f), glm::vec3(1.f), 0.f);
	transform_t& transform = *get_transform(transform_handle);
	create_rectangle_render(transform_handle, selected_color, GRID_SQUARE_WIDTH, GRID_SQUARE_WIDTH, false);
	camera_t camera = create_camera();
	float x_offset = 0.f;

	while (app.running) {
		float start = platformer::get_time_since_start_in_sec();
		process_input(mouse_state, key_state, app.window);	
		if (key_state.close_event_pressed) {
			app.running = false;
		}
		update(camera, key_state, x_offset);
		glm::vec2 grid_square;
		grid_square.x = floor((mouse_state.x + x_offset) / GRID_SQUARE_WIDTH);
		grid_square.y = floor(mouse_state.y / GRID_SQUARE_WIDTH);
		transform.position.x = grid_square.x * GRID_SQUARE_WIDTH + GRID_SQUARE_WIDTH/2;
		transform.position.y = grid_square.y * GRID_SQUARE_WIDTH + GRID_SQUARE_WIDTH/2;

		if (key_state.key_up[' ']) {
			std::cout << "here" << std::endl;
		}

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();

		// ImGui::ShowDemoWindow();

		render(app, camera);
		float end = platformer::get_time_since_start_in_sec();
		platformer::time_t::delta_time = end - start;
	}
	return -1;
}