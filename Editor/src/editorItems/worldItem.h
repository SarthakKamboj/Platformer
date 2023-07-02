#pragma once

#include "renderer/opengl/resources.h"
#include "transform/transform.h"

struct world_item_t {
	// texture_t texture;
	int texture_handle;
	int grid_squares_width = -1;
	int grid_squares_height = -1;

    static int selected_world_item_handle;
};

struct placed_world_item_t {
	int world_item_handle = -1;
	glm::vec2 grid_position;
    int rec_render_handle = -1;
};

int create_world_item(const char* path, int squares_width, int squares_height);
world_item_t* get_world_item(int world_handle);
void update_world_item_catalog();
int place_world_item(int world_item_handle, const transform_t& grid_transform);
placed_world_item_t* get_placed_world_item(int placed_handle);
