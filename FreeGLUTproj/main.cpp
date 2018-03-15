

#include <freeglut.h>

void display() {

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
	glClear(GL_COLOR_BUFFER_BIT);         // Clear the color buffer

	glutWireTeapot(1.0f);

	glFlush();  // Render now
}

int main(int argc, char* argv[]) {

	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutCreateWindow("OpenGL Setup Test"); // Create a window with the given title
	glutInitWindowSize(320, 320);   // Set the window's initial width & height
	glutInitWindowPosition(50, 50); // Position the window's initial top-left corner
	glutDisplayFunc(display); // Register display callback handler for window re-paint
	glutMainLoop();           // Enter the infinitely event-processing loop

	return 0;
}