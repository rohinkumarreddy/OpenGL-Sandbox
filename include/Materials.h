#pragma once

#include <GL\glew.h>

//forward declaration
class Shader;

class Material
{
public:
	Material();
	Material(GLfloat sIntensity, GLfloat shine);

	void UseMaterial(GLuint specularIntensityLocation, GLuint shininessLocation);
	void UseMaterial(Shader* p_shader);


	~Material();

	inline GLfloat getSpecularIntensity() { return specularIntensity; }

	inline GLfloat getShininess() { return shininess; }

private:
	GLfloat specularIntensity;
	GLfloat shininess;
};

