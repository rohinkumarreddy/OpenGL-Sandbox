#pragma once

#include <string>
#include <vector>
#include <map>

class Shader
{
public:
	static void deactivate();
public:
	Shader(char* vsFileName, char* fsFileName);
	~Shader();
	bool build();
	void activate();
	void initUniforms(std::vector<std::string> uniformLst);
	void addUniforms(std::vector<std::string> uniformLst);
	template<class T> bool setUniform(std::string, T);
	template<class T> bool addLightSource(T);
	template<class T> bool updateLightParams(T);
private:
	std::string m_vSrc;
	std::string m_fSrc;
	std::string m_vSrcFileName;
	std::string m_fSrcFileName;
	unsigned int m_Program;
	std::map<std::string, unsigned int> m_uniformList;
};