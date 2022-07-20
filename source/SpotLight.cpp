#include "SpotLight.h"
#include "Shader.h"
#include "common.h"

int SpotLight::lightCount = 0;

SpotLight::SpotLight() : PointLight(true)
{
	m_lightIndx = lightCount;
	m_direction = glm::vec3(0.0f, -1.0f, 0.0f);
	m_edge = 0.0f;
	m_procEdge = cosf(glm::radians(m_edge));
	lightCount++;
}

SpotLight::SpotLight(	GLfloat red, GLfloat green, GLfloat blue,
						GLfloat aIntensity, GLfloat dIntensity,
						GLfloat xPos, GLfloat yPos, GLfloat zPos,
						GLfloat xDir, GLfloat yDir, GLfloat zDir,
						GLfloat con, GLfloat lin, GLfloat exp,
						GLfloat cut, GLfloat edge	)
	: PointLight(	red, green, blue,
					aIntensity, dIntensity,
					xPos, yPos, zPos,
					con, lin, exp, cut, true	)
{
	m_lightIndx = lightCount;
	m_direction = glm::normalize(glm::vec3(xDir, yDir, zDir));
	m_edge = edge;
	m_procEdge = cosf(glm::radians(m_edge));
	lightCount++;
}

void SpotLight::UseLight(	GLuint ambientIntensityLocation,
							GLuint ambientColourLocation,
							GLuint diffuseIntensityLocation,
							GLuint positionLocation,
							GLuint directionLocation,
							GLuint constantLocation,
							GLuint linearLocation,
							GLuint exponentLocation,
							GLuint edgeLocation	)
{
	GLCALL(glUniform3f(positionLocation,	m_position.x,
											m_position.y,
											m_position.z));

	GLCALL(glUniform1f(constantLocation,	m_constant));
	GLCALL(glUniform1f(linearLocation,		m_linear));
	GLCALL(glUniform1f(exponentLocation,	m_exponent));

	GLCALL(glUniform3f(directionLocation,	m_direction.x,
											m_direction.y,
											m_direction.z));

	GLCALL(glUniform1f(edgeLocation,		m_procEdge));
	Light::UseLight(	ambientIntensityLocation,
						ambientColourLocation,
						diffuseIntensityLocation	);
}

void SpotLight::UseLight(Shader* p_shader)
{
	if (p_shader != nullptr)
	{
		p_shader->updateLightParams(this);
		//Light::UseLight(p_shader);
	}
}

void SpotLight::SetFlash(glm::vec3 pos, glm::vec3 dir)
{
	m_position = pos;
	m_direction = dir;
}

SpotLight::~SpotLight()
{
	lightCount--;
}
