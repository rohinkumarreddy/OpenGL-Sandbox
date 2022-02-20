#include "common.h"

//#include "glm/glm.hpp"//included in ShapeGenerator
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"

#include "Renderer.h"
#include "Shader.h"
#include "Camera.h"
#include "ShapeGenerator.h"
#include "lightData.h"

#define X_DELTA 0.1f
#define MAX_TRIS 20
const unsigned int NUM_VERTICES_PER_TRI = 3;
const unsigned int NUM_FLOATS_PER_VTX = 9;
const unsigned int VERTEX_BYTE_SIZE = NUM_FLOATS_PER_VTX * sizeof(float);
const unsigned int TRIANGLE_BYTE_SIZE = NUM_VERTICES_PER_TRI * NUM_FLOATS_PER_VTX * sizeof(float);

Renderer::Renderer(unsigned int width, unsigned int height, float pixelRatio)
	:
	m_pShader(nullptr),
	m_pShaderPT(nullptr),
	m_pLightData(nullptr),
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
	m_NumTri(1),
	m_Sh1NumIndcs(NULL),
	m_Sh2NumIndcs(NULL),
	m_Sh3NumIndcs(NULL),
	m_Sh4NumIndcs(NULL),
	m_Sh5NumIndcs(NULL),
	m_Sh6NumIndcs(NULL),
	m_curTime(clock()),
	m_prevTime(NULL),
	m_timeDelta(NULL),
	m_width(width),
	m_height(height),
	m_PixRatio(pixelRatio),
	m_pCamera(nullptr)
{
}

Renderer::~Renderer()
{
	/* GLCALL results in error here */

	//Clean-up vertex & index buffers
	if (m_VIbo)
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

	//Disable active shaders
	Shader::deactivate();

	//Clean-up shader programs
	if (m_pShader != nullptr)
		delete m_pShader;
	m_pShader = nullptr;

	//Clean-up pass-through shader
	if (m_pShaderPT != nullptr)
		delete m_pShaderPT;
	m_pShaderPT = nullptr;
}

void Renderer::initialize()
{
	/* Initialize GLEW */
	if (glewInit() != GLEW_OK)
		std::cerr << "[Error]: glewInit failed!" << std::endl;

	/* Print out OpenGL version */
	GLCALL(std::cout << "[Info]: opengl version "
		<< glGetString(GL_VERSION) << std::endl);

	/* Enable depth buffer */
	//GLCALL(glEnable(GL_DEPTH_TEST));

	/* Shading model - optional */
	GLCALL(glShadeModel(GL_SMOOTH));

	/* 4-byte pixel alignment to generate ARGB texture */
	GLCALL(glPixelStorei(GL_UNPACK_ALIGNMENT, 4));

	/* Initialize OpenGL data */
	initOpenGLData();

	/* Initialize vertex arrays */
	initVertexArrays();

	/* Initialize shaders */
	installShader();

	/* Select shader */
	m_pShader->activate();

	/* Query uniform */
	m_pShader->initUniforms(std::vector<std::string>{"u_MVPMtx",
		"u_ambientLight",
		"u_lightPos",
		"u_eyePos",
		"u_MWMtx",
		"u_lAttenuationFac"});

	/* Select shader */
	m_pShaderPT->activate();

	/* Query uniform */
	m_pShaderPT->initUniforms(std::vector<std::string>{"u_MVPMtx"});
}

/*Shader code*/
void Renderer::installShader()
{
	m_pShader = new Shader((char*)"shaders/VertexShader.glsl", (char*)"shaders/FragmentShader.glsl");
	if (!m_pShader->build())
		return;

	m_pShaderPT = new Shader((char*)"shaders/VertexShaderPT.glsl", (char*)"shaders/FragmentShaderPT.glsl");
	if (!m_pShaderPT->build())
		return;
}

void Renderer::initVertexArrays()
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

void Renderer::initOpenGLData()
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

void Renderer::draw()
{
	/* update timing */
	m_curTime = clock();
	//if (m_prevTime > 0)
	{
		float timeDelta = (float)(m_curTime - m_prevTime) / (float)CLOCKS_PER_SEC;
		if (timeDelta > 1.0f / 30.0f)
			timeDelta = 1.0f / 30.0f;
		m_timeDelta = timeDelta;
		if (m_pCamera)
			m_pCamera->setTimeDelta(timeDelta);
		//std::cout << "time delta " << timeDelta << std::endl;
	}
	m_prevTime = m_curTime;//update previous time

	GLCALL(glEnable(GL_DEPTH_TEST));
	GLCALL(glEnable(GL_CULL_FACE));

	/* Clear depth buffer */
	GLCALL(glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT));

	/* Viewport */
	GLCALL(glViewport(0, 0, m_width * m_PixRatio, m_height * m_PixRatio));

	/* Matrix transformations */
	//Done only to learn | not optimal memory wise
	glm::mat4 MVPMtx = glm::mat4(1.0f);
	glm::mat4 projMtx = glm::perspective(glm::radians(60.0f),
		((float)m_width) / m_height, 0.1f, 20.0f);//view to projection
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

	/* Select shader */
	m_pShader->activate();

	//Set Uniform
	m_pShader->setUniform("u_ambientLight", ambientLight);
	m_pShader->setUniform("u_eyePos", eyePos);
	m_pShader->setUniform("u_lightPos", lightPos);
	m_pShader->setUniform("u_lAttenuationFac", lightAttn);

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
	m_pShader->setUniform("u_MVPMtx", MVPMtx);//model to projection
	m_pShader->setUniform("u_MWMtx", shModelMtx);//model to world

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
	m_pShader->setUniform("u_MVPMtx", MVPMtx);//model to projection
	m_pShader->setUniform("u_MWMtx", shModelMtx);//model to world

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
	m_pShader->setUniform("u_MVPMtx", MVPMtx);//model to projection
	m_pShader->setUniform("u_MWMtx", shModelMtx);//model to world

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
	m_pShader->setUniform("u_MVPMtx", MVPMtx);//model to projection
	m_pShader->setUniform("u_MWMtx", shModelMtx);//model to world

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
	m_pShader->setUniform("u_MVPMtx", MVPMtx);//model to projection
	m_pShader->setUniform("u_MWMtx", shModelMtx);//model to world

	//Draw shape-6|sphere
	GLCALL(glDrawElements(GL_TRIANGLES, m_Sh6NumIndcs, GL_UNSIGNED_SHORT,
		(const void*)m_Sh6IndxOff));

	/* Select shader program */
	m_pShaderPT->activate();

	/* Bind VAO */
	GLCALL(glBindVertexArray(m_Sh4Vao));

	/* Bind IBO */
	//GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Sh2Ibo));

	//Shape-4|cube/sphere|light source
	shModelMtx = glm::translate(lightPos) * glm::scale(glm::vec3(0.25f, 0.25f, 0.25f));//model to world
	MVPMtx = VPMtx * shModelMtx;//model to projection

	//Set Uniform
	m_pShaderPT->setUniform("u_MVPMtx", MVPMtx);//model to projection

	//Draw shape-4|cube/sphere|light source
	GLCALL(glDrawElements(GL_TRIANGLES, /*m_Sh4NumIndcs*/m_Sh6NumIndcs, GL_UNSIGNED_SHORT,
		(const void*)/*m_Sh4IndxOff*/m_Sh6IndxOff));

	GLCALL(glDisable(GL_DEPTH_TEST));
}