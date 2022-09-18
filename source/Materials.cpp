#include "Materials.h"
#include "Shader.h"

Material::Material()
{
	specularIntensity = 0.0f;
	shininess = 0.0f;
}

Material::Material(GLfloat sIntensity, GLfloat shine)
{
	specularIntensity = sIntensity;
	shininess = shine;
}

void Material::UseMaterial(GLuint specularIntensityLocation, GLuint shininessLocation)
{
	glUniform1f(specularIntensityLocation, specularIntensity);
	glUniform1f(shininessLocation, shininess);
}

void Material::UseMaterial(Shader* p_shader)
{
	if (p_shader != nullptr)
	{
		p_shader->useMaterial(this);
		//Light::UseLight(p_shader);
	}
}

Material::~Material()
{
}
