#pragma once
#include "common.hpp"
class Camera {
	glm::mat4 viewMat;
	glm::mat4 projMat;
	glm::vec3 pos;
	double yaw;
	double pitch;


	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 forward;

	
public:
	Camera(glm::vec3 pos, glm::float32 yaw, glm::float32 pitch) : pos(pos), yaw(yaw), pitch(pitch), up(glm::vec3(0, 1, 0)), right(glm::vec3(1, 0, 0)), forward(glm::vec3(0, 0, -1)) {}
	void update() {
		update_vectors();
		update_view_matrix();
	}
	void update_view_matrix() {
		viewMat = glm::lookAt(pos, pos + forward, up);
	}
	void update_vectors() {
		forward.x = glm::cos(yaw) * glm::cos(pitch);
		forward.y = glm::sin(pitch);
		forward.z = glm::sin(yaw) * glm::cos(pitch);

		forward = glm::normalize(forward);
		right = glm::cross(forward, glm::vec3(0, 1, 0));
		up = glm::cross(right, forward);
	}
	void rotate_pitch(double delta) {
		pitch = glm::clamp(pitch - delta, -PITCH_MAX, PITCH_MAX);
	}
	void rotate_yaw(double delta) {
		yaw += delta;
	}
	void move_left(glm::float32 velocity) {
		pos -= right * velocity;
	}
	void move_right(glm::float32 velocity) {
		pos += right * velocity;
	}
	void move_up(glm::float32 velocity) {
		pos += up * velocity;
	}
	void move_down(glm::float32 velocity) {
		pos -= up * velocity;
	}
	void move_forward(glm::float32 velocity) {
		pos += forward * velocity;
	}
	void move_back(glm::float32 velocity) {
		pos -= forward * velocity;
	}
};