#pragma once

#include <iostream>
#include <vector>
#include <functional>
#include "PhysicalObject.h"
#include "Util.h"
#include "SimulationBase.h"
#include "Ode.h"

/*
Controls physical objects.
*/
class PhysicalSimulation : public SimulationBase
{
public:

	PhysicalSimulation() {}

	~PhysicalSimulation() {
		m_physicalObjects.clear();
	}

	/*
	Clear all forces and toeque.
	*/
	void clearForcesAndTorque();


	/*
	Adds given PhysicalObject to the simulation.
	*/
	void addPhysicalObject(PhysicalObject * po) {
		m_physicalObjects.push_back(po);
	}

	void stepSimulation(float t0, float t1) {
		Ode::solve(t0, t1, this);
	}

	void derivate(std::vector<float> &dst);

	/*
	Currently just sets the force and torque to 
	some value for all the PhysicalObjects. Later
	should just use forces that affect some point
	on object and calculate torque from that.
	*/
	void computeForcesAndTorque();
	/*
	Get current state of all PhysicalObjects in the simulation
	as a flat list.
	*/
	void getState(std::vector<float> &dst);

	/*
	Set the state of all PhysicalObjects in the simulation
	from a flat list.
	*/
	void setState(std::vector<float> &src);

	/*
	Turn a vector into a matrix.

	v =

	+-               -+
	|  0   -v.z   v.y |
	| v.z    0   -v.x |
	| -v.y  v.x    0  |
	+-               -+

	*/
	glm::mat3 star(glm::vec3 v) {
		return glm::mat3(
			0.0f, -v[2], v[1],
			v[2], 0.0f, -v[0],
			-v[1], v[0], 0.0f
		);
	}

	/*
	Get size_t needed for allocating space for all
	PhysicalObject's properties as flat list (std::vector).
	*/
	size_t getDim() {
		return m_physicalObjects.size() * STATE_SIZE;
	}

private:
	std::vector<PhysicalObject*> m_physicalObjects;
	const size_t STATE_SIZE = 18;
};