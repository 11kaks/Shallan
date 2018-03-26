#include <iostream>

#include <glm/gtc/matrix_transform.hpp> // translate, rotate..


#include <GL/glew.h>

// glfw3.h also includes OpenGl header.
// Include all platform specific headers before glfw3.h.
#include <GLFW/glfw3.h>


#include "shader.hpp"

using namespace std;

void errorCallback(int error, const char* description) {
	cerr << "[ERROR] - " << description << endl;
}

static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
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

			cout << "Kaikkii kivoi extensioneita löäytyy" << endl;

		} else {
			cout << "Jotain extensioneita puuttuu" << endl;
		}

	}
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
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(640, 480, "GLFW window", NULL, NULL);

	if(!window) {
		// Failed to create a window.
		glfwTerminate();
		throw(1);
	}


	// Register key callback
	glfwSetKeyCallback(window, keyCallback);

	// Set OpenGL context
	glfwMakeContextCurrent(window);
	return window;
}

void initDisplay() {
	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
}

GLuint initVertexBuffer() {


	// An array of 3 vectors which represents 3 vertices
	static const GLfloat g_vertex_buffer_data[] = {
		-0.6f, -0.4f, 0.f,
		0.6f, -0.4f, 0.f,
		0.f, 0.6f, 0.f,
	};

	// This will identify our vertex buffer
	GLuint vertexbuffer;
	// Generate 1 buffer, put the resulting identifier in vertexbuffer
	glGenBuffers(1, &vertexbuffer);
	// The following commands will talk about our 'vertexbuffer' buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	// Give our vertices to OpenGL.
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
	return vertexbuffer;
}

int main(int argc, char** argv) {
	
	GLFWwindow* window = initGLFWWindow();

	initGLEW();

	initDisplay();

	// Need to create a Vertex Array Object and set it as the current one
	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	GLuint vertexbuffer = initVertexBuffer();
	
	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders("shaders/SimpleVertexShader.vertexshader", "shaders/SimpleFragmentShader.fragmentshader");

	while(!glfwWindowShouldClose(window)) {
		// Setup view
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use our shader
		glUseProgram(programID);
		
		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);
		// Draw the triangle !
		glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
		glDisableVertexAttribArray(0);
		
		// Set swap interval other than 0 to prevent tearing
		//glfwSwapInterval(1);

		// Swap and check events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Cleanup

	// Cleanup VBO
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteVertexArrays(1, &VertexArrayID);
	glDeleteProgram(programID);

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}