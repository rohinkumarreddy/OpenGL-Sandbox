#include "GL/glew.h"

//#include "glm/glm.hpp"//included in ShapeGenerator
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"

#include "DrawWidget.h"

#include <QWidget>
#include <QMouseEvent>
#include <QKeyEvent>

#include <iostream>
#include <fstream>

#include "ShapeGenerator.h"
#include "Camera.h"
#include "lightData.h"
//const float X_DELTA = 0.1f;
//const uint NUM_VERTICES_PER_TRI = 3;
//const uint NUM_FLOATS_PER_VTX = 6;
//const uint TRIANGLE_BYTE_SIZE = NUM_VERTICES_PER_TRI * NUM_FLOATS_PER_VTX * sizeof(float);
//const uint MAX_TRIS = 20;

#define X_DELTA 0.1f
#define MAX_TRIS 20
const uint NUM_VERTICES_PER_TRI = 3;
const uint NUM_FLOATS_PER_VTX = 9;
const uint VERTEX_BYTE_SIZE = NUM_FLOATS_PER_VTX * sizeof(float);
const uint TRIANGLE_BYTE_SIZE = NUM_VERTICES_PER_TRI * NUM_FLOATS_PER_VTX * sizeof(float);

void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		std::cerr << "[OpenGL error] (" << error << "): " << function
			<< " " << file << ":" << line << std::endl;
		return false;
	}
	return true;
}

DrawWidget::DrawWidget(QWidget* parent)
: QGLWidget(parent),
m_pCamera(new Camera),
m_pLightData(NULL),
m_Sh1Vao(NULL),
m_Sh1VtxOff(NULL),
m_Sh1IndxOff(NULL),
m_Sh2Vao(NULL),
m_Sh2VtxOff(NULL),
m_Sh2IndxOff(NULL),
m_Sh3Vao(NULL),
m_Sh3VtxOff(NULL),
m_Sh3IndxOff(NULL),
m_Sh4Vao(NULL),
m_Sh4VtxOff(NULL),
m_Sh4IndxOff(NULL),
m_Sh5Vao(NULL),
m_Sh5VtxOff(NULL),
m_Sh5IndxOff(NULL),
m_Sh6Vao(NULL),
m_Sh6VtxOff(NULL),
m_Sh6IndxOff(NULL),
m_VIbo(NULL),
m_Program(NULL),
m_ProgramPT(NULL),
m_u_MVPMtxLoc(NULL),
m_u_ambientLightLoc(NULL),
m_u_lightPosLoc(NULL),
m_u_eyePosLoc(NULL),
m_u_MWMtxLoc(NULL),
m_u_MVPMtxPTLoc(NULL),
m_VertexShaderSrc(""),
m_FragmentShaderSrc(""),
m_VertexShaderPTSrc(""),
m_FragmentShaderPTSrc(""),
m_NumTri(1),
m_Sh1NumIndcs(NULL),
m_Sh2NumIndcs(NULL),
m_Sh3NumIndcs(NULL),
m_Sh4NumIndcs(NULL),
m_Sh5NumIndcs(NULL),
m_Sh6NumIndcs(NULL)
{
}

DrawWidget::~DrawWidget()
{
	/* GLCALL results in error here */

	//Clean-up vertex & index buffers
	if(m_VIbo)
		glDeleteBuffers(1, &m_VIbo);

	//Clean-up vertex arrays
	if (m_Sh1Vao)
		glDeleteVertexArrays(1, &m_Sh1Vao);
	if (m_Sh2Vao)
		glDeleteVertexArrays(1, &m_Sh2Vao);
	if (m_Sh3Vao)
		glDeleteVertexArrays(1, &m_Sh3Vao);
	if (m_Sh4Vao)
		glDeleteVertexArrays(1, &m_Sh4Vao);
	if (m_Sh5Vao)
		glDeleteVertexArrays(1, &m_Sh5Vao);
	if (m_Sh6Vao)
		glDeleteVertexArrays(1, &m_Sh6Vao);

	glUseProgram(0);

	//Clean-up shader program
	if (m_Program)
		glDeleteProgram(m_Program);
	if(m_ProgramPT)
		glDeleteProgram(m_ProgramPT);

	if (m_pCamera != nullptr)
		delete m_pCamera;
}

/* Depth data */
/*
const float z1 =  0.5f,
			z2 = -0.5f;

float verts[] =
{
-1.0f, -1.0f,  z1,
 1.0f,	0.0f,  0.0f,
 0.0f,  1.0f, -1.0f,
 0.0f,	0.0f,  1.0f,
 1.0f, -1.0f,  z1,
 1.0f,	0.0f,  0.0f,

 0.0f,  1.0f,  z2,
 0.0f,	0.0f,  1.0f,
 0.0f, -1.0f,  z2,
 0.0f,	0.0f,  1.0f,
 1.0f,  1.0f,  z2,
 0.0f,  0.0f,  1.0f
 }
	 */

