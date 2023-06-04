#include "physics.h"
#include <vector>
#include "constants.h"
#include "transform/transform.h"
#include <iostream>
#include "input/input.h"

std::vector<rigidbody_t> rigidbodies;

int create_rigidbody(int transform_handle, bool use_gravity, float collider_width, float collider_height, bool is_kinematic) {
	rigidbody_t rigidbody;
	rigidbody.use_gravity = use_gravity;
	rigidbody.transform_handle = transform_handle;
	rigidbody.is_kinematic = is_kinematic;
	transform_t& transform = *get_transform(transform_handle);

	aabb_collider_t aabb_collider;
	aabb_collider.x = transform.position.x;
	aabb_collider.y = transform.position.y;
	aabb_collider.width = collider_width;
	aabb_collider.height = collider_height;

	// debug stuff
	aabb_collider.collider_debug_transform_handle = create_transform(glm::vec3(aabb_collider.x, aabb_collider.y, 0.f), glm::vec3(1.f), 0.f);
	glm::vec3 collider_color(0.f, 1.f, 0.f);
	aabb_collider.collider_debug_render_handle = create_rectangle_render(aabb_collider.collider_debug_transform_handle, collider_color, collider_width, collider_height, true);

	rigidbody.aabb_collider = aabb_collider;

	rigidbodies.push_back(rigidbody);
	return rigidbodies.size() - 1;
}

void handle_position(rigidbody_t& kinematic_rb, rigidbody_t& non_kinematic_rb, PHYSICS_COLLISION_DIR col_dir) {
	transform_t& kin_transform = *get_transform(kinematic_rb.transform_handle);
	transform_t& non_kin_transform = *get_transform(non_kinematic_rb.transform_handle);

	const float offset = 0;
	if (col_dir == PHYSICS_COLLISION_DIR::HORIZONTAL) {
		if (kin_transform.position.x > non_kin_transform.position.x) {
			non_kin_transform.position.x = kin_transform.position.x - kinematic_rb.aabb_collider.width/2 - non_kinematic_rb.aabb_collider.width/2 + offset;
		}
		else {
			non_kin_transform.position.x = kin_transform.position.x + kinematic_rb.aabb_collider.width/2 + non_kinematic_rb.aabb_collider.width/2 - offset;
		}
	}
	else if (col_dir == PHYSICS_COLLISION_DIR::VERTICAL) {
		if (kin_transform.position.y > non_kin_transform.position.y) {
			non_kin_transform.position.y = kin_transform.position.y - kinematic_rb.aabb_collider.height/2 - non_kinematic_rb.aabb_collider.height/2 + offset;
		}
		else {
			non_kin_transform.position.y = kin_transform.position.y + kinematic_rb.aabb_collider.height/2 + non_kinematic_rb.aabb_collider.height/2 - offset;
		}
	}
}

extern key_state_t key_state;

