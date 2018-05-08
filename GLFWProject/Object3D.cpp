#include "Object3D.h"

using namespace std;

void initColorBuffer();

Object3D::Object3D() {

	m_projectionMatrix = glm::mat4();
	m_viewMatrix = glm::mat4();
	m_modelMatrix = glm::mat4(1.0f);
	// FIXME hard coded light position
	m_lightPos = glm::vec3(3.0f, 3.0f, 4.0f);

	m_objectName = m_defaultObjectName;
	m_vertexShaderName = m_defaultVertexShaderName;
	m_fragmentShaderName = m_defaultFragmentShaderName;
	
	// Testing some transformations
	/*Model = glm::rotate(Model, 30.0f, glm::vec3(0., 1., 0.));
	Model = glm::translate(Model, glm::vec3(0., 0., 4.));
	Model = glm::scale(Model, glm::vec3(3.));*/

	// Create and compile our GLSL program from the shaders
	reloadShaders();

	// Get a handle for our "MVP" uniform
	m_mvpMatrixID = glGetUniformLocation(m_programID, "MVP");
	m_modelMatrixID = glGetUniformLocation(m_programID, "M");
	m_viewMatrixID = glGetUniformLocation(m_programID, "V");
	m_lightPosId = glGetUniformLocation(m_programID, "LightPosition_worldspace");

	m_timeID = glGetUniformLocation(m_programID, "inTime");

	std::vector< glm::vec3 > m_vertices;
	std::vector< glm::vec2 > m_uvs;
	std::vector< glm::vec3 > m_normals;
	std::string objectFilePath = m_objectFilePath + m_objectName + m_objectFileEnding;
	bool res = loadOBJ(objectFilePath.c_str(), m_vertices, m_uvs, m_normals);

	if(res) {
		m_verticeCount = m_vertices.size();
		cout << "Vertice count " << m_verticeCount << endl;
	} else {
		throw 1;
	}

	// FIXME hack. Remove when lighting is ok.
	initColorBuffer();
	
	// Generate 1 buffer, put the resulting identifier in vertexbuffer
	glGenBuffers(1, &m_vertexBufferID);
	// The following commands will talk about our 'vertexbuffer' buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferID);
	// Give our vertices to OpenGL.
	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(glm::vec3), &m_vertices[0], GL_STATIC_DRAW);
	//return vertexbuffer;


	// NORMALS

	glGenBuffers(1, &m_normalBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, m_normalBufferID);
	glBufferData(GL_ARRAY_BUFFER, m_normals.size() * sizeof(glm::vec3), &m_normals[0], GL_STATIC_DRAW);

}

void Object3D::draw() {
	// Use our shader
	glUseProgram(m_programID);

	// Our ModelViewProjection : multiplication of our 3 matrices
	glm::mat4 MVP = m_projectionMatrix * m_viewMatrix * m_modelMatrix; // Remember, matrix multiplication is the other way around

	// Send our transformation to the currently bound shader, 
	// in the "MVP" uniform
	glUniformMatrix4fv(m_mvpMatrixID, 1, GL_FALSE, &MVP[0][0]);
	glUniformMatrix4fv(m_modelMatrixID, 1, GL_FALSE, &m_modelMatrix[0][0]);
	glUniformMatrix4fv(m_viewMatrixID, 1, GL_FALSE, &m_viewMatrix[0][0]);
	glUniform3fv(m_lightPosId, 1, &m_lightPos[0]);
	float time = (float)glfwGetTime() ;
	//cout << time << endl;
	glUniform1f(m_timeID, time);

	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferID);
	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	// 2nd attribute buffer : colors
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBufferID);
	glVertexAttribPointer(
	1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
	3,                                // size
	GL_FLOAT,                         // type
	GL_FALSE,                         // normalized?
	0,                                // stride
	(void*)0                          // array buffer offset
	);

	// 3rd attribute buffer : normals
	glEnableVertexAttribArray(2);
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
	glDrawArrays(GL_TRIANGLES, 0, m_verticeCount); // Starting from vertex 0; 3*12 vertices total -> 6 sided cube
	glDisableVertexAttribArray(0);

}

void Object3D::reloadShaders() {
	string sVertexShaderFilePath = m_shaderFilePath + m_vertexShaderName + m_vertexShaderFileEndig;
	string sFragmentShaderFilePath = m_shaderFilePath + m_fragmentShaderName + m_fragmentShaderFileEnding;
	m_programID = LoadShaders(sVertexShaderFilePath.c_str(), sFragmentShaderFilePath.c_str());
}

void Object3D::reloadObject() {

}

Object3D::~Object3D() {
	cout << "Deleting object" << endl;
	glDeleteBuffers(1, &m_vertexBufferID);
	glDeleteBuffers(1, &m_colorBufferID);
	glDeleteBuffers(1, &m_normalBufferID);
	glDeleteProgram(m_programID);
}

// Just some colors for testing...
void initColorBuffer() {

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

	GLuint m_colorbuffer;
	glGenBuffers(1, &m_colorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, m_colorbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);
}