/* Matrix transformations */
////Done only to learn | not optimal memory wise
//glm::mat4 projMtx = glm::perspective(glm::radians(60.0f),
//	((float)width()) / height(), 0.1f, 10.0f);
//glm::mat4 fullTxMtx[] =
//{
//	//teapot 1
//	projMtx * m_pCamera->getWorldToViewMtx() * glm::translate(glm::vec3(-1.0f, 0.0f, -3.0f)) * glm::rotate(glm::radians(36.0f), glm::vec3(1.0f, 0.0f, 0.0f)),

//	//teapot 2
//	projMtx * m_pCamera->getWorldToViewMtx() * glm::translate(glm::vec3(1.0f, 0.0f, -3.75f)) * glm::rotate(glm::radians(126.0f), glm::vec3(0.0f, 1.0f, 0.0f))
//};

/* Matrix buffer object */
//GLCALL(glGenBuffers(1, &m_Mtxbo));
//GLCALL(glBindBuffer(GL_ARRAY_BUFFER, m_Mtxbo));
///*GLCALL(glBufferData(GL_ARRAY_BUFFER,
//	sizeof(fullTxMtx), fullTxMtx, GL_STATIC_DRAW));*/
//GLCALL(glBufferData(GL_ARRAY_BUFFER,
//	sizeof(glm::mat4)*2, 0, GL_DYNAMIC_DRAW));

////Each shader attribute has 4 floats
////Mat-4 has 16 or 4 sets of 4-floats
////We use 4 attributes for each mat-4
///* Attribute-2,3,4,5|matrices */
//GLCALL(glEnableVertexAttribArray(2));
//GLCALL(glEnableVertexAttribArray(3));
//GLCALL(glEnableVertexAttribArray(4));
//GLCALL(glEnableVertexAttribArray(5));

//GLCALL(glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4),
//	(const void*)(0 * sizeof(float))));
//GLCALL(glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4),
//	(const void*)(4 * sizeof(float))));
//GLCALL(glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4),
//	(const void*)(8 * sizeof(float))));
//GLCALL(glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4),
//	(const void*)(12 * sizeof(float))));

///* Attribute distribution */
//glVertexAttribDivisor(2, 1);
//glVertexAttribDivisor(3, 1);
//glVertexAttribDivisor(4, 1);
//glVertexAttribDivisor(5, 1);

///* Vertex array object */
//GLCALL(glGenVertexArrays(1, &m_Vao));
//GLCALL(glBindVertexArray(m_Vao));

