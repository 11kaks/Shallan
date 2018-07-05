#include "Object3D.h"
// Check errors and print to cout. Call after every GL operation.
#include "CheckGLError.h"
#include "Util.h"
#include <vector>

using namespace std;


Object3D::Object3D(std::string objectName) {
	m_objectName = objectName;
	init();
}

Object3D::Object3D() {
	m_objectName = m_defaultObjectName;
	init();
}

void Object3D::init() {
	// Initialize matrices as identity.
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
	CheckGLError();
	m_modelMatrixID = glGetUniformLocation(m_programID, "M");
	CheckGLError();
	m_viewMatrixID = glGetUniformLocation(m_programID, "V");
	CheckGLError();
	m_lightPosId = glGetUniformLocation(m_programID, "LightPosition_worldspace");
	CheckGLError();
	m_camPosId = glGetUniformLocation(m_programID, "CameraPosition_worldspace");
	CheckGLError();
	// Not used
	//m_timeID = glGetUniformLocation(m_programID, "inTime");

	std::vector< glm::vec3 > vertices;
	std::vector< glm::vec2 > uvs;// UVs are not used
	std::vector< glm::vec3 > normals;
	std::string objectFilePath = m_objectFilePath + m_objectName + m_objectFileEnding;
	bool res = loadOBJ(objectFilePath.c_str(), vertices, uvs, normals);

	m_furthestCorner = Util::maxXYZ(vertices);

	if(res) {
		// Verticecount needed in draw().
		m_verticeCount = vertices.size();
	} else {
		std::cerr << "Object loading failed." << endl;
		throw 1;
	}

	// Generate 1 buffer, put the resulting identifier in m_vertexBufferID.
	glGenBuffers(1, &m_vertexBufferID);
	CheckGLError();
	// Bind the buffer.
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferID);
	CheckGLError();
	// Give our vertices to OpenGL.
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
	CheckGLError();

	// Same for normals.
	glGenBuffers(1, &m_normalBufferID);
	CheckGLError();
	glBindBuffer(GL_ARRAY_BUFFER, m_normalBufferID);
	CheckGLError();
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
	CheckGLError();
}

void Object3D::initCbb() {
	std::cout << "Initializing collision bounding box for drawing.." << std::endl;

	// Get a handles for uniforms
	// TODO: Maby I could use the same name as in the main shader??
	m_cbbMvpMatrixID = glGetUniformLocation(m_cbbProgramID, "MVP");
	CheckGLError();

	CollisionShape * cs = m_physicalObject->getCollisionShape();

	std::vector< glm::vec4 > vertices = cs->getCornerPointList();
	glGenBuffers(1, &m_cbbVertexBufferID);
	CheckGLError();
	glBindBuffer(GL_ARRAY_BUFFER, m_cbbVertexBufferID);
	CheckGLError();
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec4), &vertices[0], GL_STATIC_DRAW);
	CheckGLError();

	std::vector<GLuint> elements = cs->getCornerDrawOrder();
	glGenBuffers(1, &m_cbbElementsID);
	CheckGLError();
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_cbbElementsID);
	CheckGLError();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, elements.size() * sizeof(elements), &elements[0], GL_STATIC_DRAW);
	CheckGLError();

}

