
//using namespace glm;

#include <iostream>
#include "controls.hpp"

glm::mat4 ViewMatrix;
glm::mat4 ProjectionMatrix;

glm::mat4 getViewMatrix() {
	return ViewMatrix;
}
glm::mat4 getProjectionMatrix() {
	return ProjectionMatrix;
}


// Initial position : on +Z
glm::vec3 position = glm::vec3(0, 3, 7);
// Initial horizontal angle : toward -Z
float horizontalAngle = 3.14f;
// Initial vertical angle : none
float verticalAngle = -0.4f;
// Initial Field of View
float initialFoV = 45.0f;

//float speed = 3.0f; // 3 units / second
float mouseSpeed = 0.003f;

bool cursorInsideClientArea = false;
bool leftMouseButtonDown = false;
bool shiftDown = false;
bool ctrlDown = false;

double dragStartCursorX;
double dragStartCursorY;

glm::vec3 direction;
glm::vec3 up;
glm::vec3 right;

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
				leftMouseButtonDown = true;
				// Set drag start positions when mous left button is pressed.
				glfwGetCursorPos(window, &dragStartCursorX, &dragStartCursorY);
			} else {
				leftMouseButtonDown = false;
			}
		}
	}
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
	if(leftMouseButtonDown) {
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
}

void computeMatricesFromInputs(GLFWwindow* window, double xpos, double ypos) {


	// glfwGetTime is called only once, the first time this function is called
	//static double lastTime = glfwGetTime();

	// Compute time difference between current and last frame
	//double currentTime = glfwGetTime();
	//float deltaTime = float(currentTime - lastTime);

	// Get mouse position
	//double xpos, ypos;
	//glfwGetCursorPos(window, &xpos, &ypos);

	/*int screenWidth;
	int screenHeight;
	glfwGetWindowSize(window, &screenWidth, &screenHeight);*/

	// Reset mouse position for next frame
	//glfwSetCursorPos(window, screenWidth / 2, screenHeight / 2);


	float deltaX = dragStartCursorX - xpos;
	float deltaY = dragStartCursorY - ypos;

	// Move camera if left shift is down
	if(shiftDown) {

		position += right * mouseSpeed * deltaX;
		position -= up * mouseSpeed * deltaY;

	} else if(ctrlDown) {
		// Zooming control on x-axis is half as fast as along y-axis
		position -= direction * mouseSpeed * 1.5f * deltaX;
		position += direction * mouseSpeed * 3.0f * deltaY;
	} else {
		// Rotate camera

		// Compute new orientation
		horizontalAngle += mouseSpeed * deltaX;
		verticalAngle += mouseSpeed * deltaY;


		// Right vector
		right = glm::vec3(
			sin(horizontalAngle - 3.14f / 2.0f),
			0,
			cos(horizontalAngle - 3.14f / 2.0f)
		); 

		// Direction : Spherical coordinates to Cartesian coordinates conversion
		direction = glm::vec3(
			cos(verticalAngle) * sin(horizontalAngle),
			sin(verticalAngle),
			cos(verticalAngle) * cos(horizontalAngle)
		);

		// Up vector
		up = glm::cross(right, direction);
	}

	// Next poll needs to have the old coordinates as starting point.
	dragStartCursorX = xpos;
	dragStartCursorY = ypos;

	int windowWidth;
	int windowHeight;
	glfwGetWindowSize(window, &windowWidth, &windowHeight);

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	ProjectionMatrix = glm::perspective(glm::radians(initialFoV), float(windowWidth / windowHeight), 0.1f, 100.0f);
	// Camera matrix
	ViewMatrix = glm::lookAt(
		position,           // Camera is here
		position + direction, // and looks here : at the same position, plus "direction"
		up                  // Head is up (set to 0,-1,0 to look upside-down)
	);
}