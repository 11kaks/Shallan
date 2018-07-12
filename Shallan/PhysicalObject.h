#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> // translate, rotate..

#include "CollisionShape.h"
#include "InertiaShape.h"

/*
@author Kimmo Riihiaho, kimmo.riihiaho@gmail.com
@date 14.6.2018

PhysicalObject are controlled by PhysicalSimulation. Graphical
objects (Object3d) containing a PO can ask for model matrix determined by
simulation. 
*/
class PhysicalObject
{
	/*
	Let PhysicalSimulation to change PhysicalObject's 
	inner state.
	*/
	//friend class PhysicalSimulation;

public:
	/*
	Create new PO with default values: no forces or torque,
	no rotation and mass of 1 kg.
	*/
	PhysicalObject();

	~PhysicalObject() {
		delete m_collisionShape;
		delete m_inertiaShape;
	}
	
	/*
	Get model matrix for graphical object to use
	as it's own model matrix.
	*/
	glm::mat4 getModelMatrix();

	/*
	Setting the model matrix should be done when
	graphical object is bound to PO. After that,
	the simulation will control PO's model matrix.
	*/
	void setModelMatrix(glm::mat4 m);

	glm::mat3 getBodyInertia() {
		return m_inertiaShape->getInertiaTensor(m_mass);
	}

	glm::mat3 getBodyInertiaInv() {
		return m_inertiaShape->getInertiaTensorInv(m_mass);
	}

	/*
	Will always return CollisionShape that is not null.
	*/
	CollisionShape* getCollisionShape() {
		return m_collisionShape;
	}

	// PhysicalSimulation has great control over these
	// variables. Should make them private I think.

	/* State variables */
	// Position
	glm::vec3 pos;
	// Rotation matrix
	glm::mat3 R;
	// Linear momentum.
	glm::vec3 P;
	// Angular momentum
	glm::vec3 L;

	/* Derived quantities (auxiliary variables) */

	// Inverse of inertia matrix
	glm::mat3 Iinv;
	// Velocity
	glm::vec3 v;
	// Angular velocity
	glm::vec3 omega; 

	/* Computed quantities */
	glm::vec3 force; /* F(t) */
	// TODO make torque private after proper forces are in use.
	glm::vec3 torque; /* torque(t) */

	float getMass() {
		return m_mass;
	}

private:
	// Object's mass in kg.
	float m_mass;
	// Collision shape 
	CollisionShape * m_collisionShape;
	// Inertia shape is constant through the simulation.
	InertiaShape * m_inertiaShape;

};

