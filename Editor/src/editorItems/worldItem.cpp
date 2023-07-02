#include "worldItem.h"
#include <vector>
#include "imgui.h"
#include <string>
#include <fstream>
#include "transform/transform.h"
#include "renderer/basic/shape_renders.h"
#include "constants.h"

// TODO: placing them properly with an adjustable scene editor (MORE IMPORTANCE)
// TODO: work on loading in world items

std::vector<world_item_t> world_items;
std::vector<placed_world_item_t> placed_items;

int world_item_t::selected_world_item_handle = 0;

int create_world_item(const char* path, int squares_width, int squares_height) {
	world_item_t world_item;
	world_item.texture_handle = create_texture(path);
	world_item.grid_squares_width = squares_width;
	world_item.grid_squares_height = squares_height;
	world_items.push_back(world_item);
	std::ofstream out_file;
	out_file.open("world_items.txt", std::ios_base::app);
	out_file << path << " " << std::to_string(squares_width) << " " << std::to_string(squares_height) << "\n";
    world_item_t::selected_world_item_handle = world_items.size() - 1;
	return world_items.size() - 1;
}

world_item_t* get_world_item(int world_handle) {
	return &world_items[world_handle];
}

void update_world_item_catalog() {
	float divider = 5.f;
	ImGui::Begin("World Items");
	for (int i = 0; i < world_items.size(); i++) {
		world_item_t& item = world_items[i];
		texture_t& texture = get_texture(item.texture_handle);
		ImGui::Image((void*)texture.id, ImVec2(texture.width/divider, texture.height/divider));
		if (ImGui::Button(std::to_string((int)texture.id).c_str())) {
            world_item_t::selected_world_item_handle = i;
        }
	}
	ImGui::End();
}

int place_world_item(int world_item_handle, const transform_t& grid_transform) {
	placed_world_item_t placed_world_item;
	placed_world_item.world_item_handle = world_item_handle;
	placed_world_item.grid_position = glm::vec2(grid_transform.position.x, grid_transform.position.y);
    int transform_handle = create_transform(grid_transform.position, glm::vec3(1), 0);
	glm::vec3 color(1);
    placed_world_item.rec_render_handle = create_rectangle_render(transform_handle, color, world_items[world_item_handle].texture_handle, GRID_SQUARE_WIDTH, GRID_SQUARE_WIDTH, false, 1.0f);
	placed_items.push_back(placed_world_item);
	return placed_items.size() - 1;
}

placed_world_item_t* get_placed_world_item(int placed_handle) {
	return &placed_items[placed_handle];
}