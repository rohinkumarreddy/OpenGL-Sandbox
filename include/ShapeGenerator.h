#pragma once

#include "ShapeData.h"

class ShapeGenerator
{
public:
	enum class ShapeAttributeProfile
	{
		POS_COL_TEX_NOR,
		POS_COL_NOR,
		POS_TEX_NOR,
		POS_NOR,
		POS_COL
	};
	static ShapeData makeTriangle(ShapeAttributeProfile);
	static ShapeData makeCube(ShapeAttributeProfile);
	static ShapeData makeArrow(ShapeAttributeProfile);
	static ShapeData makeTorus( ShapeAttributeProfile,
								unsigned int tesselation = 20);
	static ShapeData makeSphere( ShapeAttributeProfile,
								 unsigned int tesselation = 20);
	static ShapeData makePlane( ShapeAttributeProfile,
								unsigned int dimensions = 10,
								unsigned int texRepeat = 0);
	static ShapeData makeTeapot(ShapeAttributeProfile, 
								unsigned int tesselation = 10,
								const glm::mat4& lidTransform = glm::mat4(1.0f));
	static ShapeData generateNormals(const ShapeData& data);

private:
	static ShapeData makePlaneVerts(unsigned int dimensions, unsigned int texRepeat = 0);
	static ShapeData makePlaneIndices(unsigned int dimensions);
	static ShapeData makePlaneUnseamedIndices(unsigned int tesselation);

	//Teapot helpers
	static void generatePatches(float* v, float* n, float* tc, unsigned short* el, int grid);
	static void moveLid(int grid, float* v, glm::mat4 lidTransform);
	static void buildPatchReflect(int patchNum,
		float* B, float* dB,
		float* v, float* n,
		float* tc, unsigned short* el,
		int& index, int& elIndex, int& tcIndex, int grid,
		bool reflectX, bool reflectY);
	static void buildPatch(glm::vec3 patch[][4],
		float* B, float* dB,
		float* v, float* n, float* tc,
		unsigned short* el,
		int& index, int& elIndex, int& tcIndex, int grid, glm::mat3 reflect,
		bool invertNormal);
	static void getPatch(int patchNum, glm::vec3 patch[][4], bool reverseV);
	static void computeBasisFunctions(float* B, float* dB, int grid);
	static glm::vec3 evaluate(int gridU, int gridV, float* B, glm::vec3 patch[][4]);
	static glm::vec3 evaluateNormal(int gridU, int gridV,
		float* B, float* dB, glm::vec3 patch[][4]);
};