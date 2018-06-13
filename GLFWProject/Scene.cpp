#include "Scene.h"

Scene::Scene() {
	m_currentTime = 0.0;
	m_lastTime = 0.0;
	m_physSim = new PhysicalSimulation();
}

void Scene::draw() {

	
	// hax to keep the simulation from exploding before nothing can be seen 
	double slowDown = 10.0;
	//m_lastTime = m_currentTime;
	//m_currentTime += 0.0001;

	// FIXME solver should be called from simulation like this
	// m_physSim->stepSimulation(m_lastTime, m_currentTime);
	Ode::solve(m_lastTime / slowDown, m_currentTime / slowDown, m_physSim);
	
	glm::mat4 V = m_camera->getViewMatrix();
	glm::mat4 P = m_camera->getProjectionMatrix();

	for(std::size_t i = 0; i < m_objectList.size(); ++i) {
		Object3D * drawable = m_objectList.at(i);
		drawable->setViewMatrix(V);
		drawable->setProjectionMatrix(P);
		drawable->draw();
		drawable->setCameraPosition(m_camera->getPositionWorldSpace());
	}
}

void Scene::toggleVisibility(int index) {
	try {
		Object3D * o = m_objectList.at(index);
		o->isVisible(!o->isVisible());
	} catch(...) {
		std::cerr << "Trying to toggle visibility of object #" << index + 1 << ", but it was not found." << std::endl;
	}
}

Scene::~Scene() {
	delete m_physSim;
}
