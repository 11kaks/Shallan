
#include <iostream>
#include <glm/vec3.hpp>

#include <GL/glew.h>
#include <freeglut.h>

using namespace std;

const float k_fovy = 90.0f;
const float k_aspect = 1.0f;
const float k_zNear = 1.0f;
const float k_zFar = 1000.0f;

const float k_windowPosX = 100.0f;
const float k_windowPosY = 100.0f;
const float k_windowSizeX = 600.0f;
const float k_windowSizeY = 600.0f;

/*
This function is called whenever a "Normal" key press is received.
*/
void keyboardNormal(unsigned char key, int x, int y) {
	switch(key) {
	case 27: // Escape key
		exit(EXIT_SUCCESS);
		break;
	default:
		cout << "Unhandled key press " << key << "." << endl;
	}

	glutPostRedisplay();
}
/*
Called when resizing the window.
*/
void reshape(int w, int h) {

	cout << "reshape" << endl;

	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if(h == 0)
		h = 1;
	float ratio = 1.0* w / h;

	// Use the Projection Matrix
	glMatrixMode(GL_PROJECTION);

	// Reset Matrix
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set the correct perspective.
	gluPerspective(45, ratio, k_zNear, k_zFar);

	// Get Back to the Modelview
	glMatrixMode(GL_MODELVIEW);
}

void display(void) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// Reset transformations
	glLoadIdentity();

	glTranslatef(0.0f, 0.0f, -5.0f);
	glutSolidCube(1.0f);

	glutSwapBuffers();
}

int main(int argc, char* argv[]) {

	std::cout << "Starting" << std::endl;

	// GLUT stuff

	// init glut
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(k_windowPosX, k_windowPosY);
	glutInitWindowSize(k_windowSizeX, k_windowSizeY);
	glutCreateWindow("Freeglut test");

	// register callbacks
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboardNormal);


	// OPENGL stuff



	// GLEW stuff
	GLenum err = glewInit();

	if(GLEW_OK != err) {
		/* Problem: glewInit failed, something is seriously wrong. */
		//fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		std::cout << "Glew error " << std::endl;
	} else {
		std::cout << "Glew working " << std::endl;
	}

	// Enter the infinitely event-processing loop
	glutMainLoop();

	return EXIT_SUCCESS;
}