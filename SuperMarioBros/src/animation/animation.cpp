#include "animation.h"
#include "utils/time.h"
#include <vector>
#include <iostream>
#include <cassert>

std::vector<animation_t> animations;

void update_animations() {
	for (animation_t& animation : animations) {
		if (animation.running) {
			animation.cur_time = platformer::get_time_since_start_in_sec() - animation.start_time;
			animation.running = animation.cur_time <= animation.total_time;
			if (animation.running) {
				*animation.cur = (animation.cur_time / animation.total_time) * (animation.end - animation.start) + animation.start;
			}
		}
	}
}

void start_animation(int anim_handle) {
	animation_t& animation = animations[anim_handle];
	animation.cur_time = 0.f;
	animation.running = true;
	animation.start_time = platformer::get_time_since_start_in_sec();
}

int create_animation(float* value, float start_val, float end_val, float total_time) {
    assert(value != NULL);
	animation_t animation;
	animation.cur = value;
	animation.start = start_val;
	animation.end = end_val;
	animation.running = false;
	animation.total_time = total_time;
	animations.push_back(animation);
	return animations.size() - 1;
}