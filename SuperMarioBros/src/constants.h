#pragma once

#include <string>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define RESOURCES_PATH std::string("C:\\Sarthak\\projects\\Platformer\\SuperMarioBros\\resources")
#define SHADERS_PATH (RESOURCES_PATH + std::string("\\shaders"))
#define GRAVITY (9.8f * 100.f)
#define MAX_HORIZONTAL_COL_OFFSET_PIXELS 2