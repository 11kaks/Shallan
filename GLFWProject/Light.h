#pragma once

#include <glm\glm.hpp>

class Light
{
public:

	Light() {
		m_position = m_defaultPosition;
		m_color = m_defaultColor;
		m_power = 60.0f;
	}
	~Light();

private:

	glm::vec3 m_defaultPosition = glm::vec3(0.0f, 4.0f, 5.0f);
	glm::vec3 m_defaultColor = glm::vec3(1.0f);
	glm::vec3 m_position;
	glm::vec3 m_color;
	float m_power;


};

