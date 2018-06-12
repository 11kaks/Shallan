#include "Scene.h"

Scene::Scene() {
	m_time = 0.0f;
	m_physSim = new PhysicalSimulation();
}

void Scene::draw() {

	// Simulation
	float oldTime = m_time;
	m_time += 0.0002f;
	Ode::solve(oldTime, m_time, m_physSim);
	//m_physSim->ode(oldTime, m_time);

	glm::mat4 M = m_physSim->m_physicalObjects[0]->getModelMatrix();

	// Simulation end

	glm::mat4 V = m_camera->getViewMatrix();
	glm::mat4 P = m_camera->getProjectionMatrix();

	for(std::size_t i = 0; i < m_objectList.size(); ++i) {
		Object3D * drawable = m_objectList.at(i);
		drawable->setModelMatrix(M);
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
