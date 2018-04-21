#pragma once

#include <iostream>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:

	Camera(glm::vec3 position, glm::vec3 direction, glm::vec3 up, float fieldOfView, int windowWidth, int windowHeight) {
		m_position = position;
		m_direction = direction;
		m_up = up;
		m_windowWidth = windowWidth;
		m_windowHeight = windowHeight;
		m_fov = fieldOfView;
		updateMatrices();
	}
	~Camera();

	void zoom(float deltaX, float deltaY);
	void rotate(float deltaX, float deltaY);
	void move(float deltaX, float deltaY);

	glm::mat4 getViewMatrix() {
		// Camera matrix
		return m_viewMatrix;
	}
	glm::mat4 getProjectionMatrix() {
		return m_projectionMatrix;
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

	int m_windowWidth;
	int m_windowHeight;
	// Camera field of view.
	float m_fov;
	float m_nearClip = 0.1f;
	float m_farClip = 100.0f;

	void updateMatrices() {
		m_projectionMatrix = glm::perspective(glm::radians(m_fov), float(m_windowWidth) / float(m_windowHeight), m_nearClip, m_farClip);

		m_viewMatrix = glm::lookAt(
			m_position,           // Camera is here
			m_position + m_direction, // and looks here : at the same position, plus "direction"
			m_up                  // Head is up (set to 0,-1,0 to look upside-down)
		);
	}
};

