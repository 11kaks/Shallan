#include "Camera.h"

void Camera::zoom(float deltaX, float deltaY) {
	m_position -= m_direction * deltaX;
	m_position += m_direction * deltaY;
	updateMatrices();
}

void Camera::rotate(float deltaX, float deltaY) {
	// Compute new orientation
	m_horizontalAngle += deltaX;
	m_verticalAngle += deltaY;
	calculateDirectionFromAngles();
	updateMatrices();
}

void Camera::calculateDirectionFromAngles() {

	// Right vector
	m_right = glm::vec3(
		sin(m_horizontalAngle - 3.14f / 2.0f),
		0,
		cos(m_horizontalAngle - 3.14f / 2.0f)
	);

	// Direction : Spherical coordinates to Cartesian coordinates conversion
	m_direction = glm::vec3(
		cos(m_verticalAngle) * sin(m_horizontalAngle),
		sin(m_verticalAngle),
		cos(m_verticalAngle) * cos(m_horizontalAngle)
	);

	// Up vector
	m_up = glm::cross(m_right, m_direction);
}

void Camera::move(float deltaX, float deltaY) {
	m_position += m_right * deltaX;
	m_position -= m_up * deltaY;
	updateMatrices();
}

void Camera::updateMatrices() {

	m_projectionMatrix = glm::perspective(glm::radians(m_fov), float(m_windowWidth) / float(m_windowHeight), m_nearClip, m_farClip);

	m_viewMatrix = glm::lookAt(
		m_position,           // Camera position
		m_position + m_direction, // Looking direction
		m_up                  // Up direction 
	);
}

Camera::~Camera() {
}
