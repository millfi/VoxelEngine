#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

unsigned width = 960;
unsigned height = 540;
const double ASPECT_RATIO = width / static_cast<double>(height);
double FOV_DEGREE = 90;
const auto Vertical_FOV = glm::radians(FOV_DEGREE);
const auto Horizontal_FOV = glm::atan(glm::tan(Vertical_FOV * 0.5) * ASPECT_RATIO);
double NEAR = 0.2;
double FAR = 2000;
const auto PITCH_MAX = glm::radians(89.);
auto PLAYER_SPEED = 0.005;
auto PLAYER_ROT_SPEED = 0.003;
auto PLAYER_POS = glm::vec3(0, 0, 1);
auto MOUSE_SENSITYVITY = 0.002;