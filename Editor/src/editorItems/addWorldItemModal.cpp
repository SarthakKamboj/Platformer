#include "addWorldItemModal.h"
#include "imgui.h"
#include "worldItem.h"

static std::string img_file_path;
static int num_squares_wide = 1;
static int num_squares_high = 1;

void open_add_world_modal(const std::string& file_path) {
	img_file_path = file_path;
	ImGui::OpenPopup("Add World Item");
	num_squares_high = 1;
	num_squares_wide = 1;
}

void update_add_world_item_modal() {
	if (ImGui::BeginPopupModal(ADD_WORLD_ITEM_MODAL_TITLE, NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
		std::string path_info = std::string("Image Path: ") + img_file_path;
		ImGui::Text(path_info.c_str());
		ImGui::SliderInt("Number of Squares wide", &num_squares_wide, 1, 8);
		ImGui::SliderInt("Number of Squares high", &num_squares_high, 1, 8);
		if (ImGui::Button("Add")) {
			create_world_item(img_file_path.c_str(), num_squares_wide, num_squares_high);
			ImGui::CloseCurrentPopup();
		}
		if (ImGui::Button("Cancel")) {
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
}