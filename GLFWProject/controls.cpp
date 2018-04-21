
//using namespace glm;

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
glm::vec3 position = glm::vec3(4, 3, 3);
// Initial horizontal angle : toward -Z
float horizontalAngle = 3.14f;
// Initial vertical angle : none
float verticalAngle = 0.0f;
// Initial Field of View
float initialFoV = 45.0f;

float speed = 3.0f; // 3 units / second
float mouseSpeed = 0.001f;

bool cursorInsideClientArea = false;

bool leftMouseButtonDown = false;

double dragStartCursorX;
double dragStartCursorY;

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

		if(button == GLFW_MOUSE_BUTTON_LEFT) {
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

	// Compute new orientation
	horizontalAngle += mouseSpeed * float(dragStartCursorX - xpos);
	verticalAngle += mouseSpeed * float(dragStartCursorY - ypos);

	// Next poll needs to have the old coordinates as starting point.
	dragStartCursorX = xpos;
	dragStartCursorY = ypos;

	// Direction : Spherical coordinates to Cartesian coordinates conversion
	glm::vec3 direction(
		cos(verticalAngle) * sin(horizontalAngle),
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
	);

	// Right vector
	glm::vec3 right = glm::vec3(
		sin(horizontalAngle - 3.14f / 2.0f),
		0,
		cos(horizontalAngle - 3.14f / 2.0f)
	);

	// Up vector
	glm::vec3 up = glm::cross(right, direction);

	// Move forward
	//if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
	//	position += direction * deltaTime * speed;
	//}
	//// Move backward
	//if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
	//	position -= direction * deltaTime * speed;
	//}
	//// Strafe right
	//if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
	//	position += right * deltaTime * speed;
	//}
	//// Strafe left
	//if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
	//	position -= right * deltaTime * speed;
	//}

	// Camera matrix
	//glm::mat4 viewMatrix = glm::lookAt(
	//	glm::vec3(4, 3, 3), // Camera is at (4,3,3), in World Space
	//	glm::vec3(0, 0, 0), // and looks at the origin
	//	glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	//);


	float FoV = initialFoV;// - 5 * glfwGetMouseWheel(); // Now GLFW 3 requires setting up a callback for this. It's a bit too complicated for this beginner's tutorial, so it's disabled instead.

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	ProjectionMatrix = glm::perspective(glm::radians(FoV), 4.0f / 3.0f, 0.1f, 100.0f);
	// Camera matrix
	ViewMatrix = glm::lookAt(
		position,           // Camera is here
		position + direction, // and looks here : at the same position, plus "direction"
		up                  // Head is up (set to 0,-1,0 to look upside-down)
	);

	// For the next frame, the "last time" will be "now"
	//lastTime = currentTime;

}