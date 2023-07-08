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

SDL_Window* init_sdl() {
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

	SDL_Window* window = SDL_CreateWindow("window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

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

	return window;
}

void init_rectangle_data() {
	opengl_object_data& data = rectangle_render_t::obj_data;

	vertex_t vertices[4];
	vertices[0] = create_vertex(glm::vec3(0.5f, 0.5f, 0.0f), glm::vec3(0,1,1), glm::vec2(1,1)); // top right
	vertices[1] = create_vertex(glm::vec3(0.5f, -0.5f, 0.0f), glm::vec3(0,0,1), glm::vec2(1,0)); // bottom right
	vertices[2] = create_vertex(glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(0,1,0), glm::vec2(0,0)); // bottom left
	vertices[3] = create_vertex(glm::vec3(-0.5f, 0.5f, 0.0f), glm::vec3(1,0,0), glm::vec2(0,1)); // top left

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
	glm::mat4 projection = glm::ortho(0.0f, (float)WINDOW_WIDTH, 0.0f, (float)WINDOW_HEIGHT);
	shader_set_mat4(data.shader, "projection", projection);
}

// TODO: remove use of world item handles since those may change with the application
void init_placed_world_items() {
    const char* file_path = "C:\\Sarthak\\projects\\Platformer\\Editor\\level1.txt";
    FILE* file;
    fopen_s(&file, file_path, "r");
	size_t delim_len = std::string(WORLD_ITEM_TEXT_FILE_DELIM).size();
    std::map<int, std::string> idx_to_type;
    int i = 0;
    if (file) {
        bool placed_items_section = false;
        char line[1024];
        while (!feof(file)) {
            memset(line, 0, 1024);
            fgets(line, 1024, file);
            if (strcmp(line, "WORLD_ITEMS\n") == 0) continue;
			if (!placed_items_section && (strcmp(line, "\n") == 0)) continue;
            if (!placed_items_section && (strcmp(line, "PLACED_ITEMS\n") != 0)) {
                std::string delim(WORLD_ITEM_TEXT_FILE_DELIM);
                size_t delim_len = delim.size();
                static std::string world_item_format = "%1023s " + delim + " %1023s " + delim + " %i " + delim + " %i\n";
                static const char* world_item_format_char = world_item_format.c_str();

                char name[1024]{};
                char path[1024]{};
                int width = 0;
                int height = 0;
                sscanf_s(line, world_item_format_char, name, 1023, path, 1023, &width, &height);
                std::string name_str(name);

                idx_to_type[i] = name_str;
				i++;
                continue;
            }
            if (strcmp(line, "PLACED_ITEMS\n") == 0) {
                placed_items_section = true;
                continue;
            }
			if ((strcmp(line, "\n") == 0) && placed_items_section) {
				break;
			}

            std::string delim(WORLD_ITEM_TEXT_FILE_DELIM);
            size_t delim_len = delim.size();
            static std::string placed_item_format = "%i " + delim + " %i " + delim + " %i\n";
            static const char* placed_item_format_char = placed_item_format.c_str();

            int idx = -1;
            int x = -1;
            int y = -1;
            sscanf_s(line, placed_item_format_char, &idx, &x, &y);

            std::string& type = idx_to_type[idx];
            if (type == "ground") {
                glm::vec2 grid_pos(x, y);
                glm::vec3 pixel_pos((ground_block_t::WIDTH / 2) + ground_block_t::WIDTH * x, (ground_block_t::HEIGHT / 2) + ground_block_t::HEIGHT * y, 0.f);
                create_ground_block(pixel_pos, glm::vec3(1.f), 0.f);
            }
        }
        fclose(file);
    } else {
        std::cout << "could not open world items file" << std::endl;
    }
}

void init(application_t& app) {
	app.window = init_sdl();
	app.running = true;
    // initialize opengl data for a rectangle
	init_rectangle_data();
    init_placed_world_items();
}