void DrawWidget::initOpenGLData()
{
	/* Shape Object */
	ShapeData teapot = ShapeGenerator::makeTeapot();
	ShapeData arrow = ShapeGenerator::makeArrow();
	ShapeData plane = ShapeGenerator::makePlane(20);
	ShapeData cube = ShapeGenerator::makeCube();
	ShapeData torus = ShapeGenerator::makeTorus(50);
	ShapeData sphere = ShapeGenerator::makeSphere(50);

	/* Vertex buffer object */
	GLCALL(glGenBuffers(1, &m_VIbo));
	GLCALL(glBindBuffer(GL_ARRAY_BUFFER, m_VIbo));
	GLCALL(glBufferData(GL_ARRAY_BUFFER,
		teapot.vertexBufferSize() + teapot.indexBufferSize() +
		arrow.vertexBufferSize() + arrow.indexBufferSize() +
		plane.vertexBufferSize() + plane.indexBufferSize() +
		cube.vertexBufferSize() + cube.indexBufferSize() +
		torus.vertexBufferSize() + torus.indexBufferSize() +
		sphere.vertexBufferSize() + sphere.indexBufferSize(),
		0, GL_STATIC_DRAW));

	GLsizeiptr curOffset = 0;
	m_Sh1VtxOff = curOffset;

	/* Shape 1 data */
	//Vertex data
	GLCALL(glBufferSubData(GL_ARRAY_BUFFER, curOffset,
		teapot.vertexBufferSize(), teapot.vertices));
	curOffset += teapot.vertexBufferSize();

	m_Sh1IndxOff = curOffset;

	//Index data
	GLCALL(glBufferSubData(GL_ARRAY_BUFFER, curOffset,
		teapot.indexBufferSize(), teapot.indices));
	curOffset += teapot.indexBufferSize();

	m_Sh2VtxOff = curOffset;

	/* Shape 2 data */
	//Vertex data
	GLCALL(glBufferSubData(GL_ARRAY_BUFFER, curOffset,
		arrow.vertexBufferSize(), arrow.vertices));
	curOffset += arrow.vertexBufferSize();

	m_Sh2IndxOff = curOffset;

	//Index data
	GLCALL(glBufferSubData(GL_ARRAY_BUFFER, curOffset,
		arrow.indexBufferSize(), arrow.indices));
	curOffset += arrow.indexBufferSize();

	m_Sh3VtxOff = curOffset;

	/* Shape 3 data */
	//Vertex data
	GLCALL(glBufferSubData(GL_ARRAY_BUFFER, curOffset,
		plane.vertexBufferSize(), plane.vertices));
	curOffset += plane.vertexBufferSize();

	m_Sh3IndxOff = curOffset;

	//Index data
	GLCALL(glBufferSubData(GL_ARRAY_BUFFER, curOffset,
		plane.indexBufferSize(), plane.indices));
	curOffset += plane.indexBufferSize();

	m_Sh4VtxOff = curOffset;

	/* Shape 4 data */
	//Vertex data
	GLCALL(glBufferSubData(GL_ARRAY_BUFFER, curOffset,
		cube.vertexBufferSize(), cube.vertices));
	curOffset += cube.vertexBufferSize();

	m_Sh4IndxOff = curOffset;

	//Index data
	GLCALL(glBufferSubData(GL_ARRAY_BUFFER, curOffset,
		cube.indexBufferSize(), cube.indices));
	curOffset += cube.indexBufferSize();

	m_Sh5VtxOff = curOffset;

	/* Shape 5 data */
	//Vertex data
	GLCALL(glBufferSubData(GL_ARRAY_BUFFER, curOffset,
		torus.vertexBufferSize(), torus.vertices));
	curOffset += torus.vertexBufferSize();

	m_Sh5IndxOff = curOffset;

	//Index data
	GLCALL(glBufferSubData(GL_ARRAY_BUFFER, curOffset,
		torus.indexBufferSize(), torus.indices));
	curOffset += torus.indexBufferSize();

	m_Sh6VtxOff = curOffset;

	/* Shape 6 data */
	//Vertex data
	GLCALL(glBufferSubData(GL_ARRAY_BUFFER, curOffset,
		sphere.vertexBufferSize(), sphere.vertices));
	curOffset += sphere.vertexBufferSize();

	m_Sh6IndxOff = curOffset;

	//Index data
	GLCALL(glBufferSubData(GL_ARRAY_BUFFER, curOffset,
		sphere.indexBufferSize(), sphere.indices));
	curOffset += sphere.indexBufferSize();

	m_Sh1NumIndcs = teapot.numIndices;
	m_Sh2NumIndcs = arrow.numIndices;
	m_Sh3NumIndcs = plane.numIndices;
	m_Sh4NumIndcs = cube.numIndices;
	m_Sh5NumIndcs = torus.numIndices;
	m_Sh6NumIndcs = sphere.numIndices;

	/* Clean up */
	teapot.cleanUp();
	arrow.cleanUp();
	plane.cleanUp();
	cube.cleanUp();
	torus.cleanUp();
	sphere.cleanUp();

	/*
	* Attribute data can also be sent to shader
	* using glVertexAttrib3f(attribId, <data>)
	* without enabling the corresponding attrib-Array.
	* Note: can be used to switch from solid color & dynamic color etc.
	*/
}

/* Unbind buffers */
/*GLCALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
GLCALL(glBindVertexArray(0));*/

