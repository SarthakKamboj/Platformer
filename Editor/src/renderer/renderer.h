#pragma once

#include "basic/shape_renders.h"
#include "app.h"
#include "camera.h"

void render(application_t& app, camera_t& camera);
int add_rectangle_to_renderer(const rectangle_render_t& rectangle);
