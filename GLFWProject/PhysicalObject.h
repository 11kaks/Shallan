#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> // translate, rotate..

#include "CollisionShape.h"
#include "InertiaShape.h"


class PhysicalObject
{
public:
	PhysicalObject() {
		/*
		xyz is one corner point of origosymmetric box.
		This defines the geometry of the whole box.
		*/
		glm::vec3 physicalBoxCorner = glm::vec3(1.0f, 1.0f, 1.0f);

		m_collisionShape = new CollisionBoxOrigoSymmetric(physicalBoxCorner);
		m_inertiaShape = new InertiaBox(physicalBoxCorner);

		mass = 1.0f;
		
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

	glm::mat3 getBodyInertia() {
		return m_inertiaShape->getInertiaTensor(mass);
	}

	glm::mat3 getBodyInertiaInv() {
		return m_inertiaShape->getInertiaTensorInv(mass);
	}

	~PhysicalObject();


	/* Constant quantities defined before running simulation. */
	float mass; /* mass M */
	//glm::mat3 Ibody; /* Ibody */
	//glm::mat3 Ibodyinv; /* I−1 body (inverse of Ibody) */

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

private:
	CollisionShape * m_collisionShape;
	InertiaShape * m_inertiaShape;

};

