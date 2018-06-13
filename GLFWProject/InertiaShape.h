#pragma once

#include <glm/glm.hpp>

class InertiaShape
{
public:
	glm::mat3 getInertiaTensor(float mass) {
		return m_inertiaTensor;
	}
	glm::mat3 getInertiaTensorInv(float mass) {
		return glm::inverse(m_inertiaTensor);
	}

protected:
	// This might not be good practise..
	InertiaShape() {}
	~InertiaShape() {}
	glm::mat3 m_inertiaTensor;
};

/*
Inertia of a origosymmetric box.
*/
class InertiaBox : public InertiaShape
{
public:
	InertiaBox(glm::vec3 corner) {

		float x = corner[0] / 2.0f;
		float y = corner[1] / 2.0f;
		float z = corner[2] / 2.0f;
		m_inertiaTensor = glm::mat3(
			y*y + z * z, 0.0f, 0.0f,
			0.0f, x*x + z * z, 0.0f,
			0.0f, 0.0f, x*x + y * y
		);
	}
	~InertiaBox() {}

	glm::mat3 getInertiaTensor(float mass) {
		return (mass / 12.0f) * m_inertiaTensor;
	}
	glm::mat3 getInertiaTensorInv(float mass) {
		return (mass / 12.0f) * glm::inverse(m_inertiaTensor);
	}

};