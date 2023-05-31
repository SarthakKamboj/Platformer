#include "init.h"
#include "input.h"
#include "SDL.h"
#include "glad/glad.h"
#include "states.h"
#include <iostream>
#include "constants.h"

/*
Screen coordinates will always being (0,0) in the bottom left and (SCREEN_WIDTH, SCREEN_HEIGHT) in top right
*/

void update() {}

void render(application_state_t& app_state, mouse_state_t& mouse_state){
	glClearColor(0.f, 0.f, 0.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	SDL_GL_SwapWindow(app_state.window);
}

int main(int argc, char** argv) {
	application_state_t app_state;
	mouse_state_t mouse_state;
	key_state_t key_state;

	init(app_state);

	while (app_state.running) {
		process_input(app_state, mouse_state, key_state);
		update();
		render(app_state, mouse_state);
	}
	return -1;
}