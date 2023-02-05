#include "Shader.h"
#include "common.h"
#include <fstream>
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"

#include "SpotLight.h"
#include "DirectionalLight.h"
#include "Materials.h"

//#include <string>

//read file contents
std::string readSrc(const char* fileName)
{
	std::ifstream fileInput(fileName);
	if (!fileInput.good())
		return std::string();
	else
	{
		std::string ret = std::string(std::istreambuf_iterator<char>(fileInput),
			std::istreambuf_iterator<char>());
		fileInput.close();
		return ret;
	}
}

bool checkShaderBuild(GLuint build,
	PFNGLGETSHADERIVPROC getBuildProperty,
	PFNGLGETSHADERINFOLOGPROC getBuildInfoLog,
	GLenum statusType)
{
	GLint result = GL_FALSE;
	GLCALL(getBuildProperty(build, statusType, &result));
	if (result == GL_FALSE)
	{
		int length = 0;
		GLCALL(getBuildProperty(build, GL_INFO_LOG_LENGTH, &length));
		char* message = new char[length];
		memset(message, 0, length * sizeof(char));
		GLCALL(getBuildInfoLog(build, length, &length, message));
		std::cout << message << std::endl;
		if (message)delete[] message;
		return false;
	}
	return true;
}

Shader::Shader( char* vsFileName, char* fsFileName)
	:
	m_vSrcFileName(vsFileName),
	m_fSrcFileName(fsFileName),
	m_Program(NULL)
{
}

Shader::~Shader()
{
	/* clean-up */
	if (m_Program)
		glDeleteProgram(m_Program);
	m_Program = 0;
	m_uniformList.clear();
}

bool Shader::build()
{
	m_vSrc.clear();
	m_vSrc = readSrc(m_vSrcFileName.c_str());
	if (m_vSrc.empty())
	{
		std::cerr << "[Error]: Failed to read vertex shader!" << std::endl;
		return false;
	}
	const char* vsrc = m_vSrc.c_str();
	GLuint _vshader = 0;
	GLCALL(_vshader = glCreateShader(GL_VERTEX_SHADER));
	GLCALL(glShaderSource(_vshader, 1, &vsrc, 0));
	GLCALL(glCompileShader(_vshader));
	if (!checkShaderBuild(_vshader,
		glGetShaderiv,
		glGetShaderInfoLog,
		GL_COMPILE_STATUS))
	{
		std::cerr << "[Error]: Failed to compile vertex shader!" << std::endl;
		return false;
	}

	m_fSrc.clear();
	m_fSrc = readSrc(m_fSrcFileName.c_str());
	if (m_fSrc.empty())
	{
		std::cerr << "[Error]: Failed to read fragment shader!" << std::endl;
		return false;
	}
	const char* fsrc = m_fSrc.c_str();
	GLuint _fshader = 0;
	GLCALL(_fshader = glCreateShader(GL_FRAGMENT_SHADER));
	GLCALL(glShaderSource(_fshader, 1, &fsrc, 0));
	GLCALL(glCompileShader(_fshader));
	if (!checkShaderBuild(_fshader,
		glGetShaderiv,
		glGetShaderInfoLog,
		GL_COMPILE_STATUS))
	{
		std::cerr << "[Error]: Failed to compile fragment shader!" << std::endl;
		return false;
	}

	/*
	* For OpenGL versions not supporting layout() in glsl,
	* we can use glBindAttribLocation() to set location
	* and glGetAttribLocation to get location of attributes.
	* Note: glBindAttribLocation must be performed before linking.
	*/

	GLCALL(m_Program = glCreateProgram());
	GLCALL(glAttachShader(m_Program, _vshader));
	GLCALL(glAttachShader(m_Program, _fshader));
	GLCALL(glLinkProgram(m_Program));

	if (!checkShaderBuild(m_Program,
		glGetProgramiv,
		glGetProgramInfoLog,
		GL_LINK_STATUS))
	{
		std::cerr << "[Error]: Failed to link shader!" << std::endl;
		return false;
	}
	/* clean up*/
	GLCALL(glDeleteShader(_vshader));
	GLCALL(glDeleteShader(_fshader));
	GLCALL(glUseProgram(NULL));

	return true;
}

void Shader::activate()
{
	GLCALL(glUseProgram(m_Program));
}

void Shader::deactivate()
{
	//GLCALL(glUseProgram(0));
	glUseProgram(0);
}

void Shader::initUniforms(std::vector<std::string> uniformLst)
{
	m_uniformList.clear();
	for (std::string var : uniformLst)
	{
		GLuint uniformLoc = 0;
		GLCALL(uniformLoc = glGetUniformLocation(m_Program, var.c_str()));
		m_uniformList[var] = uniformLoc;
	}
}

void Shader::addUniforms(std::vector<std::string> uniformLst)
{
	for (std::string var : uniformLst)
	{
		GLuint uniformLoc = 0;
		GLCALL(uniformLoc = glGetUniformLocation(m_Program, var.c_str()));
		m_uniformList[var] = uniformLoc;
	}
}

