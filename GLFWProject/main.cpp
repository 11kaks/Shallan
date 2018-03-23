#include <iostream>

#include <GL/glew.h>
// glfw3.h also includes OpenGl header.
// Include all platform specific headers before glfw3.h.
#include <GLFW\glfw3.h>

int main(int argc, char** argv) {
	
	if(!glfwInit()) {
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	GLFWwindow* window = glfwCreateWindow(500, 500, "GLFW window", NULL, NULL);

	if(!window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	while(!glfwWindowShouldClose(window)) {
		// Setup view

		float ratio;
		int width, height;

		glfwGetFramebufferSize(window, &width, &height);
		ratio = width / (float)height;
		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT);

		// Drawing
		glBegin(GL_TRIANGLES);
			glVertex3f(0, 0, 0);
			glVertex3f(1, 0, 0);
			glVertex3f(0, 1, 0);
		glEnd();


		// Swap and check events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();

	exit(EXIT_SUCCESS);
}