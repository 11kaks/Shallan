#include <iostream>
#include <vector>
// For fps counter
#include <sstream>

#include <glm/gtc/matrix_transform.hpp> // translate, rotate..

#include <GL/glew.h>

// glfw3.h also includes OpenGl header.
// Include all platform specific headers before glfw3.h.
#include <GLFW/glfw3.h>

#include "shader.hpp"
#include "objloader.hpp"
#include "Object3D.h"
#include "Camera.h"
#include "Controls.hpp"
#include "Scene.h"
#include "FPSCounter.h"

using namespace std;

glm::vec3 cameraPosition = glm::vec3(1.0f, 3.0f, 7.0f);
glm::vec3 cameraDirection = glm::vec3(0.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

// FOV in degrees
float cameraFov = 45.0f;

int m_windowWidth = 1280;
int m_windowHeight = 720;

FPSCounter * fpsCounter = new FPSCounter();

/*
Show fps in window title.
*/
void showFPS(GLFWwindow *pWindow);
GLFWwindow* initGLFWWindow();
void initGLEW();
void initDisplay();
void errorCallback(int error, const char* description);

int main(int argc, char** argv) {

	GLFWwindow* window = initGLFWWindow();
	initGLEW();
	initDisplay();

	// Need to create a Vertex Array Object and set it as the current one
	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	// Smoother object edges.
	//glEnable(GL_MULTISAMPLE);

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
	// Or, for an ortho camera :
	//glm::mat4 Projection = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,0.0f,100.0f); // In world coordinates

	// Camera matrix
	glm::mat4 viewMatrix = glm::lookAt(
		glm::vec3(4, 3, 3), // Camera is at (4,3,3), in World Space
		glm::vec3(0, 0, 0), // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	);

	Camera * camera = new Camera(cameraPosition, 3.14f, -0.4f, cameraUp, cameraFov, m_windowWidth, m_windowHeight);

	Object3D* chair = new Object3D("rottinkituoli");
	chair->setVertexShaderName("weave");
	chair->setFragmentShaderName("weave");
	chair->reloadShaders();
	chair->isVisible(false);

	Object3D* cube = new Object3D();
	cube->setVertexShaderName("weave");
	cube->setFragmentShaderName("weave");
	cube->reloadShaders();

	Light * light = new Light();

	Scene * scene = new Scene();
	scene->addObject(chair);
	scene->addObject(cube);
	scene->setCamera(camera);
	scene->setLight(light);
	setScene(scene);

	printHelp();

	while(!glfwWindowShouldClose(window)) {
		// Setup view
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Set swap interval other than 0 to prevent tearing
		//glfwSwapInterval(1);

		scene->draw();

		// Swap and check events
		glfwSwapBuffers(window);

	//	double currentTime = glfwGetTime();
		fpsCounter->tic(glfwGetTime());
		showFPS(window);
		glfwPollEvents();
	}

	// Cleanup
	delete chair;
	delete cube;
	delete camera;
	delete light;
	glDeleteVertexArrays(1, &VertexArrayID);
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

GLFWwindow* initGLFWWindow() {
	// Error callback can be registered before init
	glfwSetErrorCallback(errorCallback);

	if(!glfwInit()) {
		// Initialization failed
		throw(1);
	}

	// AA x4
	glfwWindowHint(GLFW_SAMPLES, 4);
	// Let's require a minimum OpenGL version of 3.3
	// Geometry shaders available 3.2->
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(m_windowWidth, m_windowHeight, "GLFW window", NULL, NULL);

	if(!window) {
		// Failed to create a window.
		glfwTerminate();
		throw(1);
	}

	// Register key callback
	glfwSetKeyCallback(window, callbackKeyPress);
	glfwSetCursorEnterCallback(window, callbackCursorEnter);
	glfwSetMouseButtonCallback(window, callbackMouseButton);
	glfwSetCursorPosCallback(window, callbackCursorPosition);

	// Set OpenGL context
	glfwMakeContextCurrent(window);
	return window;
}

void initGLEW() {
	// Init GLEW after context is set
	GLenum err = glewInit();

	if(GLEW_OK != err) {
		/* Problem: glewInit failed, something is seriously wrong. */
		//fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		cerr << "[ERROR] - " << glewGetErrorString(err) << endl;
	} else {

		if(!glewGetExtension("GL_ARB_shading_language_100") ||
			!glewGetExtension("GL_ARB_vertex_shader") ||
			!glewGetExtension("GL_ARB_fragment_shader") ||
			!glewGetExtension("GL_ARB_geometry_shader") ||
			!glewGetExtension("GL_ARB_geometry_shader4") ||
			!glewGetExtension("GL_ARB_shader_objects")) {

			// All is good
		} else {
			cout << "Some of required extensions are missing. Cannot proceed with launch." << endl;
		}

	}
}
void initDisplay() {
	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);
}

/*
Show fps in window title. 
*/
void showFPS(GLFWwindow *pWindow) {

	std::ostringstream strs;
	strs << "FPS - " << fpsCounter->getFpsI() << " : Time per frame " << (int)fpsCounter->getTimePerFrameMS() << " ms";
	std::string str = strs.str();
	
	glfwSetWindowTitle(pWindow, str.c_str());
}

void errorCallback(int error, const char* description) {
	cerr << "[ERROR] - " << description << endl;
}