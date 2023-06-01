#include "app.h" 

void update(key_state_t& key_state, float delta_time, const rectangle_t& rectangle) {
	update_rectangle(rectangle, key_state, delta_time);
}