template<class T> bool Shader::setUniform(std::string uniform, T val)
{
	if (m_uniformList.find(uniform) == m_uniformList.end())
	{
		std::cerr << "[Error]: Unable to find uniform " << uniform << std::endl;
		return false;
	}
	std::cerr << "[Warning]: Unsupported uniform type  var " << uniform << std::endl;
	//GLCALL(glUniform3fv(m_uniformList[uniform], 1, &val[0]));
	return false;
}

template<> bool Shader::setUniform<glm::vec3>(std::string uniform, glm::vec3 val)
{
	if (m_uniformList.find(uniform) == m_uniformList.end())
	{
		std::cerr << "[Error]: Unable to find uniform " << uniform << std::endl;
		return false;
	}
	GLCALL(glUniform3fv(m_uniformList[uniform], 1, &val[0]));
	return true;
}

template<> bool Shader::setUniform<glm::vec4>(std::string uniform, glm::vec4 val)
{
	if (m_uniformList.find(uniform) == m_uniformList.end())
	{
		std::cerr << "[Error]: Unable to find uniform " << uniform << std::endl;
		return false;
	}
	GLCALL(glUniform4fv(m_uniformList[uniform], 1, &val[0]));
	return true;
}

template<> bool Shader::setUniform<glm::mat4>(std::string uniform, glm::mat4 val)
{
	if (m_uniformList.find(uniform) == m_uniformList.end())
	{
		std::cerr << "[Error]: Unable to find uniform " << uniform << std::endl;
		return false;
	}
	GLCALL(glUniformMatrix4fv(m_uniformList[uniform], 1, GL_FALSE, &val[0][0]));
	return true;
}

template<> bool Shader::setUniform<GLfloat>(std::string uniform, GLfloat val)
{
	if (m_uniformList.find(uniform) == m_uniformList.end())
	{
		std::cerr << "[Error]: Unable to find uniform " << uniform << std::endl;
		return false;
	}
	GLCALL(glUniform1f(m_uniformList[uniform], val));
	return true;
}

template<> bool Shader::setUniform<GLint>(std::string uniform, GLint val)
{
	if (m_uniformList.find(uniform) == m_uniformList.end())
	{
		std::cerr << "[Error]: Unable to find uniform " << uniform << std::endl;
		return false;
	}
	GLCALL(glUniform1i(m_uniformList[uniform], val));
	return true;
}

template<class T> void Shader::initMaterial(T)
{
	std::cerr << "[Warning]: Unsupported material type used " << std::endl;
	return false;
}

template<> void Shader::initMaterial(Material* material)
{
	this->addUniforms(std::vector<std::string>
	{
		"u_material.specularIntensity",
		"u_material.shininess"
	});
	setUniform("u_material.specularIntensity", material->getSpecularIntensity());
	setUniform("u_material.shininess", material->getShininess());
}

template<class T> void Shader::useMaterial(T)
{
	std::cerr << "[Warning]: Unsupported material type used " << std::endl;
	return false;
}

template<> void Shader::useMaterial(Material* material)
{
	setUniform("u_material.specularIntensity", material->getSpecularIntensity());
	setUniform("u_material.shininess", material->getShininess());
}

template<class T> bool Shader::addLightSource(T)
{
	std::cerr << "[Warning]: Unsupported light source type used " << std::endl;
	return false;
}

template<> bool Shader::addLightSource<DirectionalLight*>(DirectionalLight* src)
{
	this->addUniforms(std::vector<std::string>
	{
		"u_directionalLight.direction",
		"u_directionalLight.base.color",
		"u_directionalLight.base.ambientIntensity",
		"u_directionalLight.base.diffuseIntensity"
	});
	return true;
}

template<> bool Shader::addLightSource<PointLight*>(PointLight* src)
{
	int indx = src->getLightIndex();
	this->addUniforms(std::vector<std::string>
	{
		"u_pointLights[" + std::to_string(indx) + "].position",
		"u_pointLights[" + std::to_string(indx) + "].constant",
		"u_pointLights[" + std::to_string(indx) + "].linear",
		"u_pointLights[" + std::to_string(indx) + "].exponent",
		"u_pointLights[" + std::to_string(indx) + "].base.color",
		"u_pointLights[" + std::to_string(indx) + "].base.ambientIntensity",
		"u_pointLights[" + std::to_string(indx) + "].base.diffuseIntensity"
	});
	return true;
}

