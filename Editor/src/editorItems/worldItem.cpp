#include "worldItem.h"
#include <iostream>
#include <vector>
#include "imgui.h"
#include <string>
#include <fstream>
#include "transform/transform.h"
#include "renderer/basic/shape_renders.h"
#include "constants.h"
#include "input/input.h"
#include <stdexcept>

std::vector<world_item_t> world_items;
std::vector<placed_world_item_t> placed_items;

int world_item_t::selected_world_item_handle = -1;

int create_world_item(const char* path, int squares_width, int squares_height) {
    static int running_count = 0;
	world_item_t world_item;
	world_item.texture_handle = create_texture(path);
	world_item.grid_squares_width = squares_width;
	world_item.grid_squares_height = squares_height;	
    world_item.handle = running_count;
	world_items.push_back(world_item);
    running_count++;
    world_item_t::selected_world_item_handle = world_item.handle;
	return world_item.handle;
}

void write_world_items_to_file() {
    std::ofstream out_file;
	out_file.open("world_items.txt");
    if (out_file.is_open()) {
        for (world_item_t& world_item : world_items) {
            texture_t& tex = *get_texture(world_item.texture_handle);
            out_file << tex.path << WORLD_ITEM_TEXT_FILE_DELIM << std::to_string(world_item.grid_squares_width) << WORLD_ITEM_TEXT_FILE_DELIM << std::to_string(world_item.grid_squares_height) << "\n";
        }
        out_file.close();
    } else {
        throw std::runtime_error("could not open world items file");
    }
}

world_item_t* get_world_item(int world_handle) {
    for (world_item_t& world_item : world_items) {
        if (world_item.handle == world_handle) {
            return &world_item;
        }
    }
	return NULL;
}

extern mouse_state_t mouse_state;

void update_world_item_catalog() {
    const float image_height = 50.f;
	ImGui::Begin("World Items");
	for (int i = 0; i < world_items.size(); i++) {
        // push id because the titles are used as IDs and if within the same window, multiple items have the same id, it can cause issues
        // push id helps solve that by ensuring unique ids
        // https://github.com/ocornut/imgui/issues/74
        ImGui::PushID(i);
		world_item_t& item = world_items[i];
        texture_t* tex_ptr = get_texture(item.texture_handle);
        assert(tex_ptr != NULL);
		texture_t& texture = *tex_ptr;
        float ratio = 50.f / texture.height;
        // ? uv0 = top left uv coordinate and uv1 = bottom right uv coordinate
		ImGui::Image((void*)texture.id, ImVec2(texture.width * ratio, texture.height * ratio), ImVec2(0,1), ImVec2(1,0));
        std::string width_text = "Width: " + std::to_string(item.grid_squares_width);
        ImGui::Text(width_text.c_str());
        std::string height_text = "Height: " + std::to_string(item.grid_squares_height);
        ImGui::Text(height_text.c_str());
		if (world_item_t::selected_world_item_handle != i) {
            bool clicked_on_select = ImGui::Button("Select Item");
            if (clicked_on_select) {
                world_item_t::selected_world_item_handle = i;
            }
        } else {
            ImGui::Button("Currently Selected");
        } 
        ImGui::PopID();
	}
	ImGui::End();
}

int place_world_item(int world_item_handle, const glm::vec2& bottom_left_grid_square_pos) {
    if (world_item_handle < 0) {
        std::cout << "could not place item since nothing was selected" << std::endl;
        return -1;
    }

    for (const placed_world_item_t& placed_item : placed_items) {
        if (placed_item.world_item_handle == world_item_handle && 
           placed_item.bottom_left_grid_square_pos == bottom_left_grid_square_pos ) {
            return -1;
        }
    }

    static int running_count = 0;

	placed_world_item_t placed_world_item;
	placed_world_item.world_item_handle = world_item_handle;
	placed_world_item.bottom_left_grid_square_pos = bottom_left_grid_square_pos;
    placed_world_item.handle = running_count;
    running_count++;

    world_item_t* world_item_ptr = get_world_item(world_item_handle);
    assert(world_item_ptr != NULL);
    world_item_t& world_item = *world_item_ptr;
    glm::vec2 center_grid_square;
    center_grid_square.x = bottom_left_grid_square_pos.x + (world_item.grid_squares_width / 2.f);
    center_grid_square.y = bottom_left_grid_square_pos.y + (world_item.grid_squares_height / 2.f);

    glm::vec3 position(1.f);
    position.x = center_grid_square.x * GRID_SQUARE_WIDTH;
    position.y = center_grid_square.y * GRID_SQUARE_WIDTH;

    int transform_handle = create_transform(position, glm::vec3(1), 0);
	glm::vec3 color(1);
    placed_world_item.rec_render_handle = create_rectangle_render(transform_handle, color, 
        world_item.texture_handle, 
        world_item.grid_squares_width * GRID_SQUARE_WIDTH, 
        world_item.grid_squares_height * GRID_SQUARE_WIDTH, false, 1.0f);
	placed_items.push_back(placed_world_item);
	return placed_world_item.handle;
}

placed_world_item_t* get_placed_world_item(int placed_handle) {
    for (placed_world_item_t& placed_item : placed_items) {
        if (placed_handle == placed_item.handle) {
            return &placed_item;
        }
    }
    return NULL;
}

void remove_placed_world_item(glm::vec2 grid_square_pos) {
    for (int i = 0; i < placed_items.size(); i++) {
        const placed_world_item_t& placed_item = placed_items[i];
        if (placed_item.bottom_left_grid_square_pos.x == grid_square_pos.x &&
            placed_item.bottom_left_grid_square_pos.y == grid_square_pos.y) {
            remove_rectangle_render(placed_item.rec_render_handle);
            placed_items.erase(placed_items.begin() + i, placed_items.begin() + i + 1);
            i--;
        }
    }
}