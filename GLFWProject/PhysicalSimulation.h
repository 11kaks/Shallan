#pragma once

#include <vector>
#include <functional>
#include "PhysicalObject.h"

/*
Controls physical objects.
*/
class PhysicalSimulation
{
public:

	int STATE_SIZE = 18;

	PhysicalSimulation() {
		PhysicalObject * po = new PhysicalObject();
		m_physicalObjects.push_back(po);
	}
	~PhysicalSimulation() {
		m_physicalObjects.clear();
	}

	//typedef void(PhysicalSimulation::*DerivFunc)(float t, float x[], float xdot[]);

	void ode(float x0[], float xEnd[], int len, float t0, float t1, std::function<void()> doNothing){
		// do some magic
	}

	void RunSimulation() {
		// FIXME use vector for dynamic allocation
		const int s = 18 ;
		float x0[s],
			xFinal[s];
		//InitStates();
		BodiesToArray(xFinal); for(double t = 0; t < 10.0; t += 1. / 24.) {
			/* copy xFinal back to x0 */
			for(int i = 0; i < STATE_SIZE *  m_physicalObjects.size(); i++) {
				x0[i] = xFinal[i];
				/*ode(x0, xFinal, STATE_SIZE *  m_physicalObjects.size(),
					t, t + 1. / 24., doNothing);*/
				/* copy d/dt X(t + 1/24 ) into state variables */
				ArrayToBodies(xFinal);
				//DisplayBodies();
			}
		}
	}

	void doNothing() {
		//
	}

	void dxdt(float t, float x[], float xdot[]) {
		/* put data in x[] into Bodies[] */
		ArrayToBodies(x);
		for(int i = 0; i < m_physicalObjects.size(); i++) {
			calculateForceAndTorque(t, m_physicalObjects[i]);
			DdtStateToArray(m_physicalObjects[i], &xdot[i * STATE_SIZE]);
		}
	}

	void DdtStateToArray(PhysicalObject *rb, float *xdot) {
		/* copy d
		dt x(t) = v(t) into xdot */
		*xdot++ = rb->v[0];
		*xdot++ = rb->v[1];
		*xdot++ = rb->v[2];
		/* Compute Rdot(t) = torque(t) ∗ R(t) */
		glm::mat3 Rdot = star(rb->omega) * rb->R;
		/* copy Rdot(t) into array */
		for(int i = 0; i < 3; i++) {
			for(int j = 0; j < 3; j++) {
				// ith column jth row
				*xdot++ = Rdot[i][j];
			}
		}
		*xdot++ = rb->force[0]; /* d/dt P(t) = F(t) */
		*xdot++ = rb->force[1];
		*xdot++ = rb->force[2];
		*xdot++ = rb->torque[0]; /* d/dt L(t) = torque(t) */
		*xdot++ = rb->torque[1];
		*xdot++ = rb->torque[2];
	}	glm::mat3 star(glm::vec3 v) {		return glm::mat3(			0.0f, -v[2], v[1],			v[2], 0.0f, -v[0],			-v[1], v[0], 0.0f		);	}

	void calculateForceAndTorque(float t, PhysicalObject *rb) {
		// do some magic
	}

	void ArrayToBodies(float x[]) {
		for(int i = 0; i < m_physicalObjects.size(); i++) {
			ArrayToState(m_physicalObjects[i], &x[i * STATE_SIZE]);
		}
	}

	void BodiesToArray(float x[]) {
		for(int i = 0; i < m_physicalObjects.size(); i++)
			StateToArray(m_physicalObjects[i], &x[i * STATE_SIZE]);
	}
	/*
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
		rb->v = rb->P * (1.f / rb->mass);
		// I_−1(t) = R(t) * I_−1_body * R(t)_T
		rb->Iinv = rb->R * rb->Ibodyinv * transpose(rb->R);
		// omega(t) = I_−1(t) * L(t)
		rb->omega = rb->Iinv * rb->L;
	}

	std::vector<PhysicalObject*> m_physicalObjects;
};

