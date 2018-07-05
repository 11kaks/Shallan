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
	Create and bind OpenGL buffers for shaders.
	*/
	Object3D();

	Object3D(std::string objectName);

	void init();

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
	Load / reload collision bounding box shades.
	*/
	void reloadCbbShaders();

	void initCbb();

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
	void setPhysicalObject(PhysicalObject * po) {
		m_physicalObject = po;
		po->setModelMatrix(m_modelMatrix);
		initCbb();
		m_isPhysical = true;
	}

	void removePhysicalObject() {
		// FIXME can I do this? What if someone still uses the pointer?
		delete m_physicalObject;
		m_isPhysical = false;
	}

	void drawCollisionBoundingBox();
	/*{
		CollisionShape * cs = m_physicalObject->getCollisionShape();
		std::vector<glm::vec3> corners = cs->getCornerPointList();

		glLineWidth(2.5);
		glColor3f(m_collisionBoxColor.x, m_collisionBoxColor.y, m_collisionBoxColor.z);

		glBegin(GL_LINE_LOOP);
			glVertex3f(corners[0].x, corners[0].y, corners[0].z);
			glVertex3f(corners[1].x, corners[1].y, corners[1].z);
			glVertex3f(corners[2].x, corners[2].y, corners[2].z);
			glVertex3f(corners[3].x, corners[3].y, corners[3].z);
		glEnd();

		glBegin(GL_LINE_LOOP);
			glVertex3f(corners[4].x, corners[4].y, corners[4].z);
			glVertex3f(corners[5].x, corners[5].y, corners[5].z);
			glVertex3f(corners[6].x, corners[6].y, corners[6].z);
			glVertex3f(corners[7].x, corners[7].y, corners[7].z);
		glEnd();

		glBegin(GL_LINE);
			glVertex3f(corners[0].x, corners[0].y, corners[0].z);
			glVertex3f(corners[4].x, corners[4].y, corners[4].z);
		glEnd();

		glBegin(GL_LINE);
			glVertex3f(corners[1].x, corners[1].y, corners[1].z);
			glVertex3f(corners[5].x, corners[5].y, corners[5].z);
		glEnd();

		glBegin(GL_LINE);
			glVertex3f(corners[2].x, corners[2].y, corners[2].z);
			glVertex3f(corners[6].x, corners[6].y, corners[6].z);
		glEnd();

		glBegin(GL_LINE);
			glVertex3f(corners[3].x, corners[3].y, corners[3].z);
			glVertex3f(corners[7].x, corners[7].y, corners[7].z);
		glEnd();
	}*/

private:
	GLuint m_vertexBufferID;
	GLuint m_cbbVertexBufferID;
	GLuint m_cbbElementsID;
	GLuint m_normalBufferID;
	GLuint m_mvpMatrixID;
	GLuint m_cbbMvpMatrixID;
	GLuint m_modelMatrixID;
	GLuint m_projectionMatrixID;
	GLuint m_viewMatrixID;
	GLuint m_programID;
	/* Geometry shape's vertex VAO id. It has to be the same
	as in shader's layer. Otherwise will generate access violation
	when drawing. Same for other VAO IDs.*/
	GLuint m_vaoMainVertsID = 0;
	// Geometry shape's normal VAO id.
	GLuint m_vaoMainNormalsID = 2;
	// Collision shape's vertex VAO id
	GLuint m_vaoCbbVertsID = 3;
	// Collision bounding box shader program id.
	GLuint m_cbbProgramID;
	GLuint m_lightPosId;
	GLuint m_camPosId;
	GLint m_timeID; // not in use right now

	glm::mat4 m_modelMatrix;
	glm::mat4 m_projectionMatrix;
	glm::mat4 m_viewMatrix;
	glm::vec3 m_lightPos;
	glm::vec3 m_camPos;

	glm::vec3 m_collisionBoxColor;

	int m_verticeCount;
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
	// Visibility flag is checked in draw().
	bool m_isVisible = true;
	// This flag is used in draw() to determine wheter to use physicalObject's model matrix controlled by simulation.
	bool m_isPhysical = false;
	bool m_drawCollisionShape = true;
	// Physical object defines model's reaction's to physical simulation.
	PhysicalObject * m_physicalObject;
};

#endif