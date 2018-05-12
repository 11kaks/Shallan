#pragma once

#include <iostream>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:

	Camera(glm::vec3 position, float vertAngle, float horAngle, glm::vec3 up, float fieldOfView, int windowWidth, int windowHeight) {
		m_position = position;
		calculateDirectionFromAngles();
		m_up = up;
		m_windowWidth = windowWidth;
		m_windowHeight = windowHeight;
		m_fov = fieldOfView;
		updateMatrices();
	}

	~Camera();
	/*
	Zoom by moving the camera along vieweing axis.
	*/
	void zoom(float deltaX, float deltaY);
	/*
	Rotate the camera around viewing axis.
	*/
	void rotate(float deltaX, float deltaY);
	/*
	Move the camera on view plane.
	*/
	void move(float deltaX, float deltaY);

	glm::mat4 getViewMatrix() {
		return m_viewMatrix;
	}
	glm::mat4 getProjectionMatrix() {
		return m_projectionMatrix;
	}
	glm::vec3 getPositionWorldSpace() {
		return m_position;
	}

private:
	// Camera position.
	glm::vec3 m_position;
	// Camera viewing direction.
	glm::vec3 m_direction;
	// Camera up direction.
	glm::vec3 m_up;
	// Camera right direction (horizont).
	glm::vec3 m_right;
	// Current horizontal angle
	float m_horizontalAngle = 3.14f;
	// Current vertical angle
	float m_verticalAngle = -0.4f;
	glm::mat4 m_viewMatrix;
	glm::mat4 m_projectionMatrix;
	// Window width (for aspect ratio).
	int m_windowWidth;
	// Window height (for aspect ratio).
	int m_windowHeight;
	// Camera field of view.
	float m_fov = 45.0f;
	// Near clipping distance.
	float m_nearClip = 0.1f;
	// Far clipping distance.
	float m_farClip = 100.0f;

	/*
	Update view and projection matrix.
	*/
	void updateMatrices();

	/*
	Calculates new direction, right and up vectors.
	
	Take care to update m_horizontalAngle and m_verticalAngle
	before calling this.
	*/
	void calculateDirectionFromAngles();
};

