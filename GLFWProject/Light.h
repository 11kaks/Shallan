#pragma once

#include <glm\glm.hpp>

class Light
{
public:

	/*
	Create a new light with default values.
	*/
	Light() {
		m_position = m_defaultPosition;
		m_color = m_defaultColor;
		m_power = 60.0f;
	}
	~Light();

	void setPosition(glm::vec3 newPosition) {
		m_position = newPosition;
	}

	void setColor(glm::vec3 newColor) {
		m_color = newColor;
	}

	glm::vec3 getPosition() {
		return m_position;
	}

	glm::vec3 getColor() {
		return m_color;
	}

private:

	glm::vec3 m_defaultPosition = glm::vec3(0.0f, 4.0f, 5.0f);
	glm::vec3 m_defaultColor = glm::vec3(1.0f);
	glm::vec3 m_position;
	glm::vec3 m_color;
	float m_power;


};

