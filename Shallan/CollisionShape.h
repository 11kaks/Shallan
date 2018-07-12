#pragma once

#include <vector>
#include "glm/glm.hpp"

/*
@author Kimmo Riihiaho, kimmo.riihiaho@gmail.com
@date 14.6.2018

CollisionShape determines the physical shape of the model.

*/
class CollisionShape
{
public:

	/*
	Get greatest value of object in x-axis.
	*/
	virtual float getMaxX() {
		return maxX;
	}

	/*
	Get smallest value of object in x-axis.
	*/
	virtual float getMinX() {
		return minX;
	}

	/*
	Get greatest value of object in y-axis.
	*/
	virtual float getMaxY() {
		return maxY;
	}

	/*
	Get smallest value of object in y-axis.
	*/
	virtual float getMinY() {
		return minY;
	}

	/*
	Get greatest value of object in z-axis.
	*/
	virtual float getMaxZ() {
		return maxZ;
	}

	/*
	Get smallest value of object in z-axis.
	*/
	virtual float getMinZ() {
		return minZ;
	}

	/*
	Corner points for drawing.
	Draw lines:
	0-1, 1-2, 2-3, 3-0, 4-5, 5-6, 6-7, 7-4, 0-4, 1-5, 2-6 and 3-7
	*/
	std::vector<glm::vec4> getCornerPointList() {
		std::vector<glm::vec4> res(8);

		res[0] = glm::vec4(minX, minY, minZ, 1.0f);
		res[1] = glm::vec4(maxX, minY, minZ, 1.0f);
		res[2] = glm::vec4(maxX, minY, maxZ, 1.0f);
		res[3] = glm::vec4(minX, minY, maxZ, 1.0f);
		res[4] = glm::vec4(minX, maxY, minZ, 1.0f);
		res[5] = glm::vec4(maxX, maxY, minZ, 1.0f);
		res[6] = glm::vec4(maxX, maxY, maxZ, 1.0f);
		res[7] = glm::vec4(minX, maxY, maxZ, 1.0f);

		return res;
	}

	/*
	Returns an int array for drawing. Draw loop 0-3 and
	4-7, and lines for the rest.
	*/
	std::vector<unsigned> getCornerDrawOrder() {
		std::vector<unsigned> elements = {
			0, 1, 2, 3,
			4, 5, 6, 7,
			0, 4, 1, 5, 2, 6, 3, 7
		};
		return elements;
	}


	void setCorner(glm::vec3 corner) {
		m_corner = glm::vec3(abs(corner[0]), abs(corner[1]), abs(corner[2]));
		minX = -m_corner[0];
		maxX = m_corner[0];
		minY = -m_corner[1];
		maxY = m_corner[1];
		minZ = -m_corner[2];
		maxZ = m_corner[2];
	}


	/*
	Get x-axis aligned dimension (width of the shape).
	*/
	//virtual float getDimX() = 0;

	/*
	Get y-axis aligned dimension (height of the shape).
	*/
	//virtual float getDimY() = 0;

	/*
	Get z-axis aligned dimension (depth of the shape).
	*/
	//virtual float getDimZ() = 0;

protected:
	float minX;
	float maxX;
	float minY;
	float maxY;
	float minZ;
	float maxZ;

	glm::vec3 m_corner;
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
		setCorner(corner);
	}


	~CollisionBoxOrigoSymmetric() {}


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

/*
General collision box. Unfinished. Maby unnecessary.
*/
class CollisionBox : public CollisionShape
{
public:
	CollisionBox() {}
	~CollisionBox() {}
private:
};

/*
General collision box. Unfinished.
*/
class CollisionSphere : public CollisionShape
{
public:
	CollisionSphere() {}
	~CollisionSphere() {}
};

