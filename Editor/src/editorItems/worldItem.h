#pragma once

#include "renderer/opengl/resources.h"

struct world_item_t {
	texture_t texture;
	int grid_squares_width = -1;
	int grid_squares_height = -1;
};

struct placed_world_item_t {
	int world_item_handle = -1;
	glm::vec2 position;
};

int create_world_item(const char* path, int squares_width, int squares_height);
world_item_t* get_world_item(int world_handle);
void render_world_items();
int place_world_item(int world_item_handle);
placed_world_item_t* get_placed_world_item(int placed_handle);

