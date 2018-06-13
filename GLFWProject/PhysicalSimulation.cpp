#include "PhysicalSimulation.h"


void PhysicalSimulation::clearForcesAndTorque() {
	for(std::size_t i = 0; i < m_physicalObjects.size(); i++) {
		m_physicalObjects[i]->force = glm::vec3(0.0f);
		m_physicalObjects[i]->torque = glm::vec3(0.0f);
	}
}

void PhysicalSimulation::computeForcesAndTorque() {
	// for all particles
	for(std::size_t i = 0; i < m_physicalObjects.size(); i++) {
		PhysicalObject * po = m_physicalObjects[i];
		glm::vec3 f = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 torq = glm::vec3(0.0f, 0.0f, 0.0f);
		po->force = f;
		po->torque = torq;
	}

	// Apply all forces
	/*for(std::size_t k = 0; k < m_forces.size(); k++) {
	m_forces[k]->applyForce();
	}*/
}

/*
Composition of dst list:

v = velocity, R = rotation matrix, f = force, q = torque

idx: value
0  : v.x
1  : v.y
2  : v.z
3  : R00
4  : R01
5  : R02
6  : R10
7  : R11
8  : R12
9  : R20
10 : R21
11 : R22
12 : f.x
13 : f.y
14 : f.z
15 : q.x
16 : q.y
17 : q.z

*/
void PhysicalSimulation::derivate(std::vector<float> &dst) {
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
		// Compute Rdot(t) = omega(t) ∗ R(t)
		glm::mat3 Rdot = star(po->omega) * po->R;
		// copy Rdot(t) into array 
		int idx = 3;
		for(int i = 0; i < 3; i++) {
			for(int j = 0; j < 3; j++) {
				// ith column jth row
				dst[ks + idx] = Rdot[i][j];
				idx++;
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

/*
Composition of dst list:

x = position, R = rotation matrix, p = linear momentum, l = angular momentum

idx: value
0  : x.x
1  : x.y
2  : x.z
3  : R00
4  : R01
5  : R02
6  : R10
7  : R11
8  : R12
9  : R20
10 : R21
11 : R22
12 : p.x
13 : p.y
14 : p.z
15 : l.x
16 : l.y
17 : l.z

*/
void PhysicalSimulation::getState(std::vector<float> &dst) {

	if(dst.size() != getDim()) {
		std::cerr << __func__ << "() Destination vector is of wrong size to hold body states. " << std::endl;
		throw(1);
	}
	for(std::size_t k = 0; k < m_physicalObjects.size(); k++) {
		PhysicalObject * po = m_physicalObjects[k];
		int ks = k * STATE_SIZE;
		dst[ks + 0] = po->m_x[0];
		dst[ks + 1] = po->m_x[1];
		dst[ks + 2] = po->m_x[2];
		int idx = 3;
		for(int i = 0; i < 3; i++) {
			for(int j = 0; j < 3; j++) {
				// ith column jth row
				dst[ks + idx] = po->R[i][j];
				idx++;
			}
		}
		dst[ks + 12] = po->P[0];
		dst[ks + 13] = po->P[1];
		dst[ks + 14] = po->P[2];
		dst[ks + 15] = po->L[0];
		dst[ks + 16] = po->L[1];
		dst[ks + 17] = po->L[2];
	}
}

void PhysicalSimulation::setState(std::vector<float> &src) {

	if(src.size() != getDim()) {
		std::cerr << __func__ << "() Source vector is of wrong size to hold body states. " << std::endl;
		throw(1);
	}

	for(std::size_t k = 0; k < m_physicalObjects.size(); k++) {
		PhysicalObject * po = m_physicalObjects[k];
		int ks = k * STATE_SIZE;
		po->m_x[0] = src[ks + 0];
		po->m_x[1] = src[ks + 1];
		po->m_x[2] = src[ks + 2];
		int idx = 3;
		for(int i = 0; i < 3; i++) {
			for(int j = 0; j < 3; j++) {
				// ith column jth row
				po->R[i][j] = src[ks + idx];
				idx++;
			}
		}
		po->P[0] = src[ks + 12];
		po->P[1] = src[ks + 13];
		po->P[2] = src[ks + 14];
		po->L[0] = src[ks + 15];
		po->L[1] = src[ks + 16];
		po->L[2] = src[ks + 17];

		/* Compute auxiliary variables... */
		// v(t) = P(t) / M
		po->v = po->P * (1.f / po->mass);
		// I_−1(t) = R(t) * I_−1_body * R(t)_T
		po->Iinv = po->R * po->getBodyInertiaInv() * transpose(po->R);
		// omega(t) = I_−1(t) * L(t)
		po->omega = po->Iinv * po->L;
	}
}