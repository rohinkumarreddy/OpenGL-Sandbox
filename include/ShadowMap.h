#pragma once

#include <GL\glew.h>

class ShadowMap
{
public:
	ShadowMap();

	virtual bool Init(unsigned int width, unsigned int height);

	virtual void Write();

	virtual void Read(GLenum TextureUnit);

	GLuint GetShadowWidth() { return m_shadowWidth; }
	GLuint GetShadowHeight() { return m_shadowHeight; }

	~ShadowMap();
protected:
	GLuint m_FBO, m_shadowMap;
	GLuint m_shadowWidth, m_shadowHeight;
};