template<> bool Shader::addLightSource<SpotLight*>(SpotLight* src)
{
	int indx = src->getLightIndex();
	this->addUniforms(std::vector<std::string>
	{
		"u_spotLights[" + std::to_string(indx) + "].direction",
		"u_spotLights[" + std::to_string(indx) + "].edge",
		"u_spotLights[" + std::to_string(indx) + "].epsilon",
		"u_spotLights[" + std::to_string(indx) + "].base.position",
		"u_spotLights[" + std::to_string(indx) + "].base.constant",
		"u_spotLights[" + std::to_string(indx) + "].base.linear",
		"u_spotLights[" + std::to_string(indx) + "].base.exponent",
		"u_spotLights[" + std::to_string(indx) + "].base.base.color",
		"u_spotLights[" + std::to_string(indx) + "].base.base.ambientIntensity",
		"u_spotLights[" + std::to_string(indx) + "].base.base.diffuseIntensity"
	});
	return true;
}

template<class T> bool Shader::updateLightParams(T)
{
	std::cerr << "[Warning]: Unsupported light source type used " << std::endl;
	return false;
}

template<> bool Shader::updateLightParams<DirectionalLight*>(DirectionalLight* src)
{
	setUniform("u_directionalLight.direction", src->getDirection());
	setUniform("u_directionalLight.base.color", src->getColor());
	setUniform("u_directionalLight.base.ambientIntensity", src->getAmbientIntensity());
	setUniform("u_directionalLight.base.diffuseIntensity", src->getDiffusionIntensity());
	return true;
}

template<> bool Shader::updateLightParams<PointLight*>(PointLight* src)
{
	int indx = src->getLightIndex();
	setUniform("u_pointLights[" +
		std::to_string(indx) +
		"].position",
		src->getPosition());

	setUniform("u_pointLights[" +
		std::to_string(indx) +
		"].constant",
		src->getConstant());

	setUniform("u_pointLights[" +
		std::to_string(indx) +
		"].linear",
		src->getLinear());

	setUniform("u_pointLights[" +
		std::to_string(indx) +
		"].exponent",
		src->getExponent());

	setUniform("u_pointLights[" +
		std::to_string(indx) +
		"].base.color",
		src->getColor());

	setUniform("u_pointLights[" +
		std::to_string(indx) +
		"].base.ambientIntensity",
		src->getAmbientIntensity());

	setUniform("u_pointLights[" +
		std::to_string(indx) +
		"].base.diffuseIntensity",
		src->getDiffusionIntensity());
	return true;
}

template<> bool Shader::updateLightParams<SpotLight*>(SpotLight* src)
{
	int indx = src->getLightIndex();

	setUniform("u_spotLights[" +
		std::to_string(indx) +
		"].direction",
		src->getDirection());

	setUniform("u_spotLights[" +
		std::to_string(indx) +
		"].edge",
		src->getEdge());

	setUniform("u_spotLights[" +
		std::to_string(indx) +
		"].epsilon",
		src->getEpsilon());

	setUniform("u_spotLights[" +
		std::to_string(indx) +
		"].base.position",
		src->getPosition());

	setUniform("u_spotLights[" +
		std::to_string(indx) +
		"].base.constant",
		src->getConstant());

	setUniform("u_spotLights[" +
		std::to_string(indx) +
		"].base.linear",
		src->getLinear());

	setUniform("u_spotLights[" +
		std::to_string(indx) +
		"].base.exponent",
		src->getExponent());

	setUniform("u_spotLights[" +
		std::to_string(indx) +
		"].base.base.color",
		src->getColor());

	setUniform("u_spotLights[" +
		std::to_string(indx) +
		"].base.base.ambientIntensity",
		src->getAmbientIntensity());

	setUniform("u_spotLights[" +
		std::to_string(indx) +
		"].base.base.diffuseIntensity",
		src->getDiffusionIntensity());
	return true;
}

/*GLCALL(m_u_MVPMtxLoc = glGetUniformLocation(m_Program, "u_MVPMtx"));
GLCALL(m_u_ambientLightLoc = glGetUniformLocation(m_Program, "u_ambientLight"));
GLCALL(m_u_lightPosLoc = glGetUniformLocation(m_Program, "u_lightPos"));
GLCALL(m_u_eyePosLoc = glGetUniformLocation(m_Program, "u_eyePos"));
GLCALL(m_u_MWMtxLoc = glGetUniformLocation(m_Program, "u_MWMtx"));
GLCALL(m_u_lAttenuationFacLoc = glGetUniformLocation(m_Program, "u_lAttenuationFac"));
GLCALL(glUniform4fv(m_u_ambientLightLoc, 1, &ambientLight[0]));
GLCALL(glUniform3fv(m_u_eyePosLoc, 1, &eyePos[0]));
GLCALL(glUniform3f(m_u_lightPosLoc,
	lightPos.r, lightPos.g, lightPos.b));
GLCALL(glUniform4fv(m_u_lAttenuationFacLoc, 1, &lightAttn[0]));
GLCALL(glUniformMatrix4fv(m_u_MVPMtxLoc, 1, GL_FALSE, &MVPMtx[0][0]));//model to projection
GLCALL(glUniformMatrix4fv(m_u_MWMtxLoc, 1, GL_FALSE, &shModelMtx[0][0]));//model to world*/