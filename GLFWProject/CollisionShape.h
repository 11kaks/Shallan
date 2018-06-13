#pragma once

#include <vector>
#include "glm/glm.hpp"

class CollisionShape
{
public:

	/*
	Get greatest value of object in x-axis.
	*/
	virtual float getMaxX() = 0;

	/*
	Get smallest value of object in x-axis.
	*/
	virtual float getMinX() = 0;

	/*
	Get greatest value of object in y-axis.
	*/
	virtual float getMaxY() = 0;

	/*
	Get smallest value of object in y-axis.
	*/
	virtual float getMinY() = 0;

	/*
	Get greatest value of object in z-axis.
	*/
	virtual float getMaxZ() = 0;
	/*
	Get smallest value of object in z-axis.
	*/
	virtual float getMinZ() = 0;

	/*
	Get x-axis aligned dimension (width of the shape).
	*/
	virtual float getDimX() = 0;

	/*
	Get y-axis aligned dimension (height of the shape).
	*/
	virtual float getDimY() = 0;

	/*
	Get z-axis aligned dimension (depth of the shape).
	*/
	virtual float getDimZ() = 0;

};

/*
Origosymmetric box can be represented with a single 3d vector.
*/
class CollisionBoxOrigoSymmetric : public CollisionShape
{
public:
	/*
	Sets the defining corner point. Corner point will always be
	defined in the positive octant of space e.g. (-1, 1, 0) -> (1, 1, 0).
	*/
	CollisionBoxOrigoSymmetric(glm::vec3 corner) {
		m_corner = glm::vec3(abs(corner[0]), abs(corner[1]), abs(corner[2]));
	}

	~CollisionBoxOrigoSymmetric() {}

	float getMaxX() {
		return m_corner[0];
	}
	float getMinX() {
		return -m_corner[0];
	}
	float getMaxY() {
		return m_corner[1];
	}
	float getMinY() {
		return -m_corner[1];
	}
	float getMaxZ() {
		return m_corner[2];
	}
	float getMinZ() {
		return -m_corner[2];
	}

	/*
	Get x-axis aligned dimension (width of the box).
	*/
	float getDimX() {
		return 2 * m_corner[0];
	}

	/*
	Get y-axis aligned dimension (height of the box).
	*/
	float getDimY() {
		return 2 * m_corner[1];
	}

	/*
	Get z-axis aligned dimension (depth of the box).
	*/
	float getDimZ() {
		return 2 * m_corner[2];
	}

private:
	glm::vec3 m_corner;
};

class CollisionBox : public CollisionShape
{
public:
	CollisionBox() {}
	~CollisionBox() {}
private:
};

class CollisionSphere : public CollisionShape
{
public:
	CollisionSphere() {}
	~CollisionSphere() {}
};

