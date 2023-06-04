#pragma once

struct animation_t {
	float start = 0.f, end = 0.f;
	float* cur = nullptr;
	float total_time = 0.f, cur_time = 0.f, start_time = 0.f;
	bool running = false;
};

int create_animation(float* value, float start_val, float end_val, float total_time);
void update_animations();
void start_animation(int anim_handle);