void DrawWidget::initVertexArrays()
{
	/* Generate vertex array objects */
	GLCALL(glGenVertexArrays(1, &m_Sh1Vao));
	GLCALL(glGenVertexArrays(1, &m_Sh2Vao));
	GLCALL(glGenVertexArrays(1, &m_Sh3Vao));
	GLCALL(glGenVertexArrays(1, &m_Sh4Vao));
	GLCALL(glGenVertexArrays(1, &m_Sh5Vao));
	GLCALL(glGenVertexArrays(1, &m_Sh6Vao));

	/* Vertex array object */
	GLCALL(glBindVertexArray(m_Sh1Vao));

	/* Vertex buffer object */
	GLCALL(glBindBuffer(GL_ARRAY_BUFFER, m_VIbo));

	/* Index buffer object */
	GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VIbo));

	GLCALL(glEnableVertexAttribArray(0));
	GLCALL(glEnableVertexAttribArray(1));
	GLCALL(glEnableVertexAttribArray(2));

	//attribute-0|position
	GLCALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE, 0));
	//4th element is by default 1.0f

	//attribute-1|color
	GLCALL(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE,
		(const void*)(3 * sizeof(float))));

	//attribute-2|normal
	GLCALL(glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE,
		(const void*)(6 * sizeof(float))));

	/* Vertex array object */
	GLCALL(glBindVertexArray(m_Sh2Vao));

	/* Vertex buffer object */
	GLCALL(glBindBuffer(GL_ARRAY_BUFFER, m_VIbo));

	/* Index buffer object */
	GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VIbo));

	GLCALL(glEnableVertexAttribArray(0));
	GLCALL(glEnableVertexAttribArray(1));
	GLCALL(glEnableVertexAttribArray(2));

	//attribute-0|position
	GLCALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE,
		(const void*)(m_Sh2VtxOff)));//4th element is by default 1.0f

	//attribute-1|color
	GLCALL(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE,
		(const void*)(m_Sh2VtxOff + 3 * sizeof(float))));

	//attribute-2|normal
	GLCALL(glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE,
		(const void*)(m_Sh2VtxOff + 6 * sizeof(float))));

	/* Vertex array object */
	GLCALL(glBindVertexArray(m_Sh3Vao));

	/* Vertex buffer object */
	GLCALL(glBindBuffer(GL_ARRAY_BUFFER, m_VIbo));

	/* Index buffer object */
	GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VIbo));

	GLCALL(glEnableVertexAttribArray(0));
	GLCALL(glEnableVertexAttribArray(1));
	GLCALL(glEnableVertexAttribArray(2));

	//attribute-0|position
	GLCALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE,
		(const void*)(m_Sh3VtxOff)));//4th element is by default 1.0f

	//attribute-1|color
	GLCALL(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE,
		(const void*)(m_Sh3VtxOff + 3 * sizeof(float))));

	//attribute-2|normal
	GLCALL(glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE,
		(const void*)(m_Sh3VtxOff + 6 * sizeof(float))));

	/* Vertex array object */
	GLCALL(glBindVertexArray(m_Sh4Vao));

	/* Vertex buffer object */
	GLCALL(glBindBuffer(GL_ARRAY_BUFFER, m_VIbo));

	/* Index buffer object */
	GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VIbo));

	GLCALL(glEnableVertexAttribArray(0));
	GLCALL(glEnableVertexAttribArray(1));
	GLCALL(glEnableVertexAttribArray(2));

	//attribute-0|position
	GLCALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE,
		(const void*)(/*m_Sh4VtxOff*/m_Sh6VtxOff)));//4th element is by default 1.0f

	//attribute-1|color
	GLCALL(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE,
		(const void*)(/*m_Sh4VtxOff*/m_Sh6VtxOff + 3 * sizeof(float))));

	//attribute-2|normal
	GLCALL(glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE,
		(const void*)(/*m_Sh4VtxOff*/m_Sh6VtxOff + 6 * sizeof(float))));

	/* Vertex array object */
	GLCALL(glBindVertexArray(m_Sh5Vao));

	/* Vertex buffer object */
	GLCALL(glBindBuffer(GL_ARRAY_BUFFER, m_VIbo));

	/* Index buffer object */
	GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VIbo));

	GLCALL(glEnableVertexAttribArray(0));
	GLCALL(glEnableVertexAttribArray(1));
	GLCALL(glEnableVertexAttribArray(2));

	//attribute-0|position
	GLCALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE,
		(const void*)(m_Sh5VtxOff)));//4th element is by default 1.0f

	//attribute-1|color
	GLCALL(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE,
		(const void*)(m_Sh5VtxOff + 3 * sizeof(float))));

	//attribute-2|normal
	GLCALL(glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE,
		(const void*)(m_Sh5VtxOff + 6 * sizeof(float))));

	/* Vertex array object */
	GLCALL(glBindVertexArray(m_Sh6Vao));

	/* Vertex buffer object */
	GLCALL(glBindBuffer(GL_ARRAY_BUFFER, m_VIbo));

	/* Index buffer object */
	GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VIbo));

	GLCALL(glEnableVertexAttribArray(0));
	GLCALL(glEnableVertexAttribArray(1));
	GLCALL(glEnableVertexAttribArray(2));

	//attribute-0|position
	GLCALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE,
		(const void*)(m_Sh6VtxOff)));//4th element is by default 1.0f

	//attribute-1|color
	GLCALL(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE,
		(const void*)(m_Sh6VtxOff + 3 * sizeof(float))));

	//attribute-2|normal
	GLCALL(glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, VERTEX_BYTE_SIZE,
		(const void*)(m_Sh6VtxOff + 6 * sizeof(float))));
}

