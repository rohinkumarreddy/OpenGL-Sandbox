#include "DirectionalLight.h"
#include "Shader.h"
#include "common.h"

DirectionalLight::DirectionalLight() : Light()
{
	m_direction = glm::vec3(0.0f, -1.0f, 0.0f);
}

DirectionalLight::DirectionalLight(	GLfloat red,
									GLfloat green,
									GLfloat blue,
									GLfloat aIntensity,
									GLfloat dIntensity,
									GLfloat xDir,
									GLfloat yDir,
									GLfloat zDir)
	: Light(red, green, blue, aIntensity, dIntensity)
{
	m_direction = glm::vec3(xDir, yDir, zDir);
}

void DirectionalLight::UseLight(	GLfloat ambientIntensityLocation,
									GLfloat ambientColourLocation,
									GLfloat diffuseIntensityLocation,
									GLfloat directionLocation)
{
	GLCALL(glUniform3f(directionLocation, m_direction.x, m_direction.y, m_direction.z));
	Light::UseLight(	ambientIntensityLocation,
						ambientColourLocation,
						diffuseIntensityLocation	);
}

void DirectionalLight::UseLight(Shader* p_shader)
{
	if (p_shader != nullptr)
	{
		p_shader->updateLightParams(this);
		//Light::UseLight(p_shader);
	}
}

DirectionalLight::~DirectionalLight()
{
}
