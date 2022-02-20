#pragma once
#include "glm\glm.hpp"

struct lightData
{
	glm::vec3 pos;
	glm::vec3 color;
	glm::vec4 attenuation;//(kC, kL, kQ, cutoff)
	lightData()
	:pos(0,0,0),
	color(1,1,1),
	attenuation(0.0025, 0.0025, 0.0025, 0)
	{
	}
};