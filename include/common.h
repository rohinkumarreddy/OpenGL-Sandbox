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