﻿#pragma once

#include <iostream>
#include <vector>
#include <functional>
#include "PhysicalObject.h"
#include "Util.h"

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

	void ode(float t0, float t1){
		// do some magic

		float h = t1 - t0;

		/*
		Time derivative of X, where X is (x', v').
		*/
		std::vector<float> d_dt(getDim());

		particleDerivative(d_dt);
		Util::scaleVector(d_dt, h);

		/*
		Particles' state before forces are taken into account (x, v).
		*/
		std::vector<float> particleState(getDim());
		getState(particleState);

		/*
		State of particle after force calculations (x + x' , v + v').
		*/
		std::vector<float> resultingState(getDim());
		Util::addVectors(d_dt, particleState, resultingState);
		setState(resultingState);
		//ArrayToBodies(resultingState);

	}

	void clearForcesAndTorque() {
		for(std::size_t i = 0; i < m_physicalObjects.size(); i++) {
			m_physicalObjects[i]->force = glm::vec3(0.0f);
			m_physicalObjects[i]->torque = glm::vec3(0.0f);
		}
	}

	void computeForcesAndTorque() {
		// for all particles
		for(std::size_t i = 0; i < m_physicalObjects.size(); i++) {
			PhysicalObject * po = m_physicalObjects[i];
			glm::vec3 f = glm::vec3(10.0f);
			glm::vec3 torq = glm::vec3(10.0f);
			po->force = f;
			po->torque = torq;
		}

		// Apply all forces
		/*for(std::size_t k = 0; k < m_forces.size(); k++) {
			m_forces[k]->applyForce();
		}*/
	}


	void particleDerivative(std::vector<float> &dst) {
		if(dst.size() != getDim()) {
			std::cerr << __func__ << "() Destination is of wrong size to hold particle states. " << std::endl;
			throw(1);
		}

		clearForcesAndTorque(); // zero the force accumulators 
		computeForcesAndTorque(); // magic force function 
		for(std::size_t k = 0; k < m_physicalObjects.size(); k++) {

			PhysicalObject * po = m_physicalObjects[k];
			int ks = STATE_SIZE * k;

			// copy d/dt x(t) = v(t) into xdot 
			dst[ks + 0] = po->v[0];
			dst[ks + 1] = po->v[1];
			dst[ks + 2] = po->v[2];
			// Compute Rdot(t) = torque(t) ∗ R(t)
			glm::mat3 Rdot = star(po->omega) * po->R;
			// copy Rdot(t) into array 
			for(int i = 0; i < 3; i++) {
				for(int j = 0; j < 3; j++) {
					// ith column jth row
					dst[ks + (i*j)] = Rdot[i][j];
				}
			}
			dst[ks + 12] = po->force[0]; // d/dt P(t) = F(t) 
			dst[ks + 13] = po->force[1];
			dst[ks + 14] = po->force[2];
			dst[ks + 15] = po->torque[0]; // d/dt L(t) = torque(t) 
			dst[ks + 16] = po->torque[1];
			dst[ks + 17] = po->torque[2];
		}
	}

	glm::mat3 star(glm::vec3 v) {
		return glm::mat3(
			0.0f, -v[2], v[1],
			v[2], 0.0f, -v[0],
			-v[1], v[0], 0.0f
		);
	}

	void getState(std::vector<float> &dst) {

		if(dst.size() != getDim()) {
			std::cerr << __func__ << "() Destination vector is of wrong size to hold body states. " << std::endl;
			throw(1);
		}
		for(std::size_t k = 0; k < m_physicalObjects.size(); k++) {
			int ks = k * STATE_SIZE;
			dst[ks + 0] = m_physicalObjects[k]->m_x[0];
			dst[ks + 1] = m_physicalObjects[k]->m_x[1];
			dst[ks + 2] = m_physicalObjects[k]->m_x[2];
			for(int i = 0; i < 3; i++) {
				for(int j = 0; j < 3; j++) {
					// ith column jth row
					dst[ks + (i*j)] = m_physicalObjects[k]->R[i][j];
				}
			}
			dst[ks + 12] = m_physicalObjects[k]->P[0];
			dst[ks + 13] = m_physicalObjects[k]->P[1];
			dst[ks + 14] = m_physicalObjects[k]->P[2];
			dst[ks + 15] = m_physicalObjects[k]->L[0];
			dst[ks + 16] = m_physicalObjects[k]->L[1];
			dst[ks + 17] = m_physicalObjects[k]->L[2];
		}
	}

	void setState(std::vector<float> &src) {

		if(src.size() != getDim()) {
			std::cerr << __func__ << "() Source vector is of wrong size to hold body states. " << std::endl;
			throw(1);
		}

		for(std::size_t k = 0; k < m_physicalObjects.size(); k++) {
			int ks = k * STATE_SIZE;
			m_physicalObjects[k]->m_x[0] = src[ks + 0];
			m_physicalObjects[k]->m_x[1] = src[ks + 1];
			m_physicalObjects[k]->m_x[2] = src[ks + 2];
			for(int i = 0; i < 3; i++) {
				for(int j = 0; j < 3; j++) {
					// ith column jth row
					m_physicalObjects[k]->R[i][j] = src[ks + (i*j)];
				}
			}
			m_physicalObjects[k]->P[0] = src[ks + 12];
			m_physicalObjects[k]->P[1] = src[ks + 13];
			m_physicalObjects[k]->P[2] = src[ks + 14];
			m_physicalObjects[k]->L[0] = src[ks + 15];
			m_physicalObjects[k]->L[1] = src[ks + 16];
			m_physicalObjects[k]->L[2] = src[ks + 17];

			/* Compute auxiliary variables... */
			// v(t) = P(t) / M
			m_physicalObjects[k]->v = m_physicalObjects[k]->P * (1.f / m_physicalObjects[k]->mass);
			// I_−1(t) = R(t) * I_−1_body * R(t)_T
			m_physicalObjects[k]->Iinv = m_physicalObjects[k]->R * m_physicalObjects[k]->Ibodyinv * transpose(m_physicalObjects[k]->R);
			// omega(t) = I_−1(t) * L(t)
			m_physicalObjects[k]->omega = m_physicalObjects[k]->Iinv * m_physicalObjects[k]->L;
		}
	}

	size_t getDim() {
		return m_physicalObjects.size() * STATE_SIZE;
	}

	std::vector<PhysicalObject*> m_physicalObjects;

	/*
	Copy the state information into an array.
	
	void StateToArray(PhysicalObject *rb, float *y) {
		*y++ = rb->m_x[0]; // x component of position 
		*y++ = rb->m_x[1]; // etc. 
		*y++ = rb->m_x[2];
		// copy rotation matrix 
		for(int i = 0; i < 3; i++) {
			for(int j = 0; j < 3; j++) {
				// ith column jth row
				*y++ = rb->R[i][j];
			}
		}
		*y++ = rb->P[0];
		*y++ = rb->P[1];
		*y++ = rb->P[2];
		*y++ = rb->L[0];
		*y++ = rb->L[1];
		*y++ = rb->L[2];
	}*/

	/*
	Copy information from an array into the state variables

	Note that ArrayToState is responsible for computing
	values for the auxiliary variables Iinv, v and omega.
	
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
		// Compute auxiliary variables... 
		// v(t) = P(t) / M
		rb->v = rb->P * (1.f / rb->mass);
		// I_−1(t) = R(t) * I_−1_body * R(t)_T
		rb->Iinv = rb->R * rb->Ibodyinv * transpose(rb->R);
		// omega(t) = I_−1(t) * L(t)
		rb->omega = rb->Iinv * rb->L;
	}*/



	//void RunSimulation() {
	//	// FIXME use vector for dynamic allocation
	//	const int s = 18 ;
	//	float x0[s],
	//		xFinal[s];
	//	//InitStates();
	//	BodiesToArray(xFinal); 
	//	for(double t = 0; t < 10.0; t += 1. / 24.) {
	//		/* copy xFinal back to x0 */
	//		for(int i = 0; i < STATE_SIZE *  m_physicalObjects.size(); i++) {
	//			x0[i] = xFinal[i];
	//			ode(x0, xFinal, STATE_SIZE *  m_physicalObjects.size(), t, t + 1. / 24.);
	//			/* copy d/dt X(t + 1/24 ) into state variables */
	//			ArrayToBodies(xFinal);
	//			//DisplayBodies();
	//		}
	//	}
	//}

	/*
	void dxdt(float t, float x[], float xdot[]) {
	// put data in x[] into Bodies[]
	ArrayToBodies(x);
	for(int i = 0; i < m_physicalObjects.size(); i++) {
	calculateForceAndTorque(t, m_physicalObjects[i]);
	DdtStateToArray(m_physicalObjects[i], &xdot[i * STATE_SIZE]);
	}
	}*/

	/*void calculateForceAndTorque(float t, PhysicalObject *rb) {
	// do some magic
	glm::vec3 f = glm::vec3(10.0f);
	glm::vec3 torq = glm::vec3(10.0f);
	rb->force = f;
	rb->torque = torq;
	}*/
	/*
	void ArrayToBodies(float x[]) {
	for(int i = 0; i < m_physicalObjects.size(); i++) {
	ArrayToState(m_physicalObjects[i], &x[i * STATE_SIZE]);
	}
	}*/

	/*
	void BodiesToArray(float x[]) {
	for(int i = 0; i < m_physicalObjects.size(); i++)
	StateToArray(m_physicalObjects[i], &x[i * STATE_SIZE]);
	}*/

	/*
	void DdtStateToArray(PhysicalObject *rb, float *xdot) {
	// copy d/dt x(t) = v(t) into xdot
	*xdot++ = rb->v[0];
	*xdot++ = rb->v[1];
	*xdot++ = rb->v[2];
	// Compute Rdot(t) = torque(t) ∗ R(t)
	glm::mat3 Rdot = star(rb->omega) * rb->R;
	// copy Rdot(t) into array
	for(int i = 0; i < 3; i++) {
	for(int j = 0; j < 3; j++) {
	// ith column jth row
	*xdot++ = Rdot[i][j];
	}
	}
	*xdot++ = rb->force[0]; // d/dt P(t) = F(t)
	*xdot++ = rb->force[1];
	*xdot++ = rb->force[2];
	*xdot++ = rb->torque[0]; // d/dt L(t) = torque(t)
	*xdot++ = rb->torque[1];
	*xdot++ = rb->torque[2];
	}*/
};

