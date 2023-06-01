#include "init.h"
#include "input/input.h"
#include "SDL.h"
#include "glad/glad.h"
#include "app.h"
#include <iostream>
#include "constants.h"
#include "renderer/opengl/object_data.h"
#include "renderer/renderer.h"
#include "gameobjects/basic/shape_gos.h"

/*
Screen coordinates will always being (0,0) in the bottom left and (SCREEN_WIDTH, SCREEN_HEIGHT) in top right
*/
void update(key_state_t& key_state, float delta_time, const rectangle_t& rectangle) {
	update_rectangle(rectangle, key_state, delta_time);
}

int main(int argc, char** argv) {
	application_t app;
	mouse_state_t mouse_state;
	key_state_t key_state;

	init(app);

	// int transform_idx = create_transform(glm::vec3(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 0.f), glm::vec3(1.f), 0.f);
	glm::vec3 rec_color = glm::vec3(0, 1, 1);
	rectangle_t rectangle = create_rectangle(glm::vec3(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 0.f), glm::vec3(1.f), 0.f, rec_color, glm::vec2(50, 100));

	float delta_time = 0.f;

	while (app.running) {
		Uint32 start = SDL_GetTicks();
		process_input(app, mouse_state, key_state);	
		update(key_state, delta_time, rectangle);
		render(app);
		Uint32 end = SDL_GetTicks();
		delta_time = (end - start) / 1000.f;
	}
	return -1;
}