void DrawWidget::drawIncTriangle()
{
	if (m_NumTri == MAX_TRIS)
		return;
	const GLfloat curX = -1 + m_NumTri * X_DELTA;
	GLfloat curTri[] =
	{
		curX, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,

		curX+X_DELTA, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,

		curX, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f
	};
	GLCALL(glBufferSubData(GL_ARRAY_BUFFER, m_NumTri * TRIANGLE_BYTE_SIZE, TRIANGLE_BYTE_SIZE, curTri));
	m_NumTri++;
}

void DrawWidget::setLDataPtr(lightData* vlightData)
{
	m_pLightData = vlightData;
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

std::string DrawWidget::readShader(const char* fileName)
{
	std::ifstream fileInput(fileName);
	if (!fileInput.good())
		return std::string();
	return std::string(std::istreambuf_iterator<char>(fileInput),
		std::istreambuf_iterator<char>());
}

bool DrawWidget::validateShader(GLuint shader)
{
	return checkShaderBuild(shader,
		glGetShaderiv,
		glGetShaderInfoLog,
		GL_COMPILE_STATUS);
}

bool DrawWidget::validateProgram(GLuint program)
{
	return checkShaderBuild(program,
		glGetProgramiv,
		glGetProgramInfoLog,
		GL_LINK_STATUS);
}

void DrawWidget::installShader()
{
	m_VertexShaderSrc = readShader("VertexShader.glsl");
	if (m_VertexShaderSrc.empty())
	{
		std::cerr << "[Error]: Failed to read vertex shader!" << std::endl;
		return;
	}
	const GLchar* vertexShaderSrc = m_VertexShaderSrc.c_str();

	m_FragmentShaderSrc = readShader("FragmentShader.glsl");
	if (m_FragmentShaderSrc.empty())
	{
		std::cerr << "[Error]: Failed to read fragment shader!" << std::endl;
		return;
	}
	const GLchar* fragmentShaderSrc = m_FragmentShaderSrc.c_str();

	GLCALL(GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER));
	GLCALL(GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER));

	GLCALL(glShaderSource(vertexShader, 1, &vertexShaderSrc, 0));
	GLCALL(glShaderSource(fragmentShader, 1, &fragmentShaderSrc, 0));

	GLCALL(glCompileShader(vertexShader));
	if (!validateShader(vertexShader))
	{
		std::cerr << "[Error]: Failed to compile vertex shader!" << std::endl;
		return;
	}

	GLCALL(glCompileShader(fragmentShader));
	if (!validateShader(fragmentShader))
	{
		std::cerr << "[Error]: Failed to compile fragment shader!" << std::endl;
		return;
	}

	/*
	* For OpenGL versions not supporting layout() in glsl,
	* we can use glBindAttribLocation() to set location
	* and glGetAttribLocation to get location of attributes.
	* Note: glBindAttribLocation must be performed before linking.
	*/

	GLCALL(m_Program = glCreateProgram());
	GLCALL(glAttachShader(m_Program, vertexShader));
	GLCALL(glAttachShader(m_Program, fragmentShader));
	GLCALL(glLinkProgram(m_Program));

	if (!validateProgram(m_Program))
	{
		std::cerr << "[Error]: Failed to link shader!" << std::endl;
		return;
	}
	GLCALL(glDeleteShader(vertexShader));
	GLCALL(glDeleteShader(fragmentShader));

	//GLCALL(glUseProgram(m_Program));
	//
	m_VertexShaderPTSrc = readShader("VertexShaderPT.glsl");
	if (m_VertexShaderPTSrc.empty())
	{
		std::cerr << "[Error]: Failed to read vertex shader!" << std::endl;
		return;
	}
	vertexShaderSrc = 0;
	vertexShaderSrc = m_VertexShaderPTSrc.c_str();

	m_FragmentShaderPTSrc = readShader("FragmentShaderPT.glsl");
	if (m_FragmentShaderPTSrc.empty())
	{
		std::cerr << "[Error]: Failed to read fragment shader!" << std::endl;
		return;
	}
	fragmentShaderSrc = 0;
	fragmentShaderSrc = m_FragmentShaderPTSrc.c_str();

	vertexShader = 0;
	fragmentShader = 0;
	GLCALL(vertexShader = glCreateShader(GL_VERTEX_SHADER));
	GLCALL(fragmentShader = glCreateShader(GL_FRAGMENT_SHADER));

	GLCALL(glShaderSource(vertexShader, 1, &vertexShaderSrc, 0));
	GLCALL(glShaderSource(fragmentShader, 1, &fragmentShaderSrc, 0));

	GLCALL(glCompileShader(vertexShader));
	if (!validateShader(vertexShader))
	{
		std::cerr << "[Error]: Failed to compile vertex shader!" << std::endl;
		return;
	}

	GLCALL(glCompileShader(fragmentShader));
	if (!validateShader(fragmentShader))
	{
		std::cerr << "[Error]: Failed to compile fragment shader!" << std::endl;
		return;
	}
	GLCALL(m_ProgramPT = glCreateProgram());
	GLCALL(glAttachShader(m_ProgramPT, vertexShader));
	GLCALL(glAttachShader(m_ProgramPT, fragmentShader));
	GLCALL(glLinkProgram(m_ProgramPT));

	if (!validateProgram(m_ProgramPT))
	{
		std::cerr << "[Error]: Failed to link shader!" << std::endl;
		return;
	}
	GLCALL(glDeleteShader(vertexShader));
	GLCALL(glDeleteShader(fragmentShader));
	//
}

