#pragma once

#include "GL/glew.h"
#include <iostream>

/* Clear all errors */
void GLClearError();

/* Log errors */
bool GLLogCall(const char* function, const char* file, int line);

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCALL(x) GLClearError();\
        x;\
        ASSERT(GLLogCall(#x, __FILE__, __LINE__))

#define X_DELTA 0.1f
#define MAX_TRIS 20

const unsigned int NUM_VERTICES_PER_TRI = 3;
const unsigned int NUM_FLOATS_PER_VTX = 9;
const unsigned int VERTEX_BYTE_SIZE = NUM_FLOATS_PER_VTX * sizeof(float);
const unsigned int TEX_VERTEX_BYTE_SIZE = (NUM_FLOATS_PER_VTX+3) * sizeof(float);//add 3 values per vertex for texture (U,V,0)
const unsigned int TRIANGLE_BYTE_SIZE = NUM_VERTICES_PER_TRI * NUM_FLOATS_PER_VTX * sizeof(float);