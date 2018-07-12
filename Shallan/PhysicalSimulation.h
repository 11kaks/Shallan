#pragma once

#include <iostream>
#include <vector>
#include <functional>
#include "PhysicalObject.h"
#include "Util.h"
#include "SimulationBase.h"
#include "Ode.h"

/*
@author Kimmo Riihiaho, kimmo.riihiaho@gmail.com
@date 14.6.2018

Controls physical objects.
*/
class PhysicalSimulation : public SimulationBase
{
public:

	/*
	Create a new empty simulation
	*/
	PhysicalSimulation() {}

	/*
	Delete the simulation and it's physical objects.
	*/
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

	/*
	Drive the simulation from starting time t0
	to ending time t1.

	@param t0 starting time in seconds
	@param t1 ending time in seconds
	*/
	void stepSimulation(float t0, float t1);

	/*
	Get the time derivative of simulation state.

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
	void getState(std::vector<float> &dst);

	/*
	Set the state of all PhysicalObjects in the simulation
	from a flat list. Also computes auxiliary variables: velocity, angular velocity 
	and inverse of inertia matrix.
		
	Composition of src vector

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
	// List of all objects in the simulation.
	std::vector<PhysicalObject*> m_physicalObjects;

	// Amount of state variables of a single physical object.
	const size_t STATE_SIZE = 18;
};