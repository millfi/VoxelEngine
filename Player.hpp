#pragma once
#include "common.hpp"
#include "Camera.hpp"

class Player : Camera {
	void keyboard_control(double delta_time, GLFWwindow* window) {
		auto vel = PLAYER_SPEED * delta_time;
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			move_forward(vel);
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			move_left(vel);
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			move_back(vel);
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			move_right(vel);
		}
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
			move_forward(vel);
		}
	}
};