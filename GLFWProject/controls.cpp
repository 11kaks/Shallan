#include <iostream>
#include "controls.hpp"


//float speed = 3.0f; // 3 units / second
float mouseSpeed = 0.003f;

bool cursorInsideClientArea = false;
bool mouseMiddleDown = false;
bool shiftDown = false;
bool ctrlDown = false;

double dragStartCursorX;
double dragStartCursorY;

// This is ugly and propably plain wrong. 
Camera * camera;

void setCamera(Camera *newCamera) {
	camera = newCamera;
}

Object3D * object3d;

void setObject(Object3D *object) {
	object3d = object;
}

/*
Activate mouse commands only if cursor is inside the client area of the window.
*/
void cursor_enter_callback(GLFWwindow* window, int entered) {
	if(entered) {
		cursorInsideClientArea = entered;
	} else {
		cursorInsideClientArea = entered;
		//leftMouseButtonDown = false;
	}
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	if(cursorInsideClientArea) {

		if(button == GLFW_MOUSE_BUTTON_MIDDLE) {
			if(action == GLFW_PRESS) {
				mouseMiddleDown = true;
				// Set drag start positions when mous left button is pressed.
				glfwGetCursorPos(window, &dragStartCursorX, &dragStartCursorY);
			} else {
				mouseMiddleDown = false;
			}
		}
	}
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
	if(mouseMiddleDown) {
		computeMatricesFromInputs(window, xpos, ypos);
	}
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
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
		object3d->reloadShaders();
		std::cout << "Reloading shaders done." << std::endl;
	}
}

void computeMatricesFromInputs(GLFWwindow* window, double xpos, double ypos) {
	
	float deltaX = dragStartCursorX - xpos;
	float deltaY = dragStartCursorY - ypos;

	// Move camera if left shift is down
	if(shiftDown) {
		camera->move(mouseSpeed * deltaX, mouseSpeed * deltaY);	
	} else if(ctrlDown) {
		// Zooming control on x-axis is half as fast as along y-axis
		camera->zoom(mouseSpeed * deltaX * 1.5f , mouseSpeed * 3.0 * deltaY);
	} else {
		camera->rotate(mouseSpeed * deltaX, mouseSpeed * deltaY);
	}

	// Next poll needs to have the old coordinates as starting point.
	dragStartCursorX = xpos;
	dragStartCursorY = ypos;
}