#include "worldItem.h"
#include <vector>
#include "imgui.h"
#include <string>
#include "editorItems/addWorldItemModal.h"

std::vector<world_item_t> world_items;
std::vector<placed_world_item_t> placed_items;

int create_world_item(const char* path, int squares_width, int squares_height) {
	world_item_t world_item;
	world_item.texture = create_texture(path);
	world_item.grid_squares_width = squares_width;
	world_item.grid_squares_height = squares_height;
	world_items.push_back(world_item);
	return world_items.size() - 1;
}

world_item_t* get_world_item(int world_handle) {
	return &world_items[world_handle];
}

void render_world_items() {
	render_add_world_item_modal();
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
