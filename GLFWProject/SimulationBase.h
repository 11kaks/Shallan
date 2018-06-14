#pragma once

#include <vector>
/*
@author Kimmo Riihiaho, kimmo.riihiaho@gmail.com
@date 14.6.2018

SimulationBase interface offers the few methods our Ode
solver requires.
*/
class SimulationBase {
public:
	/*
	Get state of all the objects in simulation as a flat list.
	The state is set to dst vector.

	@param dst must be allocated to proper legth (use getDim())
			   before using.
	*/
	virtual void getState(std::vector<float> &dst) = 0;
	/*
	Set state of all the objects in simulation from a flat list.
	*/
	virtual void setState(std::vector<float> &src) = 0;
	/*
	Apply forces and torque, then return the time derivative
	of the system in vector dst.

	@param dst must be allocated to proper legth (use getDim())
			   before using.
	*/
	virtual void derivate(std::vector<float> &dst) = 0;
	/*
	Returns length of the list needed to store the state
	of all objects in the system.
	*/
	virtual std::size_t getDim() = 0;
};
