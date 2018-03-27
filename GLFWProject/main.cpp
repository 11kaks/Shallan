#include <iostream>
#include <vector>

#include <glm/gtc/matrix_transform.hpp> // translate, rotate..

#include <GL/glew.h>

// glfw3.h also includes OpenGl header.
// Include all platform specific headers before glfw3.h.
#include <GLFW/glfw3.h>

#include "shader.hpp"
#include "objloader.hpp"
#include "Object3D.h"

using namespace std;

const char* objFilePath = "objects/cube.obj";
const char* vertexShaderPath = "shaders/SimpleVertexShader.vertexshader";
const char* fragmentShaderPath = "shaders/SimpleFragmentShader.fragmentshader";

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

			cout << "Kaikkii kivoi extensioneita l��ytyy" << endl;

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
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);
}

GLuint initVertexBuffer() {

	// Read our .obj file
	std::vector< glm::vec3 > vertices;
	std::vector< glm::vec2 > uvs;
	std::vector< glm::vec3 > normals; // Won't be used at the moment.
	bool res = loadOBJ(objFilePath, vertices, uvs, normals);

	// This will identify our vertex buffer
	GLuint vertexbuffer;
	// Generate 1 buffer, put the resulting identifier in vertexbuffer
	glGenBuffers(1, &vertexbuffer);
	// The following commands will talk about our 'vertexbuffer' buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	// Give our vertices to OpenGL.
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
	return vertexbuffer;
}

/*GLuint initColorBuffer() {

	// One color for each vertex. Same as vertex
	static const GLfloat g_color_buffer_data[] = {
		-1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f, 
		1.0f, 1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f,-1.0f, 
		1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f,-1.0f,
		1.0f,-1.0f,-1.0f,
		1.0f, 1.0f,-1.0f,
		1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f,-1.0f,
		1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f,-1.0f, 1.0f,
		1.0f,-1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f,-1.0f,-1.0f,
		1.0f, 1.0f,-1.0f,
		1.0f,-1.0f,-1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f,-1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f,-1.0f,
		-1.0f, 1.0f,-1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f,-1.0f, 1.0f
	};

	GLuint colorbuffer;
	glGenBuffers(1, &colorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);
	return colorbuffer;
}*/

int main(int argc, char** argv) {
	
	GLFWwindow* window = initGLFWWindow();

	initGLEW();

	initDisplay();

	// Need to create a Vertex Array Object and set it as the current one
	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	GLuint vertexbuffer = initVertexBuffer();
	//GLuint colorbuffer = initColorBuffer();
	
	// Create and compile our GLSL program from the shaders
	//GLuint programID = LoadShaders(vertexShaderPath, fragmentShaderPath);

	

	// Projection matrix : 45� Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
	// Or, for an ortho camera :
	//glm::mat4 Projection = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,0.0f,100.0f); // In world coordinates

	// Camera matrix
	glm::mat4 View = glm::lookAt(
		glm::vec3(4, 3, 3), // Camera is at (4,3,3), in World Space
		glm::vec3(0, 0, 0), // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	);
	// Model matrix : an identity matrix (model will be at the origin)
	glm::mat4 Model = glm::mat4(1.0f);

	// Testing some transformations
	/*Model = glm::rotate(Model, 30.0f, glm::vec3(0., 1., 0.));
	Model = glm::translate(Model, glm::vec3(0., 0., 4.));
	Model = glm::scale(Model, glm::vec3(3.));*/

	// Our ModelViewProjection : multiplication of our 3 matrices
	glm::mat4 MVP = Projection * View * Model; // Remember, matrix multiplication is the other way around

	Object3D* cube = new Object3D(View, Projection);

	while(!glfwWindowShouldClose(window)) {
		// Setup view
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use our shader
		//glUseProgram(programID);

		// Send our transformation to the currently bound shader, 
		// in the "MVP" uniform
		//glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
		
		// 1rst attribute buffer : vertices
		/*glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);*/

		// 2nd attribute buffer : colors
		/*glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
		glVertexAttribPointer(
			1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			3,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);*/

		// Draw the triangle !
		//glDrawArrays(GL_TRIANGLES, 0, 3*12); // Starting from vertex 0; 3*12 vertices total -> 6 sided cube
		//glDisableVertexAttribArray(0);
		
		// Set swap interval other than 0 to prevent tearing
		//glfwSwapInterval(1);

		cube->draw();

		// Swap and check events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Cleanup

	delete cube;

	// Cleanup VBO
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteVertexArrays(1, &VertexArrayID);
	//glDeleteProgram(cube->getProgramId());

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}