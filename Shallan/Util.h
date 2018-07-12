#pragma once

#include <iostream>
#include <vector>
#include <limits>
#include <glm/glm.hpp>

class Util
{
public:

	static void scaleVector(std::vector<float> &vector, float scale);
	static void scaledVector(const std::vector<float> &src, float scale, std::vector<float> &dst);

	/*
	Add res = f1 + f2
	*/
	static void addVectors(const std::vector<float> &f1, const std::vector<float> &f2, std::vector<float> &res);

	/*
	Return biggest float in given list.
	*/
	static float max(const std::vector<float> vec) {
		float biggest = std::numeric_limits<float>::min();
		for(std::size_t i = 0; i < vec.size(); ++i) {
			if(vec[i] > biggest) {
				biggest = vec[i];
			}
		}
		return biggest;
	}

	/*
	Return smallest float in given list.
	*/
	static float min(const std::vector<float> vec) {
		float smallest = std::numeric_limits<float>::max();
		for(std::size_t i = 0; i < vec.size(); ++i) {
			if(vec[i] < smallest) {
				smallest = vec[i];
			}
		}
		return smallest;
	}

	/*
	Find biggest pairwise distance of two float vectors.
	This is propably stupid.
	*/
	static float maxDistance(const std::vector<float> &vec1, const std::vector<float> &vec2) {
		float res = 0.0f;

		if(vec1.size() != vec2.size()) {
			std::cerr << __func__ << "() Vectors must be of same length for comparison." << std::endl;
			throw(1);
		}

		for(std::size_t i = 2; i < vec1.size(); i += 6) {
			float x1 = vec1[i - 2];
			float y1 = vec1[i - 1];
			float z1 = vec1[i];
			float x2 = vec2[i - 2];
			float y2 = vec2[i - 1];
			float z2 = vec2[i];

			float dist = sqrt((x1-x2)*(x1 - x2) + (y1 - y2)*(y1 - y2) + (z1 - z2)*(z1 - z2) );
			if(dist > res) {
				res = dist;
			}
		}
		return res;
	}

	/*
	Returns the longest vec3 in src vector.
	*/
	static glm::vec3 maxLength(const std::vector<glm::vec3> &src) {
		glm::vec3 res(0.0f);
		for(size_t i = 0; i < src.size(); i++) {
			if(glm::length(src[i]) > glm::length(res)) {
				res = src[i];
			}
		}
		return res;
	}

	/*
	Finds a maximum x, y and z coordinates individually
	and returns a vec3 that has these maximums. Can be used
	to draw a tightes axixs-aligned bounding box possible 
	that encompasses the whole object. Assumes that the 
	object is centered at origo.
	*/
	static glm::vec3 maxXYZ(const std::vector<glm::vec3> &src) {

		float maxX = 0.0f;
		float maxY = 0.0f;
		float maxZ = 0.0f;

		for(size_t i = 0; i < src.size(); i++) {
			glm::vec3 v = src[i];
			float x = abs(v[0]);
			float y = abs(v[1]);
			float z = abs(v[2]);

			if(x > maxX) {
				maxX = x;
			}
			if(y > maxY) {
				maxY = y;
			}
			if(z > maxZ) {
				maxZ = z;
			}
		}
		return glm::vec3(maxX, maxY, maxZ);
	}
};

