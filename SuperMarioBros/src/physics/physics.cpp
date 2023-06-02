#include "physics.h"
#include <vector>
#include "constants.h"
#include "transform/transform.h"
#include <iostream>

std::vector<rigidbody_t> rigidbodies;

int create_rigidbody(int transform_handle, bool use_gravity, float collider_width, float collider_height) {
	rigidbody_t rigidbody;
	rigidbody.use_gravity = use_gravity;
	rigidbody.transform_handle = transform_handle;
	transform_t& transform = *get_transform(transform_handle);

	aabb_collider_t aabb_collider;
	aabb_collider.x = transform.position.x;
	aabb_collider.y = transform.position.y;
	aabb_collider.width = collider_width;
	aabb_collider.height = collider_height;

	// debug stuff
	aabb_collider.collider_debug_transform_handle = create_transform(glm::vec3(aabb_collider.x, aabb_collider.y, 0.f), glm::vec3(1.5f), 0.f);
	glm::vec3 collider_color(0.f, 1.f, 0.f);
	aabb_collider.collider_debug_render_handle = create_rectangle_render(aabb_collider.collider_debug_transform_handle, collider_color, collider_width, collider_height, true);

	rigidbody.aabb_collider = aabb_collider;

	rigidbodies.push_back(rigidbody);
	return rigidbodies.size() - 1;
}

bool is_colliding(aabb_collider_t& collider_1, aabb_collider_t& collider_2) {
	float col_1_right = collider_1.x + collider_1.width / 2;
	float col_1_left = collider_1.x - collider_1.width / 2;
	float col_1_top = collider_1.y + collider_1.height / 2;
	float col_1_bottom = collider_1.y - collider_1.height / 2;

	float col_2_right = collider_2.x + collider_2.width / 2;
	float col_2_left = collider_2.x - collider_2.width / 2;
	float col_2_top = collider_2.y + collider_2.height / 2;
	float col_2_bottom = collider_2.y - collider_2.height / 2;

	if (col_1_left >= col_2_right || col_2_left >= col_1_right) return false;
	if (col_2_top <= col_1_bottom || col_1_top <= col_2_bottom) return false;
	return true;
}

void update_rigidbodies(float delta_time) {
	for (rigidbody_t& rb : rigidbodies) {
		if (rb.use_gravity) {
			transform_t& transform = *get_transform(rb.transform_handle);
			rb.cur_y_vel -= GRAVITY * delta_time;	
		}
	}

	for (int i = 0; i < rigidbodies.size(); i++) {
		for (int j = i+1; j < rigidbodies.size(); j++) {
			rigidbody_t& rb1 = rigidbodies[i];
			rigidbody_t& rb2 = rigidbodies[j];
			if (is_colliding(rb1.aabb_collider, rb2.aabb_collider)) {
				rb1.cur_y_vel = 0.f;
				rb2.cur_y_vel = 0.f;

				transform_t& transform1 = *get_transform(rb1.transform_handle);
				transform_t& transform2 = *get_transform(rb2.transform_handle);

				// TODO: figure out contact point to see how to change the transform position properly

				if (transform1.position.y > transform2.position.y) {
					transform1.position.y = transform2.position.y + rb2.aabb_collider.height / 2 + rb1.aabb_collider.height / 2;
				}
				else {
					transform2.position.y = transform1.position.y + rb1.aabb_collider.height / 2 + rb1.aabb_collider.height / 2;
				}

				/*
				if (transform1.position.x > transform2.position.x) {
					transform1.position.x = transform2.position.x + rb2.aabb_collider.width / 2 + rb1.aabb_collider.width / 2;
				}
				else {
					transform2.position.x = transform1.position.x + rb1.aabb_collider.width / 2 + rb1.aabb_collider.width / 2;
				}
				*/
			}
		}
	}

	for (rigidbody_t& rb : rigidbodies) {
		transform_t& transform = *get_transform(rb.transform_handle);
		transform.position.y += rb.cur_y_vel * delta_time;
		rb.aabb_collider.x = transform.position.x;
		rb.aabb_collider.y = transform.position.y;

		// debugging collider
		transform_t& collider_debug_transform = *get_transform(rb.aabb_collider.collider_debug_transform_handle);
		collider_debug_transform.position.x = rb.aabb_collider.x;
		collider_debug_transform.position.y = rb.aabb_collider.y;
	}
}
