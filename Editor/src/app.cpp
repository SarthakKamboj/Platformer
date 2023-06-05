#include "app.h" 
#include "utils/time.h"
#include "constants.h"

void update(camera_t& camera, key_state_t& key_state, float& x_offset) {
	update_camera(camera, key_state);
	if (key_state.key_being_pressed['d']) {
		x_offset += CAMERA_SCROLL_SPEED * platformer::time_t::delta_time;
	} else if (key_state.key_being_pressed['a']) {
		x_offset -= CAMERA_SCROLL_SPEED * platformer::time_t::delta_time;
	} 
}
