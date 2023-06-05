#pragma once

#include "SDL.h"
#include <map>
#include "input/input.h"
#include "imgui.h"

struct application_t {
	bool running = true;
	SDL_Window* window = NULL;
	ImGuiIO* io;
};

void init(application_t& app);
void update(key_state_t& key_state);
