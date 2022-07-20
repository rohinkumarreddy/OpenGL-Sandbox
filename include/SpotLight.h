#pragma once

#include "PointLight.h"
#include <iostream>

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

	inline void setDirection(GLfloat x, GLfloat y, GLfloat z)
	{
		m_direction = glm::vec3(x, y, z);
	}

	inline void setDirection(glm::vec3 direction)
	{
		m_direction = direction;
	}

	inline void setEdge(GLfloat edge)
	{
		m_edge = edge;
		m_procEdge = cosf(glm::radians(m_edge));
	}

	inline glm::vec3 getDirection()
	{
		return m_direction;
	}

	inline glm::vec2 getEdgeVec()
	{
		return glm::vec2(m_edge, m_procEdge);
	}

	inline GLfloat getDegEdge()
	{
		return m_edge;
	}

	inline GLfloat getEdge()
	{
		return m_procEdge;
	}

	inline const int getLightIndex()
	{
		return m_lightIndx;
	}

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
	int m_lightIndx;
};

