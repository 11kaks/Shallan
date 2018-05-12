#ifndef CONTROLS_HPP
#define CONTROLS_HPP

// Included in main.
//#include <GLFW/glfw3.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Camera.h"
#include "Object3D.h"
#include "Scene.h"

void callbackCursorEnter(GLFWwindow* window, int entered);
void callbackMouseButton(GLFWwindow* window, int button, int action, int mods);
void callbackCursorPosition(GLFWwindow* window, double xpos, double ypos);
void callbackKeyPress(GLFWwindow* window, int key, int scancode, int action, int mods);
void setScene(Scene * newScene);
void printHelp();

void mouseMiddleCommands(GLFWwindow* window, double dragStartXpos, double dragStartYpos);

#endif