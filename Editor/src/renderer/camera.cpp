#include "camera.h"
#include "glm/glm.hpp"
#include "constants.h"
#include "utils/time.h"

camera_t create_camera() {
	camera_t camera;
	camera.transform_handle = create_transform(glm::vec3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 5.f), glm::vec3(1.f), 0);
	camera.transform = get_transform(camera.transform_handle);
	camera.look_at = glm::vec3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.f);
	return camera;
}

void update_camera(camera_t& camera, key_state_t& key_state) {
	if (key_state.key_being_pressed['d']) {
		camera.transform->position.x += CAMERA_SCROLL_SPEED * platformer::time_t::delta_time;
	} else if (key_state.key_being_pressed['a']) {
		camera.transform->position.x -= CAMERA_SCROLL_SPEED * platformer::time_t::delta_time;
	} 
}

glm::mat4 get_view_matrix(camera_t& camera) {
	glm::mat4 view(1.0f);
	glm::vec3 translate(-camera.transform->position.x + (SCREEN_WIDTH/2), -camera.transform->position.y + (SCREEN_HEIGHT/2), 0.f);
	view = glm::translate(view, translate);
	return view;
}
