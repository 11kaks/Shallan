#pragma once

#include <vector>

#include "Object3D.h"
#include "Camera.h"
#include "Light.h"


class Scene
{
public:

	Scene();
	~Scene();

	void addObject(Object3D * object) {
		m_objectList.push_back(object);
	}

	void draw() {

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

	void setCamera(Camera *camera) {
		m_camera = camera;
	}


	void setLight(Light * light) {
		m_light = light;
	}

	Light * getLight() {
		return m_light;
	}

	Camera * getCamera() {
		return m_camera;
	}

	void reloadShaders() {
		for(std::size_t i = 0; i < m_objectList.size(); ++i) {
			Object3D * drawable = m_objectList.at(i);
			drawable->reloadShaders();
		}
	}

private:

	std::vector<Object3D*> m_objectList = std::vector<Object3D*>();
	Camera * m_camera;
	Light * m_light;
};
