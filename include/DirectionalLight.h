#pragma once

#include "Light.h"

class DirectionalLight :
	public Light
{
public:
	DirectionalLight();
	DirectionalLight(GLfloat red, GLfloat green, GLfloat blue,
		GLfloat aIntensity, GLfloat dIntensity,
		GLfloat xDir, GLfloat yDir, GLfloat zDir);

	void UseLight(GLfloat ambientIntensityLocation, GLfloat ambientColourLocation,
		GLfloat diffuseIntensityLocation, GLfloat directionLocation);
	
	void UseLight(Shader* p_shader);

	inline void setDirection(GLfloat x, GLfloat y, GLfloat z)
	{
		m_direction = glm::vec3(x, y, z);
	}

	inline void setDirection(glm::vec3 direction)
	{
		m_direction = direction;
	}

	inline glm::vec3 getDirection()
	{
		return m_direction;
	}

	~DirectionalLight();

private:
	glm::vec3 m_direction;
};

