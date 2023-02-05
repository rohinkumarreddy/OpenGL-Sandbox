#pragma once

#include <string>

class QImage;

class Texture
{
public:
	Texture(const std::string& filepath);
	Texture();
	~Texture();
	bool build();
	bool load(const std::string& filepath = "");
	void activate();
	void clear();
	static void deActivate();
private:
	unsigned int m_texID;
	std::string m_texPath;
};