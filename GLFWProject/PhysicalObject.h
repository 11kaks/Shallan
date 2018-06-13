#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> // translate, rotate..

#include "CollisionShape.h"


class PhysicalObject
{
public:
	PhysicalObject() {
		/*
		xyz is one corner point of origo symmetric box.
		This defines the geometry of the whole box.
		*/
		float x = 1.0f;
		float y = 1.0f;
		float z = 1.0f;

		m_collisionShape = new CollisionBoxOrigoSymmetric(glm::vec3(x, y, z));

		mass = 1.0f;
		glm::mat3 body = glm::mat3(
			y*y + z*z, 0.0f, 0.0f,
			0.0f, x*x + z*z, 0.0f,
			0.0f, 0.0f, x*x + y*y
		);
		Ibody = (mass / 12) * body;
		Ibodyinv = glm::inverse(Ibody);
		// Set some angular velocity for testing
		omega = glm::vec3(0.0f, 0.0f, 0.0f);
		m_x = glm::vec3(0.0f);
		v = glm::vec3(0.0f);
		// Rotation initialized as identity matrix.
		R = glm::mat3(1.0f);
		P = glm::vec3(1.0f);
		L = glm::vec3(1.0f);
	}
	
	glm::mat4 getModelMatrix() {
		glm::mat4 m = glm::mat4();
		m[0] = glm::vec4(R[0], 0.0f); // first column
		m[1] = glm::vec4(R[1], 0.0f); // etc..
		m[2] = glm::vec4(R[2], 0.0f);
		m[3] = glm::vec4(m_x, 1.0f);
		return m;
	}

	void setModelMatrix(glm::mat4 m) {
		R[0] = glm::vec3(m[0]);
		R[1] = glm::vec3(m[1]);
		R[2] = glm::vec3(m[2]);
		m_x  = glm::vec3(m[3]);
	}

	~PhysicalObject();


	/* Constant quantities defined before running simulation. */
	float mass; /* mass M */
	glm::mat3 Ibody; /* Ibody */
	glm::mat3 Ibodyinv; /* I−1 body (inverse of Ibody) */

	/* State variables */
	glm::vec3 m_x; /* x(t) */
	glm::mat3 R; /* R(t) */
	glm::vec3 P; /* P(t) */
	glm::vec3 L; /* L(t) */

	/* Derived quantities (auxiliary variables) */
	glm::mat3 Iinv; /* I−1(t) */
	// Velocity
	glm::vec3 v;
	// Angular velocity
	glm::vec3 omega; 

	/* Computed quantities */
	glm::vec3 force; /* F(t) */
	glm::vec3 torque; /* torque(t) */

	CollisionShape * m_collisionShape;

};

