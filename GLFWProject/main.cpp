#include <iostream>

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // translate, rotate..


#include <GL/glew.h>

// glfw3.h also includes OpenGl header.
// Include all platform specific headers before glfw3.h.
#include <GLFW/glfw3.h>

using namespace std;

void errorCallback(int error, const char* description) {
	cerr << "[ERROR] - " << description << endl;
}

static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}

int main(int argc, char** argv) {

	// Error callback can be registered before init
	glfwSetErrorCallback(errorCallback);

	GLuint vertex_buffer, vertex_shader, fragment_shader, program;
	GLint mvp_location, vpos_location, vcol_location;

	if(!glfwInit()) {
		// Initialization failed
		return 1;
	}

	// Let's require a minimum OpenGL version of 2.0
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	GLFWwindow* window = glfwCreateWindow(640, 480, "GLFW window", NULL, NULL);

	if(!window) {
		// Failed to create a window.
		glfwTerminate();
		return 1;
	}

	// Set OpenGL context
	glfwMakeContextCurrent(window);
	
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

	// Register key callback
	glfwSetKeyCallback(window, keyCallback);
	
	while(!glfwWindowShouldClose(window)) {
		// Setup view

		// Store width and height
		int width, height;

		glfwGetFramebufferSize(window, &width, &height);
		float ratio = width / (float)height;
		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT);

		double time = glfwGetTime();

		glMatrixMode(GL_PROJECTION_MATRIX);
		glLoadIdentity();
		gluPerspective(60, ratio, 0.1, 100);

		glMatrixMode(GL_MODELVIEW_MATRIX);
		glTranslatef(0, 0, -1);

		glBegin(GL_TRIANGLES);
			glColor3f(1.f, 0.f, 0.f);
			glVertex3f(-0.6f, -0.4f, 0.f);
			glColor3f(0.f, 1.f, 0.f);
			glVertex3f(0.6f, -0.4f, 0.f);
			glColor3f(0.f, 0.f, 1.f);
			glVertex3f(0.f, 0.6f, 0.f);
		glEnd();

		// Set swap interval other than 0 to prevent tearing
		glfwSwapInterval(1);

		// Swap and check events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Cleanup
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}