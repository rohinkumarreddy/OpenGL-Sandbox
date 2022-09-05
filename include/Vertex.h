#pragma once

#include "glm/glm.hpp"

struct Vertex
{
	glm::vec3 position;
	glm::vec3 texture;
	glm::vec3 normal;
};

struct Vertex4
{
	glm::vec3 position;
	glm::vec3 color;
	glm::vec3 texture;
	glm::vec3 normal;
};