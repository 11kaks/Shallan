#ifndef CONTROLS_HPP
#define CONTROLS_HPP

// Include GLFW
#include <GLFW/glfw3.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

void cursor_enter_callback(GLFWwindow* window, int entered);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);

void computeMatricesFromInputs(GLFWwindow* window, double dragStartXpos, double dragStartYpos);
glm::mat4 getViewMatrix();
glm::mat4 getProjectionMatrix();

#endif