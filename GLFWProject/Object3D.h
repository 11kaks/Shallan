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

/*
Encapsulates a 3D model with vertices and normals. 
Uses shader program to draw itself. Needs light 
and camera positions for correct shading.

Simple setters and getters are defined in this header file, wheras 
more complex loading and drawing methods are in cpp.
*/
class Object3D
{
public:

	/*
	Initialize M, V and P matrices; lights and camera. 
	Create and bind OpenGL buffers for shaders.
	*/
	Object3D();

	/*
	Destroy buffers and shader program.
	*/
	~Object3D();

	/*
	Get ID of the shader program.
	*/
	GLuint getProgramId() {
		return m_programID;
	}

	/*
	Draw the object using M, V and P matrices; camera and light.
	*/
	void draw();

	/*
	Reload shaders from file.
	*/
	void reloadShaders();

	/*
	Set view-to-clip space conversion matrix.
	*/
	void setProjectionMatrix(const glm::mat4 newProjectionMatrix) {
		m_projectionMatrix = newProjectionMatrix;
	}

	/*
	Set world-to-view conversion matrix.
	*/
	void setViewMatrix(const glm::mat4 newViewMatrix) {
		m_viewMatrix = newViewMatrix;
	}

	/*
	Set model-to-world conversion matrix.
	*/
	void setModelMatrix(const glm::mat4 newModelMatrix) {
		m_modelMatrix = newModelMatrix;
	}

	/*
	Set camera position in world space.
	*/
	void setCameraPosition(glm::vec3 newPosition) {
		m_camPos = newPosition;
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
	GLuint m_vertexBufferID;
	GLuint m_normalBufferID;
	GLuint m_mvpMatrixID;
	GLuint m_modelMatrixID;
	GLuint m_projectionMatrixID;
	GLuint m_viewMatrixID;
	GLuint m_programID;
	GLuint m_lightPosId;
	GLuint m_camPosId;
	GLint m_timeID; // not in use right now

	glm::mat4 m_modelMatrix;
	glm::mat4 m_projectionMatrix;
	glm::mat4 m_viewMatrix;
	glm::vec3 m_lightPos;
	glm::vec3 m_camPos;

	int m_verticeCount = 1;
	// Relative path of object files.
	std::string m_objectFilePath = "objects/";
	// Relative path of shader files.
	std::string m_shaderFilePath = "shaders/";
	// Default name of object to be loaded.
	const std::string m_defaultObjectName = "cube";
	//const std::string m_defaultObjectName = "rottinkituoli";
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
	// Force light to camera's location.
	bool m_useLightToCam = true;
};

#endif