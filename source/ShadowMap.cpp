#include "ShadowMap.h"

#include <iostream>

ShadowMap::ShadowMap()
	:m_FBO(0),
	m_shadowMap(0),
	m_shadowWidth(0),
	m_shadowHeight(0)
{
}

bool ShadowMap::Init(unsigned int width, unsigned int height)
{
	m_shadowWidth = width; m_shadowHeight = height;

	glGenFramebuffers(1, &m_FBO);

	glGenTextures(1, &m_shadowMap);
	glBindTexture(GL_TEXTURE_2D, m_shadowMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_FBO);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_shadowMap, 0);

	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (Status != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "Framebuffer error: " << Status << "\n";
		return false;
	}

	return true;
}

void ShadowMap::Write()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_FBO);
}

void ShadowMap::Read(GLenum texUnit)
{
	glActiveTexture(texUnit);
	glBindTexture(GL_TEXTURE_2D, m_shadowMap);
}

ShadowMap::~ShadowMap()
{
	if (m_FBO)
	{
		glDeleteFramebuffers(1, &m_FBO);
	}

	if (m_shadowMap)
	{
		glDeleteTextures(1, &m_shadowMap);
	}
}
