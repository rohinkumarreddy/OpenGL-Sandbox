#pragma once

#include <string>

class QImage;

class Texture
{
public:
	Texture(const std::string& filepath);
	Texture();
	~Texture();
	void loadTexture();
	void activate();
	void clear();
private:
	unsigned int m_texID;
	std::string m_texPath;
};