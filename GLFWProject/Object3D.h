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
#include "PhysicalObject.h"

/*
@author Kimmo Riihiaho, kimmo.riihiaho@gmail.com
@date 14.6.2018

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
	Load default object.
	Create and bind OpenGL buffers for shaders.
	*/
	Object3D();

	/*
	Initialize M, V and P matrices; lights and camera.
	Load object file with given name.
	Create and bind OpenGL buffers for shaders.
	*/
	Object3D(std::string objectName);

	/*
	Initialize graphical shape's drawing.
	*/
	void init();

	/*
	Destroy buffers and shader program.
	*/
	~Object3D();
	
	/*
	Draw the object using M, V and P matrices; camera and light.
	*/
	void draw();

	/*
	Reload graphical shape's shaders from file.
	*/
	void reloadShaders();

	/*
	Load / reload collision bounding box shades.
	*/
	void reloadCbbShaders();

	/*
	Initialize collision shape drawing.
	*/
	void initCbb();
	
	/*
	Draw collision bounding box using default shaders named cbb.
	*/
	void drawCollisionBoundingBox();

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
	Call reloadShaders() after all shader names have been set.

	@param name as it is in file system without .vert ending (it will be added).
	*/
	void setVertexShaderName(std::string name) {
		m_vertexShaderName = name;
	}

	/*
	Call reloadShaders() after all shader names have been set.

	@param name as it is in file system without .frag ending (it will be added).
	*/
	void setFragmentShaderName(std::string name) {
		m_fragmentShaderName = name;
	}
	/* Get visibility. */
	bool isVisible() {
		return m_isVisible;
	}
	/* Set visibility. */
	void isVisible(bool isVisible) {
		m_isVisible = isVisible;
	}

	/*
	Setting physical object makes the model part of the PhysicalSimulation.
	Given PhysicalObject's rotation and translation is overridden
	by graphical object's model matrix.
	*/
	void setPhysicalObject(PhysicalObject * po);

	/*
	Removes the physical object and deletes the pointer to it.
	Might be dangerous...
	*/
	void removePhysicalObject();

	/*
	Set the scale for the object. Doesn't change the 
	actual model matrix, but is used when drawn.

	TODO: when computing collisions, the scale has to be 
	passed to the collision shape.
	*/
	void setScale(glm::vec3 scale) {
		m_scale = scale;
	}



private:
	/* Graphical shape's vertex buffer ID. */
	GLuint m_vertexBufferID;
	/* Graphical shape's normal buffer ID. */
	GLuint m_normalBufferID;
	/* Collision box's vertex buffer ID. */
	GLuint m_cbbVertexBufferID;
	/* Collision box's drawing order buffer ID. */
	GLuint m_cbbElementsID;
	/* Graphical shape's MVP ID. */
	GLuint m_mvpMatrixID;
	/* Collision box's MVP ID. (same as for graphical shape)*/
	GLuint m_cbbMvpMatrixID;
	GLuint m_modelMatrixID;
	GLuint m_projectionMatrixID;
	GLuint m_viewMatrixID;
	/* Graphical shape's shader program ID. */
	GLuint m_programID;
	/* Collision bounding box shader program id. */
	GLuint m_cbbProgramID;
	/* Geometry shape's vertex VAO id. It has to be the same
	as in shader's layer. Otherwise will generate access violation
	when drawing. Same for other VAO IDs.*/
	GLuint m_vaoMainVertsID = 0;
	// Geometry shape's normal VAO id.
	GLuint m_vaoMainNormalsID = 2;
	// Collision shape's vertex VAO id
	GLuint m_vaoCbbVertsID = 3;
	
	GLuint m_lightPosId;
	GLuint m_camPosId;
	GLint m_timeID; // not in use right now

	glm::mat4 m_modelMatrix;
	glm::mat4 m_projectionMatrix;
	glm::mat4 m_viewMatrix;
	/* Scale of the geometric shape. */
	glm::vec3 m_scale = glm::vec3(1.0f);

	glm::vec3 m_lightPos;
	glm::vec3 m_camPos;

	glm::vec3 m_collisionBoxColor;
	// Furthest corner of graphical shape. Used to define collision box.
	glm::vec3 m_furthestCorner;
	/* Amount of vertices in graphical shape. */
	int m_verticeCount;
	// Relative path of object files.
	std::string m_objectFilePath = "objects/";
	// Relative path of shader files.
	std::string m_shaderFilePath = "shaders/";
	// Default name of object to be loaded.
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
	// Force light to camera's location.
	bool m_useLightToCam = true;
	// Visibility flag is checked in draw().
	bool m_isVisible = true;
	// This flag is used in draw() to determine wheter to use physicalObject's model matrix controlled by simulation.
	bool m_isPhysical = false;
	// Draw collision shape. Checked in draw().
	bool m_drawCollisionShape = true;
	// Physical object defines model's reaction's to physical simulation. Contains collision shape.
	PhysicalObject * m_physicalObject;

	/*
	Returns the model matrix scaled with m_scale.
	*/
	glm::mat4 getScaledModelMatrix() {
		return glm::scale(m_projectionMatrix, m_scale);
	}
};

#endif