void DrawWidget::initializeGL()
{
	/* Initialize GLEW */
	if (glewInit() != GLEW_OK)
		std::cerr << "[Error]: glewInit failed!" << std::endl;

	/* Print out OpenGL version */
	GLCALL(std::cout << "[Info]: opengl version "
		<< glGetString(GL_VERSION) << std::endl);

	/* Enable depth buffer */
	GLCALL(glEnable(GL_DEPTH_TEST));

	/* Initialize OpenGL data */
	initOpenGLData();

	/* Initialize vertex arrays */
	initVertexArrays();

	/* Initialize shaders */
	installShader();

	/* Select shader program */
	GLCALL(glUseProgram(m_Program));

	/* Query uniform */
	GLCALL(m_u_MVPMtxLoc = glGetUniformLocation(m_Program, "u_MVPMtx"));
	GLCALL(m_u_ambientLightLoc = glGetUniformLocation(m_Program, "u_ambientLight"));
	GLCALL(m_u_lightPosLoc = glGetUniformLocation(m_Program, "u_lightPos"));
	GLCALL(m_u_eyePosLoc = glGetUniformLocation(m_Program, "u_eyePos"));
	GLCALL(m_u_MWMtxLoc = glGetUniformLocation(m_Program, "u_MWMtx"));
	GLCALL(m_u_lAttenuationFacLoc = glGetUniformLocation(m_Program, "u_lAttenuationFac"));

	/* Select shader program */
	GLCALL(glUseProgram(m_ProgramPT));

	/* Query uniform */
	GLCALL(m_u_MVPMtxPTLoc = glGetUniformLocation(m_ProgramPT, "u_MVPMtx"));
}

