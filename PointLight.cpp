#include "PointLight.h"
#include "Shader.h"
#include "common.h"

PointLight::PointLight() : Light()
{
	m_position = glm::vec3(0.0f, 0.0f, 0.0f);
	m_constant = 1.0f;
	m_linear = 0.0f;
	m_exponent = 0.0f;
}

PointLight::PointLight(	GLfloat red, GLfloat green, GLfloat blue,
						GLfloat aIntensity, GLfloat dIntensity,
						GLfloat xPos, GLfloat yPos, GLfloat zPos,
						GLfloat con, GLfloat lin, GLfloat exp)
	: Light(red, green, blue, aIntensity, dIntensity)
{
	m_position = glm::vec3(xPos, yPos, zPos);
	m_constant = con;
	m_linear = lin;
	m_exponent = exp;
}

void PointLight::UseLight(	GLuint ambientIntensityLocation,
							GLuint ambientColourLocation,
							GLuint diffuseIntensityLocation,
							GLuint positionLocation,
							GLuint constantLocation,
							GLuint linearLocation,
							GLuint exponentLocation)
{
	GLCALL(glUniform3f(positionLocation, m_position.x, m_position.y, m_position.z));
	GLCALL(glUniform1f(constantLocation, m_constant));
	GLCALL(glUniform1f(linearLocation, m_linear));
	GLCALL(glUniform1f(exponentLocation, m_exponent));
	Light::UseLight(	ambientIntensityLocation,
						ambientColourLocation,
						diffuseIntensityLocation	);
}

void PointLight::UseLight(Shader* p_shader)
{
	if (p_shader != nullptr)
	{
		p_shader->setUniform("u_lightPos", m_position);
		p_shader->setUniform("u_lAttenuationFac", glm::vec4(m_constant,
															m_linear,
															m_exponent,
															0));
		Light::UseLight(p_shader);
	}
}

PointLight::~PointLight()
{
}
