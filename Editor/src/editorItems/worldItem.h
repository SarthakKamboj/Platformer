#pragma once

#include "renderer/opengl/resources.h"
#include "transform/transform.h"

struct world_item_t {
	int texture_handle;
	int grid_squares_width = -1;
	int grid_squares_height = -1;

    static int selected_world_item_handle;
};

struct placed_world_item_t {
	int world_item_handle = -1;
	glm::vec2 top_left_grid_square_pos;
    int rec_render_handle = -1;
};

int create_world_item(const char* path, int squares_width, int squares_height);
void write_world_item_to_file(world_item_t& world_item);
world_item_t* get_world_item(int world_handle);
void update_world_item_catalog();
// pass in the world item handle and the top left grid square
int place_world_item(int world_item_handle, const glm::vec2& top_left_grid_square);
placed_world_item_t* get_placed_world_item(int placed_handle);
