#pragma once

#include "Light.h"

class PointLight : public Light
{
public:
	PointLight(bool skipIndx = false);
	PointLight(GLfloat red, GLfloat green, GLfloat blue,
		GLfloat aIntensity, GLfloat dIntensity,
		GLfloat xPos, GLfloat yPos, GLfloat zPos,
		GLfloat con, GLfloat lin, GLfloat exp, GLfloat cut, bool skipIndx = false);

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

	inline void setAttenuation( GLfloat constant, GLfloat linear,
								GLfloat exponent, GLfloat cut)
	{
		m_constant = constant;
		m_linear = linear;
		m_exponent = exponent;
		m_cutoff = cut;
	}

	inline void setAttenuation(glm::vec4 attenuation)
	{
		m_constant = attenuation.x;
		m_linear = attenuation.y;
		m_exponent = attenuation.z;
		m_cutoff = attenuation.w;
	}

	inline glm::vec3 getPosition()
	{
		return m_position;
	}

	inline glm::vec4 getAttenuation()
	{
		return glm::vec4(m_constant, m_linear, m_exponent, m_cutoff);
	}

	inline GLfloat getConstant()
	{
		return m_constant;
	}

	inline GLfloat getLinear()
	{
		return m_linear;
	}

	inline GLfloat getExponent()
	{
		return m_exponent;
	}

	inline GLfloat getCutOff()
	{
		return m_cutoff;
	}

	inline const int getLightIndex()
	{
		return m_lightIndx;
	}

	~PointLight();

private:
	static int lightCount;

public:
	static int getLightCount()
	{
		return lightCount;
	}

protected:
	glm::vec3 m_position;

	GLfloat m_constant,
			m_linear,
			m_exponent,
			m_cutoff;
private:
	int m_lightIndx;
	bool m_SkipIndx;
};