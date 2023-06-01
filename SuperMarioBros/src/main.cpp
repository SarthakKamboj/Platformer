#include "input/input.h"
#include "SDL.h"
#include "glad/glad.h"
#include "app.h"
#include <iostream>
#include "constants.h"
#include "renderer/opengl/object_data.h"
#include "renderer/renderer.h"
#include "gameobjects/gos.h"

/*
Screen coordinates will always being (0,0) in the bottom left and (SCREEN_WIDTH, SCREEN_HEIGHT) in top right
*/

int main(int argc, char** argv) {
	application_t app;
	mouse_state_t mouse_state;
	key_state_t key_state;

	init(app);

	glm::vec3 rec_color = glm::vec3(0, 1, 1);
	main_character_t mc = create_main_character(glm::vec3(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 0.f), glm::vec3(1.f), 0.f, rec_color, glm::vec2(50, 100));
	const int NUM_BLOCKS = 15;
	ground_block_t blocks[NUM_BLOCKS];
	for (int i = 0; i < NUM_BLOCKS; i++) {
		blocks[i] = create_ground_block(glm::vec3((ground_block_t::WIDTH / 2) + ground_block_t::WIDTH * i, 20, 0.f), glm::vec3(1.f), 0.f);
	}

	float delta_time = 0.f;

	while (app.running) {
		Uint32 start = SDL_GetTicks();
		process_input(mouse_state, key_state, app.window);	
		if (key_state.close_event_pressed) {
			app.running = false;
		}
		update(key_state, delta_time, mc);
		render(app);
		Uint32 end = SDL_GetTicks();
		delta_time = (end - start) / 1000.f;
	}
	return -1;
}