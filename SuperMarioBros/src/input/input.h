#pragma once

#include "states.h"

namespace input {

	struct key_state_t {
		std::map<unsigned char, bool> key_down;
		std::map<unsigned char, bool> key_up;
		std::map<unsigned char, bool> key_being_pressed;
	};

	struct mouse_state_t {
		float x = 0.f, y = 0.f;
		bool left_mouse_down = false, left_mouse_up = false, left_mouse_being_pressed = false;
		bool right_mouse_down = false, right_mouse_up = false, right_mouse_being_pressed = false;
	};

	void process_input(application_state_t& app_state, mouse_state_t& mouse_state, key_state_t& key_state);
}
