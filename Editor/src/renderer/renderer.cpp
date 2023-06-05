#include "renderer.h"
#include <vector>
#include "imgui.h"
#include "backends/imgui_impl_opengl3.h"
#include "SDL.h"

// will add spritesheet renderers as well in the future
static std::vector<rectangle_render_t> rectangles;

void render(application_t& app) {
	glClearColor(0.f, 0.f, 0.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	ImGuiIO& io = *app.io;
	if(io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
		SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
	}

	for (const rectangle_render_t& rectangle : rectangles) {
		draw_rectangle_render(rectangle);
	}

	SDL_GL_SwapWindow(app.window);
}

int add_rectangle_to_renderer(const rectangle_render_t& rectangle) {
	rectangles.push_back(rectangle);
	return rectangles.size() - 1;
}