#pragma once

#include "glm/glm.hpp"
#include <string>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define RESOURCES_PATH std::string("C:\\Sarthak\\projects\\Platformer\\Editor\\resources")
#define SHADERS_PATH (RESOURCES_PATH + std::string("\\shaders"))
#define GRID_SQUARE_WIDTH 20.f
#define GRIDLINE_COLOR glm::vec3(1.f)