#include "Object3D.h"
#include <vector>

using namespace std;

void _CheckGLError(const char* file, int line);

#define CheckGLError() _CheckGLError(__FILE__, __LINE__)

Object3D::Object3D(std::string objectName) {
	m_objectName = objectName;
	init();
}

Object3D::Object3D() {
	m_objectName = m_defaultObjectName;
	init();
}

void Object3D::init() {
	// Initialize matrises as identity.
	m_viewMatrix = glm::mat4();
	m_modelMatrix = glm::mat4(1.0f);
	m_projectionMatrix = glm::mat4();

	// Testing if shader can work with other than identity matrix.
	// Rotate model space
	//m_modelMatrix = glm::rotate(m_modelMatrix, 45.0f, glm::vec3(0.f, 1.f, 0.f));
	// Translate model space
	//m_modelMatrix = glm::translate(m_modelMatrix, glm::vec3(0.f, 0.9f, 0.f));
	//m_modelMatrix = glm::scale(m_modelMatrix, glm::vec3(1.5f));

	// Initialize light and comera somewhere.
	m_lightPos = glm::vec3(3.0f, 3.0f, 4.0f);
	m_camPos = glm::vec3(3.0f, 1.0f, -4.0f);

	m_collisionBoxColor = glm::vec3(0.0f, 0.7f, 0.0f);

	// Set default object name and shaders.
	m_vertexShaderName = m_defaultVertexShaderName;
	m_fragmentShaderName = m_defaultFragmentShaderName;

	// Create and compile our GLSL program from the shaders
	reloadShaders();
	reloadCbbShaders();

	// Get a handles for uniforms
	m_mvpMatrixID = glGetUniformLocation(m_programID, "MVP");
	m_modelMatrixID = glGetUniformLocation(m_programID, "M");
	m_viewMatrixID = glGetUniformLocation(m_programID, "V");
	m_lightPosId = glGetUniformLocation(m_programID, "LightPosition_worldspace");
	m_camPosId = glGetUniformLocation(m_programID, "CameraPosition_worldspace");
	// Not used
	//m_timeID = glGetUniformLocation(m_programID, "inTime");

	std::vector< glm::vec3 > vertices;
	std::vector< glm::vec2 > uvs;// UVs are not used
	std::vector< glm::vec3 > normals;
	std::string objectFilePath = m_objectFilePath + m_objectName + m_objectFileEnding;
	bool res = loadOBJ(objectFilePath.c_str(), vertices, uvs, normals);

	if(res) {
		// Verticecount needed in draw().
		m_verticeCount = vertices.size();
	} else {
		std::cerr << "Object loading failed." << endl;
		throw 1;
	}

	// Generate 1 buffer, put the resulting identifier in m_vertexBufferID.
	glGenBuffers(1, &m_vertexBufferID);
	// Bind the buffer.
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferID);
	// Give our vertices to OpenGL.
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

	// Same for normals.
	glGenBuffers(1, &m_normalBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, m_normalBufferID);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
}

void Object3D::initCbb() {
	std::cout << "Initializing collision bounding box for drawing.." << std::endl;

	// Get a handles for uniforms
	// TODO: Maby I could use the same name as in the main shader??
	m_cbbMvpMatrixID = glGetUniformLocation(m_cbbProgramID, "MVP");
	CollisionShape * cs = m_physicalObject->getCollisionShape();

	if(cs == NULL) {
		std::cout << "Collision shape was null while trying to initialize cbb drawing!" << std::endl;
		return;
	}

	std::vector< glm::vec4 > vertices = m_physicalObject->getCollisionShape()->getCornerPointList();
	glGenBuffers(1, &m_cbbVertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, m_cbbVertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec4), &vertices[0], GL_STATIC_DRAW);


	GLuint elements[] = {
		0, 1, 2, 3,
		4, 5, 6, 7,
		0, 4, 1, 5, 2, 6, 3, 7
	};
	glGenBuffers(1, &m_cbbElementsID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_cbbElementsID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);
}

void Object3D::reloadCbbShaders() {
	string cbbVertexShaderFilePath = m_shaderFilePath + "cbb" + m_vertexShaderFileEndig;
	string cbbFragmentShaderFilePath = m_shaderFilePath + "cbb" + m_fragmentShaderFileEnding;
	m_cbbProgramID = LoadShaders(cbbVertexShaderFilePath.c_str(), cbbFragmentShaderFilePath.c_str());
}

