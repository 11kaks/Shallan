#include <iostream>
#include "Controls.hpp"

/*
Controls is not a real class. It's just a place to store 
input handlers. There is probably a better way to do this
but this is good enough for now.

All the methods in this file can be called from main directly.
*/

// Speed factor for all mouse commands. Smaller is slower.
float mouseSpeed = 0.003f;

// Zooming speed
float zoomFac = 3.0;
// Zoom along x-axis of screen is slower than along y axis
// if this is set to < 1.
float zoomXYRatio = 0.3;

bool cursorInsideClientArea = false;
bool mouseMiddleDown = false;
bool shiftDown = false;
bool ctrlDown = false;

// Positions used to calculate dragging.
double dragStartCursorX;
double dragStartCursorY;

// This is ugly and propably plain wrong. 
Scene * scene;

/*
Control class gives commands to the scene. The scene must be set in
main method before any commands can be used.
*/
void setScene(Scene * newScene) {
	scene = newScene;
}

/*
Activate mouse commands only if cursor is inside the client area of the window.
*/
void callbackCursorEnter(GLFWwindow* window, int entered) {
	cursorInsideClientArea = entered;
}

/*
Handle mouse button states.
*/
void callbackMouseButton(GLFWwindow* window, int button, int action, int mods) {
	if(cursorInsideClientArea) {

		if(button == GLFW_MOUSE_BUTTON_MIDDLE) {
			if(action == GLFW_PRESS) {
				mouseMiddleDown = true;
				// Set drag start positions when middle mouse button is pressed.
				glfwGetCursorPos(window, &dragStartCursorX, &dragStartCursorY);
			} else {
				mouseMiddleDown = false;
			}
		}
	}
}

/*
Handle cursor movement.
*/
void callbackCursorPosition(GLFWwindow* window, double xpos, double ypos) {
	if(mouseMiddleDown) {
		mouseMiddleCommands(window, xpos, ypos);
	}
}

/*
Handle keyboard events. Set modifier key states.
*/
void callbackKeyPress(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
	if(key == GLFW_KEY_LEFT_SHIFT) {
		if(action == GLFW_PRESS || action == GLFW_REPEAT) {
			shiftDown = true;
		} else {
			shiftDown = false;
		}
	}
	if(key == GLFW_KEY_LEFT_CONTROL) {
		if(action == GLFW_PRESS || action == GLFW_REPEAT) {
			ctrlDown = true;
		} else {
			ctrlDown = false;
		}
	}
	if(key == GLFW_KEY_R && action == GLFW_PRESS) {
		std::cout << "Reloading shaders..." << std::endl;;
		scene->reloadShaders();
		std::cout << "Reloading shaders done." << std::endl;
	}
	if(key == GLFW_KEY_H && action == GLFW_PRESS) {
		printHelp();
	}
	if(key == GLFW_KEY_1 && action == GLFW_PRESS) {
		scene->toggleVisibility(0);
	}
	if(key == GLFW_KEY_2 && action == GLFW_PRESS) {
		scene->toggleVisibility(1);
	}
}

/*
Perform actions which use mouse middle button. Modified
by shift and ctrl keys.
*/
void mouseMiddleCommands(GLFWwindow* window, double xpos, double ypos) {
	
	float deltaX = (float)dragStartCursorX - (float)xpos;
	float deltaY = (float)dragStartCursorY - (float)ypos;

	// Move camera if left shift is down
	if(shiftDown) {
		scene->getCamera()->move(mouseSpeed * deltaX, mouseSpeed * deltaY);	
	} else if(ctrlDown) {
		scene->getCamera()->zoom(mouseSpeed * deltaX * zoomFac * zoomXYRatio, mouseSpeed * zoomFac * deltaY);
	} else {
		scene->getCamera()->rotate(mouseSpeed * deltaX, mouseSpeed * deltaY);
	}

	// Next poll needs to have the old coordinates as starting point.
	dragStartCursorX = xpos;
	dragStartCursorY = ypos;
}


void printHelp() {
	std::cout << std::endl;
	std::cout << "Help - mouse commands" << std::endl;
	std::cout << "Use mouse middle to rotate the camera." << std::endl;
	std::cout << "Use lshift + mouse middle to move the camera." << std::endl;
	std::cout << "Use ctrl + mouse middle to zoom camera. (Actually moves the camera along viewing axis.)" << std::endl;
	std::cout << std::endl;
	std::cout << "Help - keyboard commands" << std::endl;
	std::cout << "Use R to reload shaders." << std::endl;
	std::cout << "Use H to show this help." << std::endl;
}