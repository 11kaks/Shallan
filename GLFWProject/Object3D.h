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

	/*
	@param name as it is in file system without .obj ending (it will be added).
	*/
	void setObjectName(std::string name) {
		m_objectName = name;
	}
	/*
	@param name as it is in file system without .vert ending (it will be added).
	*/
	void setVertexShaderName(std::string name) {
		m_vertexShaderName = name;
		reloadShaders();
	}

	/*
	@param name as it is in file system without .frag ending (it will be added).
	*/
	void setFragmentShaderName(std::string name) {
		m_fragmentShaderName = name;
		reloadShaders();
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
	// Location of objects
	std::string m_objectFilePath = "objects/";
	// Location of shaders
	std::string m_shaderFilePath = "shaders/";
	// Default name of object to be loaded
	const std::string m_defaultObjectName = "cube";
	const std::string m_defaultVertexShaderName = "simpleLight";
	const std::string m_defaultFragmentShaderName = "simpleLight";
	const std::string m_objectFileEnding = ".obj";
	const std::string m_vertexShaderFileEndig = ".vert";
	const std::string m_fragmentShaderFileEnding = ".frag";
	// Object name to be used in loading from file. Does not contain the file ending.
	std::string m_objectName;
	// Vertex shader name to be used in loading from file. Does not contain the file ending.
	std::string m_vertexShaderName;
	// Fragment shader name to be used in loading from file. Does not contain the .obj file ending.
	std::string m_fragmentShaderName;
};

#endif