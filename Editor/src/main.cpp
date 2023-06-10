#include "input/input.h"
#include "SDL.h"
#include "glad/glad.h"
#include "app.h"
#include <iostream>
#include "constants.h"
#include "renderer/opengl/object_data.h"
#include "renderer/renderer.h"
#include "utils/time.h"
#include "editorItems/gridline.h"
#include <cmath>
#include "imgui.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_opengl3.h"
#include "renderer/camera.h"
#include "editorItems/worldItem.h"
#include "imgui-filebrowser/imfilebrowser.h"
#include "editorItems/addWorldItemModal.h"

/*
Screen coordinates will always being (0,0) in the bottom left and (SCREEN_WIDTH, SCREEN_HEIGHT) in top right
*/

// TODO: add texture images to select from

key_state_t key_state;

int main(int argc, char** argv) {
	application_t app;
	mouse_state_t mouse_state;

	init(app);

	int num_cols = ceil(2*SCREEN_WIDTH / GRID_SQUARE_WIDTH);
	for (int col = 0; col < num_cols; col++) {
		create_gridline(col * GRID_SQUARE_WIDTH, SCREEN_HEIGHT / 2.f, dir_t::COL);
	}

	int num_rows = ceil(2*SCREEN_HEIGHT / GRID_SQUARE_WIDTH);
	for (int row = 0; row < num_rows; row++) {
		create_gridline(SCREEN_WIDTH, row * GRID_SQUARE_WIDTH, dir_t::ROW);
	}

	glm::vec3 selected_color(0, 1, 1);
	int transform_handle = create_transform(glm::vec3(0.f), glm::vec3(1.f), 0.f);
	transform_t& transform = *get_transform(transform_handle);
	create_rectangle_render(transform_handle, selected_color, GRID_SQUARE_WIDTH, GRID_SQUARE_WIDTH, false);
	camera_t camera = create_camera();
	float x_offset = 0.f;

	// texture_t texture = create_texture("C:\\Sarthak\\projects\\Platformer\\Editor\\resources\\Legacy-Fantasy - High Forest 2.0\\Legacy-Fantasy - High Forest 2.3\\Assets\\Hive.png");
	create_world_item("C:\\Sarthak\\projects\\Platformer\\Editor\\resources\\Legacy-Fantasy - High Forest 2.0\\Legacy-Fantasy - High Forest 2.3\\Assets\\Hive.png", 5, 5);
	create_world_item("C:\\Sarthak\\projects\\Platformer\\Editor\\resources\\Legacy-Fantasy - High Forest 2.0\\Legacy-Fantasy - High Forest 2.3\\Assets\\Hive.png", 5, 5);
	create_world_item("C:\\Sarthak\\projects\\Platformer\\Editor\\resources\\Legacy-Fantasy - High Forest 2.0\\Legacy-Fantasy - High Forest 2.3\\Assets\\Hive.png", 5, 5);

	texture_t tex = create_texture("C:\\Sarthak\\projects\\Platformer\\Editor\\resources\\Legacy-Fantasy - High Forest 2.0\\Legacy-Fantasy - High Forest 2.3\\Character\\Idle\\Idle-Sheet.png");
	ImGui::FileBrowser file_browser;
	file_browser.SetTitle("File Browser");
	file_browser.SetTypeFilters({".png", ".jpg", ".JPG", ".jpeg", ".JPEG"});


	while (app.running) {
		float start = platformer::get_time_since_start_in_sec();
		process_input(mouse_state, key_state, app.window);	
		if (key_state.close_event_pressed) {
			app.running = false;
		}
		update(camera, key_state, x_offset);
		glm::vec2 grid_square;
		grid_square.x = floor((mouse_state.x + x_offset) / GRID_SQUARE_WIDTH);
		grid_square.y = floor(mouse_state.y / GRID_SQUARE_WIDTH);
		transform.position.x = grid_square.x * GRID_SQUARE_WIDTH + GRID_SQUARE_WIDTH/2;
		transform.position.y = grid_square.y * GRID_SQUARE_WIDTH + GRID_SQUARE_WIDTH/2;

		if (key_state.key_up[' ']) {
			std::cout << "here" << std::endl;
		}

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();

		ImGui::ShowDemoWindow();

		if (ImGui::BeginMainMenuBar()) {
			if (ImGui::BeginMenu("File")) {
				if (ImGui::MenuItem("Add World Item")) {
					file_browser.Open();
				}
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}

		file_browser.Display();

		if (file_browser.HasSelected())
		{
			std::cout << "Selected filename" << file_browser.GetSelected().string() << std::endl;
			std::string path = file_browser.GetSelected().string();
			file_browser.ClearSelected();
			open_add_world_modal(path);
		}

		/*
		if (ImGui::BeginPopupModal("Add World Item", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
			if (ImGui::Button("Cancel")) {
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
		*/

		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

        ImGui::PopStyleVar(2);
		ImGuiIO& io = ImGui::GetIO();

		bool open = true;
		ImGui::Begin("DockSpace Demo", &open, window_flags);
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
			ImGuiID dockspace_id = ImGui::GetID("DockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}
		else
		{
			std::cout << "imgui docking not enabled" << std::endl;
		}
		ImGui::End();

		// ImGui::Image((void*)texture.id, ImVec2(texture.width, texture.height));
		/*
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("Edit"))
			{
				if (ImGui::MenuItem("Add world item", "CTRL+A")) {

				}
				ImGui::Separator();
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}
		*/

		render(app, camera, tex);
		float end = platformer::get_time_since_start_in_sec();
		platformer::time_t::delta_time = end - start;
	}
	return -1;
}