#pragma once

#include <GL\glew.h>
#include <glm\glm.hpp>

//forward declaration
class Shader;

class Light
{
public:
	Light();
	Light(GLfloat red, GLfloat green, GLfloat blue,
		GLfloat aIntensity, GLfloat dIntensity);

	~Light();

	inline void setColor(GLfloat red, GLfloat green, GLfloat blue)
	{
		m_color = glm::vec3(red, green, blue);
	}

	inline void setColor(glm::vec3 color)
	{
		m_color = color;
	}

	inline void setIntensity(GLfloat ambient, GLfloat diffusion)
	{
		m_ambientIntensity = ambient;
		m_diffuseIntensity = diffusion;
	}

	inline glm::vec3 getColor()
	{
		return m_color;
	}

	inline GLfloat getAmbientIntensity()
	{
		return m_ambientIntensity;
	}

	inline GLfloat getDiffusionIntensity()
	{
		return m_diffuseIntensity;
	}

protected:
	glm::vec3 m_color;
	GLfloat m_ambientIntensity;
	GLfloat m_diffuseIntensity;
	void UseLight(	GLuint ambientIntensityLocation,
					GLuint ambientColourLocation,
					GLuint diffuseIntensityLocation	);
	//void UseLight(Shader* p_shader);
};

