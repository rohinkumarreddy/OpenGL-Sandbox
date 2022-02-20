#include "Shader.h"
#include "common.h"
#include <fstream>
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"

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
	for each (std::string var in uniformLst)
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
	return true;
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