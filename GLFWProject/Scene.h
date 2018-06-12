#pragma once

#include <vector>

#include "Object3D.h"
#include "Camera.h"
#include "Light.h"
#include "PhysicalSimulation.h"
#include "Ode.h"

/*
Scene encapsulates drawable objects, lights and the camera.
All changes to these objects should be done through scene object.

In the future, it would be better if encapsulated objects were not open 
byu getters.
*/
class Scene
{
public:

	Scene();
	~Scene();

	/*
	Adds given object to the scene.
	*/
	void addObject(Object3D * object) {
		m_objectList.push_back(object);
	}

	/*
	Provide all objects with camera's matrices and call for
	object draw method. Lights are not used at the moment.
	*/
	void draw();

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

	/*
	Reload shaders for all objects in the scene.
	*/
	void reloadShaders() {
		for(std::size_t i = 0; i < m_objectList.size(); ++i) {
			Object3D * drawable = m_objectList.at(i);
			drawable->reloadShaders();
		}
	}

	/*
	Toggle visibility of object in given index 0->
	*/
	void toggleVisibility(int index);

private:
	std::vector<Object3D*> m_objectList = std::vector<Object3D*>();
	Camera * m_camera;
	Light * m_light;

	PhysicalSimulation * m_physSim;

	// Simulation time in seconds
	float m_time;
};

