#include "Scene.h"

Scene::Scene() {
}

void Scene::draw() {

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

Scene::~Scene() {
}
