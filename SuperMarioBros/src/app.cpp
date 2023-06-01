#include "app.h" 

void update(key_state_t& key_state, float delta_time, const main_character_t& mc) {
	update_main_character(mc, key_state, delta_time);
}
