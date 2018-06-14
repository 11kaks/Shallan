#include "PhysicalObject.h"

PhysicalObject::PhysicalObject() {
	/*
	xyz is one corner point of origosymmetric box.
	This defines the geometry of the whole box.
	*/
	glm::vec3 physicalBoxCorner = glm::vec3(1.0f, 1.0f, 1.0f);

	m_collisionShape = new CollisionBoxOrigoSymmetric(physicalBoxCorner);
	m_inertiaShape = new InertiaBox(physicalBoxCorner);

	m_mass = 1.0f;

	// Set some angular velocity for testing
	omega = glm::vec3(0.0f);
	pos = glm::vec3(0.0f);
	v = glm::vec3(0.0f);
	// Rotation initialized as identity matrix.
	R = glm::mat3(1.0f);
	P = glm::vec3(0.0f);
	L = glm::vec3(0.0f);
}

glm::mat4 PhysicalObject::getModelMatrix() {
	glm::mat4 m = glm::mat4();
	m[0] = glm::vec4(R[0], 0.0f); // first column
	m[1] = glm::vec4(R[1], 0.0f); // etc..
	m[2] = glm::vec4(R[2], 0.0f);
	m[3] = glm::vec4(pos, 1.0f);
	return m;
}

void PhysicalObject::setModelMatrix(glm::mat4 m) {
	R[0] = glm::vec3(m[0]);
	R[1] = glm::vec3(m[1]);
	R[2] = glm::vec3(m[2]);
	pos = glm::vec3(m[3]);
}