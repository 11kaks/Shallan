#pragma once

#include <vector>

class SimulationBase {
public:
	virtual void getState(std::vector<float> &dst) = 0;
	virtual void setState(std::vector<float> &src) = 0;
	virtual void derivate(std::vector<float> &dst) = 0;
	virtual std::size_t getDim() = 0;

};
