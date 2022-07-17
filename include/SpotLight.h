#pragma once

#include "PointLight.h"

class SpotLight :
	public PointLight
{
public:
	SpotLight();

	SpotLight(	GLfloat red, GLfloat green, GLfloat blue,
				GLfloat aIntensity, GLfloat dIntensity,
				GLfloat xPos, GLfloat yPos, GLfloat zPos,
				GLfloat xDir, GLfloat yDir, GLfloat zDir,
				GLfloat con, GLfloat lin, GLfloat exp,
				GLfloat cut, GLfloat edg	);

	void UseLight(	GLuint ambientIntensityLocation,
					GLuint ambientColourLocation,
					GLuint diffuseIntensityLocation,
					GLuint positionLocation,
					GLuint directionLocation,
					GLuint constantLocation,
					GLuint linearLocation,
					GLuint exponentLocation,
					GLuint edgeLocation	);

	void UseLight(Shader* p_shader);

	void SetFlash(glm::vec3 pos, glm::vec3 dir);

	~SpotLight();

private:
	static int lightCount;

public:
	static int getLightCount()
	{
		return lightCount;
	}

private:
	glm::vec3 m_direction;

	GLfloat m_edge,
			m_procEdge;
};

