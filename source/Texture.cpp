#include "Texture.h"
#include "common.h"
#include <QImage>
#include <QFile>
#include <QMessageBox>

Texture::Texture()
	: m_texPath(""),
	m_texID(0)
{
}

Texture::Texture(const std::string& path)
	: m_texPath(path),
	m_texID(0)
{
}

Texture::~Texture()
{
	clear();
}

void Texture::loadTexture()
{
	QString path = QString::fromStdString(m_texPath);
	QFile m(path);
	if (!m.exists())
	{
		QMessageBox::critical(nullptr, "Error", "Unable to find required texture", QMessageBox::StandardButton::Ok);
		return;
	}

	QImage texImg(path);
	texImg = texImg.mirrored();

	GLCALL(glGenTextures(1, &m_texID));
	GLCALL(glBindTexture(GL_TEXTURE_2D, m_texID));

	GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
	GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
	GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

	GLCALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
		texImg.width(), texImg.height(),
		0, GL_BGRA, GL_UNSIGNED_BYTE,
		texImg.bits()));
	GLCALL(glGenerateMipmap(GL_TEXTURE_2D));

	GLCALL(glBindTexture(GL_TEXTURE_2D, 0));
}

void Texture::activate()
{
	GLCALL(glActiveTexture(GL_TEXTURE0));
	GLCALL(glBindTexture(GL_TEXTURE_2D, m_texID));
}

void Texture::clear()
{
	if(m_texID)
		glDeleteTextures(1, &m_texID);
	m_texPath = "";
}