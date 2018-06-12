#pragma once

#include "PhysicalSimulation.h"

class Ode
{
public:

	static void solve(float t0, float t1, PhysicalSimulation *ps) {
		float h = t1 - t0;
		bool adaptive = false;

		if(adaptive) {
			/*float maxE = 0.1f;

			// Original state
			std::vector<float> x0(ps->getDim());
			ps->getState(x0);

			std::vector<float> firstHalfStep(ps->getDim());
			takeIntermediateStep(ps, m_oldStep / 2.f, firstHalfStep);
			ps->setState(firstHalfStep);
			std::vector<float> secondHalfStep(ps->getDim());
			takeIntermediateStep(ps, m_oldStep / 2.f, secondHalfStep);

			// Reset system to original state.
			ps->setState(x0);

			std::vector<float> fullStep(ps->getDim());
			takeIntermediateStep(ps, m_oldStep, fullStep);

			float e = Util::maxDistance(secondHalfStep, fullStep);

			//std::cout << m_oldStep << std::endl;

			m_oldStep = sqrt((maxE / e)) * m_oldStep;

			// Set final state.
			ps->setState(secondHalfStep);*/
		} else {
			std::vector<float> state(ps->getDim());
			Ode::takeIntermediateStep(ps, h, state);
			ps->setState(state);
		}
	}

	/*
	Take runge kutta 4 step and return resulting state. Simulation state
	is not changed.
	*/
	static void takeIntermediateStep(PhysicalSimulation* ps, float stepSize, std::vector<float> &state) {

		// Store original state for later
		std::vector<float> x0(ps->getDim());
		ps->getState(x0);

		// Derivative of the original state scaled by step size.
		std::vector<float> k1(ps->getDim());
		ps->particleDerivative(k1);
		Util::scaleVector(k1, stepSize);

		// Set new intermediate state x0 + k1/2
		std::vector<float> halfk1PlusX0(ps->getDim());
		Util::scaledVector(k1, 0.5f, halfk1PlusX0);
		Util::addVectors(x0, halfk1PlusX0, halfk1PlusX0);
		ps->setState(halfk1PlusX0);

		// Derivative at f(x0 + k1/2) scaled by step size.
		std::vector<float> k2(ps->getDim());
		ps->particleDerivative(k2);
		Util::scaleVector(k2, stepSize);

		// Set new intermediate state x0 + k2/2
		std::vector<float> halfk2PlusX0(ps->getDim());
		Util::scaledVector(k2, 0.5f, halfk2PlusX0);
		Util::addVectors(x0, halfk2PlusX0, halfk2PlusX0);
		ps->setState(halfk2PlusX0);

		// Derivative at f(x0 + k2/2) scaled by step size.
		std::vector<float> k3(ps->getDim());
		ps->particleDerivative(k3);
		Util::scaleVector(k3, stepSize);

		// Set new intermediate state x0 + k3
		std::vector<float> k3PlusX0(ps->getDim());
		Util::addVectors(x0, k3, k3PlusX0);
		ps->setState(k3PlusX0);

		// Derivative at f(x0 + k3) scaled by step size.
		std::vector<float> k4(ps->getDim());
		ps->particleDerivative(k4);
		Util::scaleVector(k4, stepSize);

		// Scale and add x0 and k1-k4
		Util::scaleVector(k1, 1.f / 6.f);
		Util::scaleVector(k2, 1.f / 3.f);
		Util::scaleVector(k3, 1.f / 3.f);
		Util::scaleVector(k4, 1.f / 6.f);

		//vector<float> finalState(ps->getDim());
		Util::addVectors(x0, k1, state);
		Util::addVectors(state, k2, state);
		Util::addVectors(state, k3, state);
		Util::addVectors(state, k4, state);

		// Reset system to original state.
		ps->setState(x0);
	}


};

