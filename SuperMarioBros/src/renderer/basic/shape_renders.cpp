#include "shape_renders.h"
#include "utils/conversion.h"
#include "../renderer.h"
#include "transform/transform.h"
#include <vector>

opengl_object_data rectangle_render_t::obj_data{};

static std::vector<rectangle_render_t> rectangles;

int create_rectangle_render(int transform_handle, glm::vec3& color, float width, float height, bool wireframe) {
    static int running_count = 0;
	rectangle_render_t rectangle;
	rectangle.transform_handle = transform_handle;
	rectangle.width = width;
	rectangle.height = height;
	rectangle.color = color;
	rectangle.wireframe_mode = wireframe;
	glm::vec2 rec_screen_size = conversion::ndc_size_to_window_size(glm::vec2(1, 1));
	rectangle._internal_transform.scale = glm::vec3(width, height, 1.f);
    rectangle.handle = running_count;
    rectangles.push_back(rectangle);
    running_count++;
	return rectangle.handle;
}

void draw_rectangle_renders() {
    for (const rectangle_render_t& rectangle : rectangles) {
		draw_rectangle_render(rectangle);
	}
}

void draw_rectangle_render(const rectangle_render_t& rectangle) {
	transform_t cur_transform = *get_transform(rectangle.transform_handle);
	cur_transform.position += rectangle._internal_transform.position;
	cur_transform.rotation_deg += rectangle._internal_transform.rotation_deg;
	cur_transform.scale *= rectangle._internal_transform.scale;
	glm::mat4 model_matrix = get_model_matrix(cur_transform);
	shader_set_mat4(rectangle_render_t::obj_data.shader, "model", model_matrix);
	shader_set_vec3(rectangle_render_t::obj_data.shader, "rec_color", rectangle.color);
	if (rectangle.wireframe_mode) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	draw_obj(rectangle_render_t::obj_data);
}