void handle_collision(rigidbody_t& rb1, rigidbody_t& rb2) {
	aabb_collider_t& collider_1 = rb1.aabb_collider;
	aabb_collider_t& collider_2 = rb2.aabb_collider;

	collision_info_t col_info;
	float col_1_right = collider_1.x + collider_1.width / 2;
	float col_1_left = collider_1.x - collider_1.width / 2;
	float col_1_top = collider_1.y + collider_1.height / 2;
	float col_1_bottom = collider_1.y - collider_1.height / 2;

	float col_2_right = collider_2.x + collider_2.width / 2;
	float col_2_left = collider_2.x - collider_2.width / 2;
	float col_2_top = collider_2.y + collider_2.height / 2;
	float col_2_bottom = collider_2.y - collider_2.height / 2;

	if (col_1_left > col_2_right || col_2_left > col_1_right) return;
	if (col_2_top < col_1_bottom || col_1_top < col_2_bottom) return;

	// rb1 is on the right and rb2 is on the left
	if (col_1_left == col_2_right) {
		// don't allow rb2 to move right
		if (rb2.cur_x_vel > 0) rb2.cur_x_vel = 0.f;
		// don't allow rb1 to move left
		if (rb1.cur_x_vel < 0) rb1.cur_x_vel = 0.f;
		return;
	}
	// rb2 is on the right and rb1 is on the left
	else if (col_2_left == col_1_right) {
		// don't allow rb2 to move left
		if (rb2.cur_x_vel < 0) rb2.cur_x_vel = 0.f;
		// don't allow rb1 to move right
		if (rb1.cur_x_vel > 0) rb1.cur_x_vel = 0.f;
		return;
	}

	// rb2 on bottom and rb1 on top
	if (col_2_top == col_1_bottom) {
		// don't let rb2 go up
		if (rb2.cur_y_vel > 0) rb2.cur_y_vel = 0.f;
		// don't let rb1 go down
		if (rb1.cur_y_vel < 0) rb1.cur_y_vel = 0.f;
		return;
	}
	// rb1 on bottom and rb2 on top
	else if (col_1_top == col_2_bottom) {
		// don't let rb2 go down
		if (rb2.cur_y_vel < 0) rb2.cur_y_vel = 0.f;
		// don't let rb1 go up
		if (rb1.cur_y_vel > 0) rb1.cur_y_vel = 0.f;
		return;
	}

	col_info.is_colliding = true;
	col_info.dir = VERTICAL;

	float overlap_from_col1_x = col_1_right - col_2_left;
	float overlap_from_col2_x = col_2_right - col_1_left;

	float overlap_from_col1_y = col_1_top - col_2_bottom;
	float overlap_from_col2_y = col_2_top - col_1_bottom;

	bool overlap_on_x = fmin(overlap_from_col1_x, overlap_from_col2_x) < MAX_HORIZONTAL_COL_OFFSET_PIXELS;

	if (overlap_on_x && fmin(overlap_from_col1_y, overlap_from_col2_y) > 3.5f) {
		col_info.dir = HORIZONTAL;
	}

	if (col_info.dir == VERTICAL) {
		rb1.cur_y_vel = 0.f;
		rb2.cur_y_vel = 0.f;
	}
	else {
		rb1.cur_x_vel = 0.f;
		rb2.cur_x_vel = 0.f;
	}

	transform_t& transform1 = *get_transform(rb1.transform_handle);
	transform_t& transform2 = *get_transform(rb2.transform_handle);

	if (rb1.is_kinematic != rb2.is_kinematic) {
		if (rb1.is_kinematic) {
			handle_position(rb1, rb2, col_info.dir);
		}
		else {
			handle_position(rb2, rb1, col_info.dir);
		}
	}
}

void update_rigidbodies(float delta_time) {	

	for (rigidbody_t& rb : rigidbodies) {
		transform_t& transform = *get_transform(rb.transform_handle);
		if (rb.use_gravity) {
			rb.cur_y_vel -= GRAVITY * delta_time;
		}
	}

	for (int i = 0; i < rigidbodies.size(); i++) {
		for (int j = i + 1; j < rigidbodies.size(); j++) {
			rigidbody_t& rb1 = rigidbodies[i];
			rigidbody_t& rb2 = rigidbodies[j];
			handle_collision(rb1, rb2);
		}
	}

	for (rigidbody_t& rb : rigidbodies) {
		transform_t& transform = *get_transform(rb.transform_handle);
		transform.position.y += rb.cur_y_vel * delta_time;
		transform.position.x += rb.cur_x_vel * delta_time;
		rb.aabb_collider.x = transform.position.x;
		rb.aabb_collider.y = transform.position.y;

		// debugging collider
		transform_t& collider_debug_transform = *get_transform(rb.aabb_collider.collider_debug_transform_handle);
		collider_debug_transform.position.x = rb.aabb_collider.x;
		collider_debug_transform.position.y = rb.aabb_collider.y;
	}	
}

rigidbody_t* get_rigidbody(int rb_handle) {
	return &rigidbodies[rb_handle];
}
