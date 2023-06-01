#pragma once

#include "SDL.h"
#include <map>

struct application_t {
	bool running = true;
	SDL_Window* window = NULL;
};

