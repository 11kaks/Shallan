#ifndef OBJECT3D_H
#define OBJECT3D_H

#include <iostream>
#include <vector>
#include <string>

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

	~Object3D();

	GLuint getProgramId() {
		return m_programID;
	}

	void draw();

	void reloadShaders();

	void setProjectionMatrix(const glm::mat4 newProjectionMatrix) {
		m_projectionMatrix = newProjectionMatrix;
	}

	void setViewMatrix(const glm::mat4 newViewMatrix) {
		m_viewMatrix = newViewMatrix;
	}

	void setModelMatrix(const glm::mat4 newModelMatrix) {
		m_modelMatrix = newModelMatrix;
	}

private:
	// This will identify our vertex buffer
	GLuint m_vertexBufferID;
	GLuint m_normalBufferID;
	GLuint m_colorBufferID;
	GLuint m_mvpMatrixID;
	GLuint m_modelMatrixID;
	GLuint m_projectionMatrixID;
	GLuint m_viewMatrixID;
	GLuint m_programID;
	GLuint m_lightPosId;
	GLint m_timeID;
	glm::mat4 m_modelMatrix;
	glm::mat4 m_projectionMatrix;
	glm::mat4 m_viewMatrix;
	glm::vec3 m_lightPos;
	int m_verticeCount;
	// Default location of objects
	std::string m_objectFilePath = "objects/";
	// Default location of shaders
	std::string m_shaderFilePath = "shaders/";
	// Default name of object to be loaded
	const std::string m_defaultObjectName = "cube.obj";
	const std::string m_defaultVertexShaderName = "simpleLight.vert";
	const std::string m_defaultFragmentShaderName = "simpleLight.frag";
};

#endif