void Object3D::drawCollisionBoundingBox() {
	// https://www.khronos.org/opengl/wiki/Buffer_Object

	// Use our shader
	glUseProgram(m_cbbProgramID);
	CheckGLError();
	// Our ModelViewProjection : multiplication of our 3 matrices
	glm::mat4 MVP = m_projectionMatrix * m_viewMatrix * m_modelMatrix;

	// Send matrices to the currently bound shader.
	glUniformMatrix4fv(m_cbbMvpMatrixID, 1, GL_FALSE, &MVP[0][0]);
	CheckGLError();
	
	glEnableVertexAttribArray(m_vaoCbbVertsID);
	CheckGLError();
	glBindBuffer(GL_ARRAY_BUFFER, m_cbbVertexBufferID);
	CheckGLError();
	glVertexAttribPointer(
		3,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		4,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);
	CheckGLError();
	
	glLineWidth(4);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_cbbElementsID);
	CheckGLError();
	glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_INT, 0);
	glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_INT, (GLvoid*)(4 * sizeof(GLuint)));
	glDrawElements(GL_LINES, 8, GL_UNSIGNED_INT, (GLvoid*)(8 * sizeof(GLuint)));
	CheckGLError();
	glDisableVertexAttribArray(m_vaoCbbVertsID);
	CheckGLError();
	
}

void Object3D::draw() {

	if(!m_isVisible) {
		return;
	}

	/*
	If this is a physical object, let's ask for current
	model matrix as set by PhysicalSimultaion.
	*/
	if(m_isPhysical) {
		m_modelMatrix = m_physicalObject->getModelMatrix();
		drawCollisionBoundingBox();
	}

// Use our shader
	
	glUseProgram(m_programID);

	// Our ModelViewProjection : multiplication of our 3 matrices
	glm::mat4 MVP = m_projectionMatrix * m_viewMatrix * m_modelMatrix;

	// Send matrices to the currently bound shader.
	glUniformMatrix4fv(m_mvpMatrixID, 1, GL_FALSE, &MVP[0][0]);
	glUniformMatrix4fv(m_modelMatrixID, 1, GL_FALSE, &m_modelMatrix[0][0]);
	glUniformMatrix4fv(m_viewMatrixID, 1, GL_FALSE, &m_viewMatrix[0][0]);

	if(m_useLightToCam){
		m_lightPos = m_camPos;
	}

	glUniform3fv(m_lightPosId, 1, &m_lightPos[0]);
	glUniform3fv(m_camPosId, 1, &m_camPos[0]);

	// Do time-dependant stuff
	//float time = (float)glfwGetTime() ;
	//glUniform1f(m_timeID, time);

	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(m_vaoMainVertsID);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferID);
	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);
	// 2nd buffer could be color, but not used now.

	// 3rd attribute buffer : normals
	glEnableVertexAttribArray(m_vaoMainNormalsID);
	glBindBuffer(GL_ARRAY_BUFFER, m_normalBufferID);
	glVertexAttribPointer(
		2,                                // attribute
		3,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);

	// Last do this
	try {
		glDrawArrays(GL_TRIANGLES, 0, m_verticeCount);
	} catch(...) {
		std::cout << "Some error while drawing main shape" << std::endl;
	}
	glDisableVertexAttribArray(m_vaoMainVertsID);
	glDisableVertexAttribArray(m_vaoMainNormalsID);
	
}

void Object3D::reloadShaders() {
	string sVertexShaderFilePath = m_shaderFilePath + m_vertexShaderName + m_vertexShaderFileEndig;
	string sFragmentShaderFilePath = m_shaderFilePath + m_fragmentShaderName + m_fragmentShaderFileEnding;
	m_programID = LoadShaders(sVertexShaderFilePath.c_str(), sFragmentShaderFilePath.c_str());
}

Object3D::~Object3D() {
	glDeleteBuffers(1, &m_vertexBufferID);
	glDeleteBuffers(1, &m_normalBufferID);
	glDeleteBuffers(1, &m_cbbVertexBufferID);
	glDeleteBuffers(1, &m_cbbElementsID);
	glDeleteProgram(m_programID);
}


void _CheckGLError(const char* file, int line) {
	GLenum err(glGetError());

	while(err != GL_NO_ERROR) {
		std::string error;
		switch(err) {
		case GL_INVALID_OPERATION:  error = "INVALID_OPERATION";      break;
		case GL_INVALID_ENUM:       error = "INVALID_ENUM";           break;
		case GL_INVALID_VALUE:      error = "INVALID_VALUE";          break;
		case GL_OUT_OF_MEMORY:      error = "OUT_OF_MEMORY";          break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:  error = "INVALID_FRAMEBUFFER_OPERATION";  break;
		}
		std::cout << "GL_" << error.c_str() << " - " << file << ":" << line << std::endl;
		err = glGetError();
	}

	return;
}