void Object3D::drawCollisionBoundingBox() {
	// https://www.khronos.org/opengl/wiki/Buffer_Object

	// Use our shader
	glUseProgram(m_cbbProgramID);
	CheckGLError();

	glm::mat4 MVP = m_projectionMatrix * m_viewMatrix * m_modelMatrix;
	// Send matrices to the currently bound shader.
	glUniformMatrix4fv(m_cbbMvpMatrixID, 1, GL_FALSE, &MVP[0][0]);
	CheckGLError();

	glEnableVertexAttribArray(m_vaoCbbVertsID);
	CheckGLError();
	glBindBuffer(GL_ARRAY_BUFFER, m_cbbVertexBufferID);
	CheckGLError();
	glVertexAttribPointer(
		m_vaoCbbVertsID,    // attribute 3
		4,                  // size vec4
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);
	CheckGLError();
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_cbbElementsID);
	CheckGLError();
	glDisableVertexAttribArray(m_vaoCbbVertsID);
	CheckGLError();

	glEnableVertexAttribArray(m_vaoCbbVertsID);
	glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_INT, 0);
	glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_INT, (GLvoid*)(4 * sizeof(GLuint)));
	glDrawElements(GL_LINES, 8, GL_UNSIGNED_INT, (GLvoid*)(8 * sizeof(GLuint)));
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
		if(m_drawCollisionShape){
			drawCollisionBoundingBox();
		}
	}

	// Use our shader	
	glUseProgram(m_programID);
	CheckGLError();

	glm::mat4 MVP = m_projectionMatrix * m_viewMatrix * m_modelMatrix;
	// Send matrices to the currently bound shader.
	glUniformMatrix4fv(m_mvpMatrixID, 1, GL_FALSE, &MVP[0][0]);
	CheckGLError();
	glUniformMatrix4fv(m_modelMatrixID, 1, GL_FALSE, &m_modelMatrix[0][0]);
	CheckGLError();
	glUniformMatrix4fv(m_viewMatrixID, 1, GL_FALSE, &m_viewMatrix[0][0]);
	CheckGLError();

	if(m_useLightToCam){
		m_lightPos = m_camPos;
	}

	glUniform3fv(m_lightPosId, 1, &m_lightPos[0]);
	CheckGLError();
	glUniform3fv(m_camPosId, 1, &m_camPos[0]);
	CheckGLError();

	// Do time-dependant stuff
	//float time = (float)glfwGetTime() ;
	//glUniform1f(m_timeID, time);

	// attribute buffer for vertices
	glEnableVertexAttribArray(m_vaoMainVertsID);
	CheckGLError();
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferID);
	CheckGLError();
	glVertexAttribPointer(
		m_vaoMainVertsID,   // must match the layout in the shader.
		3,                  // size vec3
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);
	CheckGLError();
	glDisableVertexAttribArray(m_vaoMainVertsID);


	// attribute buffer for normals
	glEnableVertexAttribArray(m_vaoMainNormalsID);
	CheckGLError();
	glBindBuffer(GL_ARRAY_BUFFER, m_normalBufferID);
	CheckGLError();
	glVertexAttribPointer(
		m_vaoMainNormalsID,               // attribute
		3,                                // size vec3
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);
	CheckGLError();
	glDisableVertexAttribArray(m_vaoMainNormalsID);

	glEnableVertexAttribArray(m_vaoMainVertsID);
	CheckGLError();
	glEnableVertexAttribArray(m_vaoMainNormalsID);
	CheckGLError();
	glDrawArrays(GL_TRIANGLES, 0, m_verticeCount);
	CheckGLError();
	glDisableVertexAttribArray(m_vaoMainVertsID);
	CheckGLError();
	glDisableVertexAttribArray(m_vaoMainNormalsID);
	CheckGLError();
	
}

void Object3D::reloadShaders() {
	string sVertexShaderFilePath = m_shaderFilePath + m_vertexShaderName + m_vertexShaderFileEndig;
	string sFragmentShaderFilePath = m_shaderFilePath + m_fragmentShaderName + m_fragmentShaderFileEnding;
	m_programID = LoadShaders(sVertexShaderFilePath.c_str(), sFragmentShaderFilePath.c_str());
}

void Object3D::reloadCbbShaders() {
	string cbbVertexShaderFilePath = m_shaderFilePath + "cbb" + m_vertexShaderFileEndig;
	string cbbFragmentShaderFilePath = m_shaderFilePath + "cbb" + m_fragmentShaderFileEnding;
	m_cbbProgramID = LoadShaders(cbbVertexShaderFilePath.c_str(), cbbFragmentShaderFilePath.c_str());
}

Object3D::~Object3D() {
	glDeleteBuffers(1, &m_vertexBufferID);
	CheckGLError();
	glDeleteBuffers(1, &m_normalBufferID);
	CheckGLError();
	glDeleteBuffers(1, &m_cbbVertexBufferID);
	CheckGLError();
	glDeleteBuffers(1, &m_cbbElementsID);
	CheckGLError();
	glDeleteProgram(m_programID);
	CheckGLError();
}