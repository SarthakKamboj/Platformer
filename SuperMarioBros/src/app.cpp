#include "app.h" 
#include "gameobjects/gos.h"
#include "physics/physics.h"

void update(key_state_t& key_state, float delta_time, const main_character_t& mc) {
	update_rigidbodies(delta_time);

	update_main_character(mc, key_state, delta_time);
}