void DrawWidget::paintGL()
{
	GLCALL(glEnable(GL_DEPTH_TEST));
	GLCALL(glEnable(GL_CULL_FACE));

	/* Clear depth buffer */
	GLCALL(glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT));

	/* Viewport */
	GLCALL(glViewport(0, 0, width() * devicePixelRatio(), height() * devicePixelRatio()));

	/* Matrix transformations */
	//Done only to learn | not optimal memory wise
	glm::mat4 MVPMtx = glm::mat4(1.0f);
	glm::mat4 projMtx = glm::perspective(glm::radians(60.0f),
		((float)width()) / height(), 0.1f, 20.0f);//view to projection
	glm::mat4 viewMtx = m_pCamera->getWorldToViewMtx();//world to view
	glm::mat4 VPMtx = projMtx * viewMtx;//world to projection
	glm::vec3 eyePos = m_pCamera->getPosition();//eye position|world space

	//Ambient-light
	glm::vec4 ambientLight(0.05f, 0.05f, 0.05f, 1.0f);

	//Light-source
	glm::vec3 lightPos(0.0f, 3.0f, 0.0f);
	if (m_pLightData)
		lightPos = m_pLightData->pos;

	//Light attenuation(kC, kL, kQ)
	glm::vec4 lightAttn(0.0025f, 0.0025f, 0.0025f, 0);
	if (m_pLightData)
		lightAttn = m_pLightData->attenuation;

	/* Select shader program */
	GLCALL(glUseProgram(m_Program));

	//Set Uniform
	GLCALL(glUniform4fv(m_u_ambientLightLoc, 1, &ambientLight[0]));
	GLCALL(glUniform3fv(m_u_eyePosLoc, 1, &eyePos[0]));
	GLCALL(glUniform3f(m_u_lightPosLoc,
		lightPos.r, lightPos.g, lightPos.b));
	GLCALL(glUniform4fv(m_u_lAttenuationFacLoc, 1, &lightAttn[0]));

	/* Bind VAO */
	GLCALL(glBindVertexArray(m_Sh1Vao));

	/* Bind IBO */
	//GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Sh1Ibo));

	//teapot-1
	glm::mat4 shModelMtx = glm::translate(glm::vec3(-3.0f, 0.0f, -2.0f)) *
							glm::rotate(glm::radians(-90.0f),
							glm::vec3(1.0f, 0.0f, 0.0f));//model to world
	MVPMtx = VPMtx * shModelMtx;//model to projection

	//Set Uniform
	GLCALL(glUniformMatrix4fv(m_u_MVPMtxLoc, 1, GL_FALSE, &MVPMtx[0][0]));//model to projection
	GLCALL(glUniformMatrix4fv(m_u_MWMtxLoc, 1, GL_FALSE, &shModelMtx[0][0]));//model to world

	//Draw teapot-1
	GLCALL(glDrawElements(GL_TRIANGLES, m_Sh1NumIndcs,
						  GL_UNSIGNED_SHORT, (const void*)m_Sh1IndxOff));

	/* Bind VAO */
	GLCALL(glBindVertexArray(m_Sh2Vao));

	/* Bind IBO */
	//GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Sh2Ibo));

	//Shape-2|arrow-1
	shModelMtx = glm::translate(glm::vec3(3.0f, 0.0f, 0.0f));//model to world
	MVPMtx = VPMtx * shModelMtx;//model to projection

	//Set Uniform
	GLCALL(glUniformMatrix4fv(m_u_MVPMtxLoc, 1, GL_FALSE, &MVPMtx[0][0]));//model to projection
	GLCALL(glUniformMatrix4fv(m_u_MWMtxLoc, 1, GL_FALSE, &shModelMtx[0][0]));//model to world

	//Draw shape-2|arrow-1
	GLCALL(glDrawElements(GL_TRIANGLES, m_Sh2NumIndcs, GL_UNSIGNED_SHORT,
		(const void*)m_Sh2IndxOff));

	/* Bind VAO */
	GLCALL(glBindVertexArray(m_Sh3Vao));

	/* Bind IBO */
	//GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Sh2Ibo));

	//Shape-3|plane
	shModelMtx = glm::mat4(1.0f);//model to world
	MVPMtx = VPMtx * shModelMtx;//model to projection

	//Set Uniform
	GLCALL(glUniformMatrix4fv(m_u_MVPMtxLoc, 1, GL_FALSE, &MVPMtx[0][0]));//model to projection
	GLCALL(glUniformMatrix4fv(m_u_MWMtxLoc, 1, GL_FALSE, &shModelMtx[0][0]));//model to world

	//Draw shape-3|plane
	GLCALL(glDrawElements(GL_TRIANGLES, m_Sh3NumIndcs, GL_UNSIGNED_SHORT,
		(const void*)m_Sh3IndxOff));
	
	/* Bind VAO */
	GLCALL(glBindVertexArray(m_Sh5Vao));

	/* Bind IBO */
	//GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Sh2Ibo));

	//Shape-5|torus
	shModelMtx = glm::translate(glm::vec3(0.0f, 0.15f, 0.0f));//model to world
	MVPMtx = VPMtx * shModelMtx;//model to projection

	//Set Uniform
	GLCALL(glUniformMatrix4fv(m_u_MVPMtxLoc, 1, GL_FALSE, &MVPMtx[0][0]));//model to projection
	GLCALL(glUniformMatrix4fv(m_u_MWMtxLoc, 1, GL_FALSE, &shModelMtx[0][0]));//model to world

	//Draw shape-5|torus
	GLCALL(glDrawElements(GL_TRIANGLES, m_Sh5NumIndcs, GL_UNSIGNED_SHORT,
		(const void*)m_Sh5IndxOff));

	/* Bind VAO */
	GLCALL(glBindVertexArray(m_Sh6Vao));

	/* Bind IBO */
	//GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Sh2Ibo));

	//Shape-6|sphere
	shModelMtx = glm::translate(glm::vec3(2.0f, 2.0f, 2.0f));//model to world
	MVPMtx = VPMtx * shModelMtx;//model to projection

	//Set Uniform
	GLCALL(glUniformMatrix4fv(m_u_MVPMtxLoc, 1, GL_FALSE, &MVPMtx[0][0]));//model to projection
	GLCALL(glUniformMatrix4fv(m_u_MWMtxLoc, 1, GL_FALSE, &shModelMtx[0][0]));//model to world

	//Draw shape-6|sphere
	GLCALL(glDrawElements(GL_TRIANGLES, m_Sh6NumIndcs, GL_UNSIGNED_SHORT,
		(const void*)m_Sh6IndxOff));

	/* Select shader program */
	GLCALL(glUseProgram(m_ProgramPT));

	/* Bind VAO */
	GLCALL(glBindVertexArray(m_Sh4Vao));

	/* Bind IBO */
	//GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Sh2Ibo));

	//Shape-4|cube/sphere|light source
	shModelMtx = glm::translate(lightPos) * glm::scale(glm::vec3(0.25f, 0.25f, 0.25f));//model to world
	MVPMtx = VPMtx * shModelMtx;//model to projection

	//Set Uniform
	GLCALL(glUniformMatrix4fv(m_u_MVPMtxPTLoc, 1, GL_FALSE, &MVPMtx[0][0]));//model to projection

	//Draw shape-4|cube/sphere|light source
	GLCALL(glDrawElements(GL_TRIANGLES, /*m_Sh4NumIndcs*/m_Sh6NumIndcs, GL_UNSIGNED_SHORT,
		(const void*)/*m_Sh4IndxOff*/m_Sh6IndxOff));

	GLCALL(glDisable(GL_DEPTH_TEST));
}

