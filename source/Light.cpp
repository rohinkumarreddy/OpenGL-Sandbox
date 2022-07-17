#include "Light.h"
#include "Shader.h"
#include "common.h"

Light::Light()
{
	m_color = glm::vec3(1.0f, 1.0f, 1.0f);
	m_ambientIntensity = 1.0f;
	m_diffuseIntensity = 1.0f;
}

Light::Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat dIntensity)
{
	m_color = glm::vec3(red, green, blue);
	m_ambientIntensity = aIntensity;
	m_diffuseIntensity = dIntensity;
}

void Light::UseLight(	GLuint ambientIntensityLocation,
						GLuint ambientColourLocation,
						GLuint diffuseIntensityLocation		)
{
	GLCALL(glUniform3f(ambientColourLocation, m_color.x, m_color.y, m_color.z));
	GLCALL(glUniform1f(ambientIntensityLocation, m_ambientIntensity));
	GLCALL(glUniform1f(diffuseIntensityLocation, m_diffuseIntensity));
}

//void Light::UseLight(Shader* p_shader)
//{
//	if (p_shader != nullptr)
//	{
//		/*p_shader->setUniform("u_ambientLight", glm::vec4(	m_ambientIntensity,
//															m_ambientIntensity,
//															m_ambientIntensity,
//															m_ambientIntensity	));
//		p_shader->setUniform("u_diffusedLight", glm::vec4(	m_diffuseIntensity,
//															m_diffuseIntensity,
//															m_diffuseIntensity,
//															m_diffuseIntensity	));*/
//	}
//}

Light::~Light()
{
}