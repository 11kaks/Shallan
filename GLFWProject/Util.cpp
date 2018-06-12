#include "Util.h"


void Util::scaleVector(std::vector<float> &vector, float scale) {

	for(std::size_t i = 0; i < vector.size(); ++i) {
		vector[i] *= scale;
	}
}


void Util::scaledVector(const std::vector<float> &src, float scale, std::vector<float> &dst) {

	if(src.size() != dst.size()) {
		std::cerr << __func__ << "() Source and destination vectors differs in size." << std::endl;
	}

	for (std::size_t i = 0; i < src.size(); ++i) {
		dst[i] = src[i] * scale;
	}
}

void Util::addVectors(const std::vector<float> &f1, const std::vector<float> &f2, std::vector<float> &res) {
	
	if(f1.size() != f2.size() || f2.size() != res.size()) {
		std::cout << __func__ << "() Trying to add vectors with different dimensions. " << std::endl;
	}
	for(std::size_t i = 0; i < f1.size(); ++i) {
		res[i] = f2[i] + f1[i];
	}
}
