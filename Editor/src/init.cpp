#include <string>
#include <stdexcept>
#include <iostream>
#include "glad/glad.h"
#include "constants.h"
#include "app.h"
#include "SDL.h"
#include "renderer/opengl/buffers.h"
#include "renderer/opengl/resources.h"
#include "renderer/opengl/vertex.h"
#include "renderer/basic/shape_renders.h"
#include "imgui.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_opengl3.h"

void init_sdl(application_t& app) {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		const char* sdl_error = SDL_GetError();
		std::string error_msg = "SDL could not be initialized: " + std::string(sdl_error);
		throw std::runtime_error(error_msg);
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	SDL_Window* window = SDL_CreateWindow("window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

	if (window == NULL) {
		const char* sdl_error = SDL_GetError();
		std::string error_msg = "SDL could not be initialized: " + std::string(sdl_error);
		throw std::runtime_error(error_msg);
	}

	SDL_GLContext context = SDL_GL_CreateContext(window);
	gladLoadGLLoader(SDL_GL_GetProcAddress);

	SDL_GL_MakeCurrent(window, context);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows

	ImGui::StyleColorsDark();

	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	const char* glsl_version = "#version 410";
	ImGui_ImplSDL2_InitForOpenGL(window, context);
	ImGui_ImplOpenGL3_Init(glsl_version);

	app.window = window;
	app.io = &io;
}

void init_rectangle_data() {
	opengl_object_data& data = rectangle_render_t::obj_data;

	vertex_t vertices[4];
	vertices[0] = create_vertex(glm::vec3(0.5f, 0.5f, 0.0f), glm::vec3(0,1,1), glm::vec2(0.f)); // top right
	vertices[1] = create_vertex(glm::vec3(0.5f, -0.5f, 0.0f), glm::vec3(0,0,1), glm::vec2(0.f)); // bottom right
	vertices[2] = create_vertex(glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(0,1,0), glm::vec2(0.f)); // bottom left
	vertices[3] = create_vertex(glm::vec3(-0.5f, 0.5f, 0.0f), glm::vec3(1,0,0), glm::vec2(0.f)); // top left

	data.vbo = create_vbo((float*)vertices, sizeof(vertices));

	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3
	};

	data.ebo = create_ebo(indices, sizeof(indices));
	data.vao = create_vao();

	bind_vao(data.vao);
	vao_enable_attribute(data.vao, data.vbo, 0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_t), offsetof(vertex_t, position));
	vao_enable_attribute(data.vao, data.vbo, 1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_t), offsetof(vertex_t, color));
	bind_ebo(data.ebo);
	unbind_vao();
	unbind_ebo();

	data.shader = create_shader((SHADERS_PATH + "\\rectangle.vert").c_str(), (SHADERS_PATH + "\\rectangle.frag").c_str());
	shader_set_mat4(data.shader, "model", glm::mat4(1));
	glm::mat4 projection = glm::ortho(0.0f, (float)SCREEN_WIDTH, 0.0f, (float)SCREEN_HEIGHT);
	shader_set_mat4(data.shader, "projection", projection);
}

void init_fbo_draw_data(application_t& app) {
	opengl_object_data& data = app.fbo_draw_data;

	vertex_t vertices[4];
	float val = 1.f;
	vertices[0] = create_vertex(glm::vec3(val, val, 0.0f), glm::vec3(0,1,1), glm::vec2(1.0f, 1.f)); // top right
	vertices[1] = create_vertex(glm::vec3(val, -val, 0.0f), glm::vec3(0,0,1), glm::vec2(1.0f, 0.f)); // bottom right
	vertices[2] = create_vertex(glm::vec3(-val, -val, 0.0f), glm::vec3(0,1,0), glm::vec2(0.f, 0.f)); // bottom left
	vertices[3] = create_vertex(glm::vec3(-val, val, 0.0f), glm::vec3(1,0,0), glm::vec2(0.f, 1.0f)); // top left

	data.vbo = create_vbo((float*)vertices, sizeof(vertices));

	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3
	};

	data.ebo = create_ebo(indices, sizeof(indices));
	data.vao = create_vao();

	bind_vao(data.vao);
	vao_enable_attribute(data.vao, data.vbo, 0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_t), offsetof(vertex_t, position));
	vao_enable_attribute(data.vao, data.vbo, 1, 2, GL_FLOAT, GL_FALSE, sizeof(vertex_t), offsetof(vertex_t, tex_coord));
	bind_ebo(data.ebo);
	unbind_vao();
	unbind_ebo();

	data.shader = create_shader((SHADERS_PATH + "\\fbo.vert").c_str(), (SHADERS_PATH + "\\fbo.frag").c_str());
	shader_set_mat4(data.shader, "model", glm::mat4(1));
	glm::mat4 projection = glm::ortho(0.0f, (float)SCREEN_WIDTH, 0.0f, (float)SCREEN_HEIGHT);
	shader_set_mat4(data.shader, "projection", projection);
	transform_t transform;
	transform.position.x = SCREEN_WIDTH / 2;
	transform.position.y = SCREEN_HEIGHT / 2;
	glm::mat4 model_matrix = get_model_matrix(transform);
	shader_set_mat4(data.shader, "model", model_matrix);
	shader_set_int(data.shader, "fbo_texture", 0);
}

void init(application_t& app) {
	init_sdl(app);
	app.running = true;
	init_rectangle_data();
	init_fbo_draw_data(app);
	app.world_fbo = create_framebuffer();
}