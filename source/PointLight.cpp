#include "PointLight.h"
#include "Shader.h"
#include "common.h"

int PointLight::lightCount = 0;

PointLight::PointLight() : Light()
{
	m_lightIndx = lightCount;
	m_position = glm::vec3(0.0f, 0.0f, 0.0f);
	m_constant = 1.0f;
	m_linear = 0.0f;
	m_exponent = 0.0f;
	m_cutoff = 0.0f;
	lightCount++;
}

PointLight::PointLight(	GLfloat red, GLfloat green, GLfloat blue,
						GLfloat aIntensity, GLfloat dIntensity,
						GLfloat xPos, GLfloat yPos, GLfloat zPos,
						GLfloat con, GLfloat lin, GLfloat exp, GLfloat cut)
	: Light(red, green, blue, aIntensity, dIntensity)
{
	m_lightIndx = lightCount;
	m_position = glm::vec3(xPos, yPos, zPos);
	m_constant = con;
	m_linear = lin;
	m_exponent = exp;
	m_cutoff = cut;
	lightCount++;
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
		p_shader->updateLightParams(this);
		//Light::UseLight(p_shader);
	}
}

PointLight::~PointLight()
{
	lightCount--;
}
