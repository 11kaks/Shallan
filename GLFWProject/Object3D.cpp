#include "Object3D.h"


using namespace std;

Object3D::Object3D() {

}

Object3D::Object3D(glm::mat4 viewMatrix, glm::mat4 projectionMatrix) {

	m_projectionMatrix = projectionMatrix;
	m_viewMatrix = viewMatrix;
	m_modelMatrix = glm::mat4(1.0f);

	// Create and compile our GLSL program from the shaders
	m_programID = LoadShaders("shaders/SimpleVertexShader.vertexshader", "shaders/SimpleFragmentShader.fragmentshader");

	// Get a handle for our "MVP" uniform
	m_MatrixID = glGetUniformLocation(m_programID, "MVP");


	// Read our .obj file
	std::vector< glm::vec3 > vertices;
	std::vector< glm::vec2 > uvs;
	std::vector< glm::vec3 > normals; // Won't be used at the moment.
	bool res = loadOBJ("objects/cube.obj", vertices, uvs, normals);

	if(res) {
		m_verticeCount = vertices.size();
		cout << "Vertice count " << m_verticeCount << endl;
	} else {
		throw 1;
	}

	// This will identify our vertex buffer
	//GLuint vertexbuffer;

	// Generate 1 buffer, put the resulting identifier in vertexbuffer
	glGenBuffers(1, &m_vertexbuffer);
	// The following commands will talk about our 'vertexbuffer' buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexbuffer);
	// Give our vertices to OpenGL.
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
	//return vertexbuffer;

}

void Object3D::draw() {
	// Use our shader
	glUseProgram(m_programID);

	// Our ModelViewProjection : multiplication of our 3 matrices
	glm::mat4 MVP = m_projectionMatrix * m_viewMatrix * m_modelMatrix; // Remember, matrix multiplication is the other way around

	// Send our transformation to the currently bound shader, 
	// in the "MVP" uniform
	glUniformMatrix4fv(m_MatrixID, 1, GL_FALSE, &MVP[0][0]);

	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexbuffer);
	glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	glDrawArrays(GL_TRIANGLES, 0, m_verticeCount); // Starting from vertex 0; 3*12 vertices total -> 6 sided cube
	glDisableVertexAttribArray(0);
}



Object3D::~Object3D() {
	glDeleteProgram(m_programID);
}
