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

bool Texture::build()
{
	if(m_texID == 0)
		GLCALL(glGenTextures(1, &m_texID));
	if (m_texID != 0)
	{
		activate();
		GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));//GL_MIRRORED_REPEAT));//GL_REPEAT
		GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));//GL_MIRRORED_REPEAT));//GL_REPEAT
		GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));//GL_LINEAR
		GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));//GL_LINEAR
		deActivate();
	}
	else
		return false;
	return true;
}

bool Texture::load(const std::string& filepath)
{
	QString path = QString::fromStdString(filepath);
	if(path == "")
		path = QString::fromStdString(m_texPath);

	QFile m(path);
	if (!m.exists())
	{
		QMessageBox::critical(nullptr, "Error", "Unable to find required texture", QMessageBox::StandardButton::Ok);
		return false;
	}

	QImage texImg(path);
	texImg = texImg.mirrored();
	texImg = texImg.convertToFormat(QImage::Format::Format_RGBA8888);

	if (build() == false)
		return false;

	activate();

	GLCALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
		texImg.width(), texImg.height(),
		0, GL_RGBA, GL_UNSIGNED_BYTE,//GL_BGRA
		texImg.bits()));

	GLCALL(glGenerateMipmap(GL_TEXTURE_2D));

	deActivate();

	return true;
}

void Texture::activate()
{
	GLCALL(glActiveTexture(GL_TEXTURE0));
	GLCALL(glBindTexture(GL_TEXTURE_2D, m_texID));
}

void Texture::deActivate()
{
	GLCALL(glBindTexture(GL_TEXTURE_2D, NULL));
}

void Texture::clear()
{
	deActivate();
	if(m_texID)
		glDeleteTextures(1, &m_texID);
	m_texPath = "";
}