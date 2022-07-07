#pragma once

#include "Light.h"

class PointLight : public Light
{
public:
	PointLight();
	PointLight(GLfloat red, GLfloat green, GLfloat blue,
		GLfloat aIntensity, GLfloat dIntensity,
		GLfloat xPos, GLfloat yPos, GLfloat zPos,
		GLfloat con, GLfloat lin, GLfloat exp);

	void UseLight(GLuint ambientIntensityLocation, GLuint ambientColourLocation,
		GLuint diffuseIntensityLocation, GLuint positionLocation,
		GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation);

	void UseLight(Shader *p_shader);

	inline void setPosition(GLfloat x, GLfloat y, GLfloat z)
	{
		m_position = glm::vec3(x, y, z);
	}

	inline void setPosition(glm::vec3 position)
	{
		m_position = position;
	}

	inline void setAttenuation(GLfloat constant, GLfloat linear, GLfloat exponent)
	{
		m_constant = constant;
		m_linear = linear;
		m_exponent = exponent;
	}

	inline void setAttenuation(glm::vec3 attenuation)
	{
		m_constant = attenuation.x;
		m_linear = attenuation.y;
		m_exponent = attenuation.z;
	}

	inline glm::vec3 getPosition()
	{
		return m_position;
	}

	inline glm::vec3 getAttenuation()
	{
		return glm::vec3(m_constant, m_linear, m_exponent);
	}

	~PointLight();

protected:
	glm::vec3 m_position;

	GLfloat m_constant,
			m_linear,
			m_exponent;
};

