#ifndef OBJECT3D_H
#define OBJECT3D_H

#include <iostream>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> // translate, rotate..

#include <GL/glew.h>

// glfw3.h also includes OpenGl header.
// Include all platform specific headers before glfw3.h.
#include <GLFW/glfw3.h>

#include "shader.hpp"
#include "objloader.hpp"

class Object3D
{
public:
	Object3D();
	Object3D(glm::mat4 viewMatrix, glm::mat4 projectionMatrix);

	~Object3D();

	GLuint getProgramId() {
		return m_programID;
	}

	void draw();

private:
	// This will identify our vertex buffer
	GLuint m_vertexbuffer;
	GLuint m_MatrixID;
	GLuint m_programID;
	glm::mat4 m_modelMatrix;
	glm::mat4 m_projectionMatrix;
	glm::mat4 m_viewMatrix;
	int m_verticeCount;
};

#endif