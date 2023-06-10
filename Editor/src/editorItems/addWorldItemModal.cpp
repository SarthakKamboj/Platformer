#include "addWorldItemModal.h"
#include "imgui.h"

static std::string img_file_path;

void open_add_world_modal(const std::string& file_path) {
	img_file_path = file_path;
	ImGui::OpenPopup("Add World Item");
}

void render_add_world_item_modal() {
	if (ImGui::BeginPopupModal(ADD_WORLD_ITEM_MODAL_TITLE, NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
		std::string path_info = std::string("Image Path: ") + img_file_path;
		ImGui::Text(path_info.c_str());
		if (ImGui::Button("Cancel")) {
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
}