#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> // translate, rotate..


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
		mass = 1.0f;
		glm::mat3 body = glm::mat3(
			y*y + z*z, 0.0f, 0.0f,
			0.0f, x*x + z*z, 0.0f,
			0.0f, 0.0f, x*x + y*y
		);
		Ibody = (mass / 12) * body;
		Ibodyinv = glm::inverse(Ibody);
		// Set some angular velocity for testing
		omega = glm::vec3(2.0f, 0.0f, 0.0f);
		m_x = glm::vec3(0.0f);
		v = glm::vec3(0.0f);
		// Rotation initialized as identity matrix.
		R = glm::mat3(1.0f);
		P = glm::vec3(0.0f);
		L = glm::vec3(0.0f);
	}

	~PhysicalObject();

	const size_t STATE_SIZE = 18;

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
	glm::vec3 v; /* v(t) */
	glm::vec3 omega; /* omega(t) */

	/* Computed quantities */
	glm::vec3 force; /* F(t) */
	glm::vec3 torque; /* torque(t) */	/* 
	Copy the state information into an array.
	*/
	void StateToArray(PhysicalObject *rb, float *y) {
		*y++ = rb->m_x[0]; /* x component of position */
		*y++ = rb->m_x[1]; /* etc. */
		*y++ = rb->m_x[2]; 
		/* copy rotation matrix */
		for(int i = 0; i < 3; i++) {
			for(int j = 0; j < 3; j++) {
				// ith column jth row
				*y++ = rb->R[i][j];			}		}		*y++ = rb->P[0];
		*y++ = rb->P[1];
		*y++ = rb->P[2];
		*y++ = rb->L[0];
		*y++ = rb->L[1];
		*y++ = rb->L[2];
	}

	/* 
	Copy information from an array into the state variables 

	Note that ArrayToState is responsible for computing 
	values for the auxiliary variables Iinv, v and omega.
	*/
	void ArrayToState(PhysicalObject *rb, float *y) {
		rb->m_x[0] = *y++;
		rb->m_x[1] = *y++;
		rb->m_x[2] = *y++;
		for(int i = 0; i < 3; i++) {
			for(int j = 0; j < 3; j++) {
				// ith column jth row
				rb->R[i][j] = *y++;
			}
		}
		rb->P[0] = *y++;
		rb->P[1] = *y++;
		rb->P[2] = *y++;
		rb->L[0] = *y++;
		rb->L[1] = *y++;
		rb->L[2] = *y++;
		/* Compute auxiliary variables... */
		// v(t) = P(t) / M
		rb->v = rb->P * (1.f /mass);
		// I_−1(t) = R(t) * I_−1_body * R(t)_T
		rb->Iinv = R * Ibodyinv * transpose(R);
		// omega(t) = I_−1(t) * L(t)
		rb->omega = rb->Iinv * rb->L;
	}
};

