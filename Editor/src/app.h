#pragma once

#include "SDL.h"
#include <map>
#include "input/input.h"
#include "imgui.h"
#include "renderer/camera.h"

struct application_t {
	bool running = true;
	SDL_Window* window = NULL;
	ImGuiIO* io;
};

void init(application_t& app);
void update(camera_t& camera, key_state_t& key_state, float& x_offset);
