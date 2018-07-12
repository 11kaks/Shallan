#pragma once

#include <glm/glm.hpp>
/*
@author Kimmo Riihiaho (kimmo.riihiaho@gmail.com)
@date 14.6.2018

InertiaShape describes how the object should react to 
rotation. Hollow ball would be more difficult to get
rolling than a solid ball of same mass.

InertiaShape offers inertia as a 3 x 3 matrix that 
can be used as scaling factor when computing angular
momemtum and angular velocity. Inertia tensor remains
constant through simulation though it does depend on 
objects mass.
*/
class InertiaShape
{
public:
	glm::mat3 getInertiaTensor(float mass) {
		return m_inertiaTensor;
	}
	glm::mat3 getInertiaTensorInv(float mass) {
		return glm::inverse(m_inertiaTensor);
	}

	~InertiaShape() {}
protected:
	// This might not be good practise..
	InertiaShape() {}
	glm::mat3 m_inertiaTensor;
};

/*
Inertia of a origosymmetric solid box.
*/
class InertiaBox : public InertiaShape
{
public:

	/*
	Construct new box inertia by providing
	one corner point of the box.
	*/
	InertiaBox(glm::vec3 corner) {
		// Absolute taken to define the box 
		// in the positive octant of the space.
		float x = abs(corner[0]) / 2.0f;
		float y = abs(corner[1]) / 2.0f;
		float z = abs(corner[2]) / 2.0f;
		m_inertiaTensor = glm::mat3(
			y*y + z * z, 0.0f, 0.0f,
			0.0f, x*x + z * z, 0.0f,
			0.0f, 0.0f, x*x + y * y
		);
	}
	~InertiaBox() {}

	/*
	Get the inertia tensor for object with given mass.
	*/
	glm::mat3 getInertiaTensor(float mass) {
		return (mass / 12.0f) * m_inertiaTensor;
	}
	/*
	Get the inverse of inertia tensor for object with given mass.
	*/
	glm::mat3 getInertiaTensorInv(float mass) {
		return (mass / 12.0f) * glm::inverse(m_inertiaTensor);
	}

};