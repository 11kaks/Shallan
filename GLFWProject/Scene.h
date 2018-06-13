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
	Applies default physics to given object3d.
	The PhysicalObject given to Object3d is added
	to PhysicalSimulation.
	*/
	void applyPhysics(Object3D * object) {
		PhysicalObject * po = new PhysicalObject();
		object->setPhysicalObject(po);
		m_physSim->addPhysicalObject(po);
	}

	/*
	should be able to remove physics. How to 
	remove the PhysicalObject from PhysicalSimulation? 
	Would need to search through the list.
	void removePhysics(Object3D * object) {

	}*/

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
	Increase time of physical simulation. This should 
	be done after rendering each frame, so that
	the simulation can adapt to framerate. 

	@param time change in seconds
	*/
	void increaseTime(double deltaTime) {
		m_lastTime = m_currentTime;
		m_currentTime += deltaTime;
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
	double m_currentTime;
	// Time of previous physicas simulation time in seconds
	double m_lastTime;
};

