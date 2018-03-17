
#include <iostream>

#include <GL/glew.h>
#include <freeglut.h>

void display() {

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
	glClear(GL_COLOR_BUFFER_BIT);         // Clear the color buffer

	glutWireTeapot(1.0f);

	glFlush();  // Render now
}

int main(int argc, char* argv[]) {

	std::cout << "Starting" << std::endl;

	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutCreateWindow("OpenGL Setup Test"); // Create a window with the given title
	glutInitWindowSize(320, 320);   // Set the window's initial width & height
	glutInitWindowPosition(50, 50); // Position the window's initial top-left corner
	glutDisplayFunc(display); // Register display callback handler for window re-paint

	GLenum err = glewInit();

	if(GLEW_OK != err) {
		/* Problem: glewInit failed, something is seriously wrong. */
		//fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		std::cout << "Glew error " << std::endl;
	} else {
		std::cout << "Glew working " << std::endl;
	}


	glutMainLoop();           // Enter the infinitely event-processing loop

	return 0;
}