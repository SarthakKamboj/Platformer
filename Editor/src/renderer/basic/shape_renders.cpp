#include "shape_renders.h"
#include "utils/conversion.h"
#include "../renderer.h"
#include "transform/transform.h"
#include <vector>

opengl_object_data rectangle_render_t::obj_data{};

extern std::vector<rectangle_render_t> rectangles;

int create_rectangle_render(int transform_handle, glm::vec3& color, int tex_handle, float width, float height, bool wireframe, float tex_influence) {
	rectangle_render_t rectangle;
	rectangle.transform_handle = transform_handle;
	rectangle.width = width;
	rectangle.height = height;
	rectangle.color = color;
	rectangle.wireframe_mode = wireframe;
    rectangle.tex_influence = tex_influence;
    // 2D ortho projection represented in pixels and original rec mesh is 1x1, so scale rectangle mesh by rec width and height
	rectangle._internal_transform.scale = glm::vec3(width, height, 1.f);
    rectangle.texture_handle = tex_handle;
	return add_rectangle_to_renderer(rectangle);
}

void remove_rectangle_render(int rec_render_handle) {
    remove_transform(rectangles[rec_render_handle].transform_handle);
    rectangles.erase(rectangles.begin() + rec_render_handle, rectangles.begin() + rec_render_handle + 1);
}

void draw_rectangle_render(const rectangle_render_t& rectangle) {
	transform_t cur_transform = *get_transform(rectangle.transform_handle);
	cur_transform.position += rectangle._internal_transform.position;
	cur_transform.rotation_deg += rectangle._internal_transform.rotation_deg;
	cur_transform.scale *= rectangle._internal_transform.scale;
	glm::mat4 model_matrix = get_model_matrix(cur_transform);
	shader_set_mat4(rectangle_render_t::obj_data.shader, "model", model_matrix);
	shader_set_vec3(rectangle_render_t::obj_data.shader, "color", rectangle.color);
	shader_set_float(rectangle_render_t::obj_data.shader, "tex_influence", rectangle.tex_influence);
    if (rectangle.texture_handle != -1) {
        bind_texture_by_handle(rectangle.texture_handle);
    }
	if (rectangle.wireframe_mode) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	draw_obj(rectangle_render_t::obj_data);
}


void draw_rectangle_render(int rectangle_handle) {
	draw_rectangle_render(rectangles[rectangle_handle]);
}
