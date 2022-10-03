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

bool Texture::loadTexture()
{
	QString path = QString::fromStdString(m_texPath);
	QFile m(path);
	if (!m.exists())
	{
		QMessageBox::critical(nullptr, "Error", "Unable to find required texture", QMessageBox::StandardButton::Ok);
		return false;
	}

	QImage texImg(path);
	texImg = texImg.mirrored();
	texImg = texImg.convertToFormat(QImage::Format::Format_RGBA8888);

	//std::cout << "chk1.1" << "\n";
	GLCALL(glGenTextures(1, &m_texID));
	GLCALL(glBindTexture(GL_TEXTURE_2D, m_texID));

	//std::cout << "chk1.2" << "\n";
	GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));//GL_MIRRORED_REPEAT));//GL_REPEAT
	GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));//GL_MIRRORED_REPEAT));//GL_REPEAT
	GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));//GL_LINEAR
	GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));//GL_LINEAR

	//std::cout << "chk1.3 width " << texImg.width() << " height " << texImg.height() << "\n";
	GLCALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
		texImg.width(), texImg.height(),
		0, GL_RGBA, GL_UNSIGNED_BYTE,//GL_BGRA
		texImg.bits()));
	//std::cout << "chk1.4" << "\n";
	GLCALL(glGenerateMipmap(GL_TEXTURE_2D));

	//std::cout << "chk1.5" << "\n";
	GLCALL(glBindTexture(GL_TEXTURE_2D, 0));
	//std::cout << "chk1.6" << "\n";
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
	if(m_texID)
		glDeleteTextures(1, &m_texID);
	m_texPath = "";
}