void DrawWidget::mouseMoveEvent(QMouseEvent* e)
{
	m_pCamera->mouseUpdate(glm::vec2(e->x(), e->y()));
	repaint();
}

void DrawWidget::keyPressEvent(QKeyEvent* e)
{
	switch(e->key())
	{
	case Qt::Key::Key_W:
		m_pCamera->moveForward();
		break;
	case Qt::Key::Key_S:
		m_pCamera->moveBackward();
		break;
	case Qt::Key::Key_A:
		m_pCamera->strafeLeft();
		break;
	case Qt::Key::Key_D:
		m_pCamera->strafeRight();
		break;
	case Qt::Key::Key_R:
		m_pCamera->moveUp();
		break;
	case Qt::Key::Key_F:
		m_pCamera->moveDown();
		break;
	}
	repaint();
}

/*
	glm::mat4 projMtx = glm::perspective(glm::radians(60.0f),
		((float)width()) / height(), 0.1f, 10.0f);
	glm::mat4 fullTxMtx[] =
	{
		//teapot 1
		projMtx * m_pCamera->getWorldToViewMtx() *
		glm::translate(glm::vec3(-1.0f, 0.0f, -3.0f)) *
		glm::rotate(glm::radians(36.0f), glm::vec3(1.0f, 0.0f, 0.0f)),

		//teapot 2
		projMtx * m_pCamera->getWorldToViewMtx() *
		glm::translate(glm::vec3(1.5f, 0.0f, -3.75f)) *
		glm::rotate(glm::radians(126.0f), glm::vec3(0.0f, 1.0f, 0.0f))
	};

	GLCALL(glBindBuffer(GL_ARRAY_BUFFER, m_Mtxbo));
	GLCALL(glBufferData(GL_ARRAY_BUFFER,
		sizeof(fullTxMtx), fullTxMtx, GL_DYNAMIC_DRAW));

	/* Draw elements */
//GLCALL(glDrawElementsInstanced(GL_TRIANGLES, m_NumIndcs, GL_UNSIGNED_SHORT, 0, 2));
//GLCALL(glDrawElements(GL_TRIANGLES, m_NumIndcs, GL_UNSIGNED_SHORT, 0));*/
//drawIncTriangle();
//GLCALL(glDrawArrays(GL_TRIANGLES, 0, m_NumTri * NUM_VERTICES_PER_TRI));

/*glm::vec3 u_color(1.0f, 0.0f, 0.0f);
GLCALL(GLint u_colorLoc = glGetUniformLocation(m_Program, "u_color"));
GLCALL(GLint u_yFlipLoc = glGetUniformLocation(m_Program, "u_yFlip"));

GLCALL(glUniform3fv(u_colorLoc, 1, &u_color[0]));
GLCALL(glUniform1f(u_yFlipLoc, 1.0f));*/
/*
//Optimal memory usage by initializing using each other matrices
//preserving the order of multiplication
glm::mat4 projMtx = glm::perspective(glm::radians(60.0f),
	((float)width()) / height(), 0.1f, 10.0f);

glm::mat4 transMtx = glm::translate(projMtx, glm::vec3(0.0f, 0.0f, -3.0f));

glm::mat4 u_fullTxMtx = glm::rotate(transMtx, glm::radians(54.0f),
	glm::vec3(1.0f, 0.0f, 0.0f));

GLCALL(GLint u_fullTxMtxLoc = glGetUniformLocation(m_Program, "u_fullTxMtx"));

//Done only to learn | not optimal memory wise
glm::mat4 projMtx = glm::perspective(glm::radians(60.0f),
	((float)width()) / height(), 0.1f, 10.0f);

//teapot 1
glm::mat4 transMtx = glm::translate(glm::vec3(-1.0f, 0.0f, -3.0f));

glm::mat4 rotMtx = glm::rotate(glm::radians(36.0f),
	glm::vec3(1.0f, 0.0f, 0.0f));

glm::mat4 u_fullTxMtx = projMtx * transMtx * rotMtx;

GLCALL(glUniformMatrix4fv(u_fullTxMtxLoc, 1, GL_FALSE, &u_fullTxMtx[0][0]));
*/