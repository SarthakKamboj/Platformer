#include "worldItem.h"
#include <vector>
#include "imgui.h"
#include <string>
#include <fstream>

// TODO: placing them properly with an adjustable scene editor (MORE IMPORTANCE)
// TODO: work on loading in world items

std::vector<world_item_t> world_items;
std::vector<placed_world_item_t> placed_items;

int create_world_item(const char* path, int squares_width, int squares_height) {
	world_item_t world_item;
	world_item.texture = create_texture(path);
	world_item.grid_squares_width = squares_width;
	world_item.grid_squares_height = squares_height;
	world_items.push_back(world_item);
	std::ofstream out_file;
	out_file.open("world_items.txt", std::ios_base::app);
	out_file << path << " " << std::to_string(squares_width) << " " << std::to_string(squares_height) << "\n";
	return world_items.size() - 1;
}

world_item_t* get_world_item(int world_handle) {
	return &world_items[world_handle];
}

void render_world_items() {
	float divider = 5.f;
	ImGui::Begin("World Items");
	for (int i = 0; i < world_items.size(); i++) {
		world_item_t& item = world_items[i];
		texture_t& texture = item.texture;
		ImGui::Image((void*)texture.id, ImVec2(texture.width/divider, texture.height/divider));
		ImGui::Button(std::to_string((int)item.texture.id).c_str());
	}
	ImGui::End();
}

int place_world_item(int world_item_handle) {
	placed_world_item_t placed_world_item;
	placed_world_item.world_item_handle = world_item_handle;
	placed_world_item.position = glm::vec2(0.f);
	placed_items.push_back(placed_world_item);
	return placed_items.size() - 1;
}

placed_world_item_t* get_placed_world_item(int placed_handle) {
	return &placed_items[placed_handle];
}
