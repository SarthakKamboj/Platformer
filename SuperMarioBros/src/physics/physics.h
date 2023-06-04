#pragma once

#include "renderer/basic/shape_renders.h"

struct aabb_collider_t {
	float x = -1.f, y = -1.f;
	float width = -1.f;
	float height = -1.f;

	// for debugging
	int collider_debug_render_handle;
	int collider_debug_transform_handle;
};

struct rigidbody_t {
	aabb_collider_t aabb_collider;
	int transform_handle = -1;
	float cur_y_vel = 0.f;
	float cur_x_vel = 0.f;
	bool use_gravity = false;
	bool is_kinematic = false;
	bool debug = false;
};

enum PHYSICS_COLLISION_DIR {
	NONE,
	VERTICAL,
	HORIZONTAL,
};

struct collision_info_t {
	bool is_colliding = false;
	PHYSICS_COLLISION_DIR dir = PHYSICS_COLLISION_DIR::NONE;
};

int create_rigidbody(int transform_handle, bool use_gravity, float collider_width, float collider_height, bool is_kinematic);
void update_rigidbodies(float delta_time);
rigidbody_t* get_rigidbody(int rb_handle);
