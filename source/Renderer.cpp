#include "common.h"

//#include "glm/glm.hpp"//included in ShapeGenerator
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"

#include "Renderer.h"
#include "Mesh.h"
#include "Texture.h"
#include "Shader.h"
#include "Camera.h"
#include "ShapeGenerator.h"
#include "lightData.h"
#include "PointLight.h"
#include "DirectionalLight.h"

Renderer::Renderer(unsigned int width, unsigned int height, float pixelRatio)
	:
	m_pShader(nullptr),
	m_pTexShader(nullptr),
	m_pShaderPT(nullptr),
	m_pLightData(nullptr),
	m_pMesh1(new Mesh),
	m_pMesh2(new Mesh),
	m_pMesh3(new Mesh),
	m_pMesh4(new Mesh),
	m_pMesh5(new Mesh),
	m_pMesh6(new Mesh),
	m_pTexMesh(new Mesh),
	m_pTex1(new Texture("Textures/brick.png")),
	m_pTex2(new Texture("Textures/bricks.png")),
	m_pTex3(new Texture("Textures/tiles.png")),
	m_timeDelta(NULL),
	m_width(width),
	m_height(height),
	m_PixRatio(pixelRatio),
	m_pCamera(nullptr),
	m_pDirectionalLight(new DirectionalLight())
{
	m_PointLightVec.push_back(new PointLight());
	m_PointLightVec.push_back(new PointLight());
	m_PointLightVec.push_back(new PointLight());
}

Renderer::~Renderer()
{
	/* GLCALL results in error here */

	//Clean-up Meshes
	if (m_pMesh1 != nullptr)
		delete m_pMesh1;
	m_pMesh1 = nullptr;
	if (m_pMesh2 != nullptr)
		delete m_pMesh2;
	m_pMesh2 = nullptr;
	if (m_pMesh3 != nullptr)
		delete m_pMesh3;
	m_pMesh3 = nullptr;
	if (m_pMesh4 != nullptr)
		delete m_pMesh4;
	m_pMesh4 = nullptr;
	if (m_pMesh5 != nullptr)
		delete m_pMesh5;
	m_pMesh5 = nullptr;
	if (m_pMesh6 != nullptr)
		delete m_pMesh6;
	m_pMesh6 = nullptr;
	if (m_pTexMesh != nullptr)
		delete m_pTexMesh;
	m_pTexMesh = nullptr;

	//Clean-up Textures
	if (m_pTex1 != nullptr)
		delete m_pTex1;
	m_pTex1 = nullptr;
	if (m_pTex2 != nullptr)
		delete m_pTex2;
	m_pTex2 = nullptr;
	if (m_pTex3 != nullptr)
		delete m_pTex3;
	m_pTex3 = nullptr;

	//Disable active shaders
	Shader::deactivate();

	//Clean-up main shader
	if (m_pShader != nullptr)
		delete m_pShader;
	m_pShader = nullptr;

	//Clean-up texture shader
	if (m_pTexShader != nullptr)
		delete m_pTexShader;
	m_pTexShader = nullptr;

	//Clean-up pass-through shader
	if (m_pShaderPT != nullptr)
		delete m_pShaderPT;
	m_pShaderPT = nullptr;

	//Clean-up lights
	if (!m_PointLightVec.empty())
	{
		for each (auto var in m_PointLightVec)
		{
			if (var != nullptr)
				delete var;
			var = nullptr;
		}
		m_PointLightVec.clear();
	}
	if (m_pDirectionalLight != nullptr)
		delete m_pDirectionalLight;
	m_pDirectionalLight = nullptr;
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

	/* Setup initial scene */
	setupScene();

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
	m_pTexShader->activate();

	/* Query uniform */
	m_pTexShader->initUniforms(std::vector<std::string>
	{
		"u_MVPMtx",
		"u_eyePos",
		"u_MWMtx",
		"u_material.specularIntensity",
		"u_material.shininess",
		"u_pointLightCount"
	});

	/* Add light sources */
	m_pTexShader->addLightSource(m_pDirectionalLight);
	for each (auto var in m_PointLightVec)
	{
		m_pTexShader->addLightSource(var);
	}

	/* Select shader */
	m_pShaderPT->activate();

	/* Query uniform */
	m_pShaderPT->initUniforms(std::vector<std::string>{"u_MVPMtx"});
}

/*Shader code*/
void Renderer::installShader()
{
	m_pShader = new Shader((char*)"shaders/VertexShaderNoTex.glsl", (char*)"shaders/FragmentShaderNoTex.glsl");
	if (!m_pShader->build())
		return;

	m_pTexShader = new Shader((char*)"shaders/VertexShader.glsl", (char*)"shaders/FragmentShader.glsl");
	if (!m_pTexShader->build())
		return;

	m_pShaderPT = new Shader((char*)"shaders/VertexShaderPT.glsl", (char*)"shaders/FragmentShaderPT.glsl");
	if (!m_pShaderPT->build())
		return;
}

void Renderer::setupScene()
{
	/* Shape Object */
	ShapeData teapot = ShapeGenerator::makeTeapot();
	ShapeData arrow = ShapeGenerator::makeArrow();
	ShapeData plane = ShapeGenerator::makePlane(100,2);
	ShapeData cube = ShapeGenerator::makeCube();
	ShapeData torus = ShapeGenerator::makeTorus(50);
	ShapeData sphere = ShapeGenerator::makeSphere(50);
	ShapeData texCube = ShapeGenerator::makeCube();

	/* Create Meshes */
	m_pMesh1->createMesh(teapot.vertices, teapot.indices, teapot.numVertices, teapot.numIndices, MeshAttribute::MeshAttributeProfile::POS_COL_TEX_NOR);
	m_pMesh2->createMesh(arrow.vertices, arrow.indices, arrow.numVertices, arrow.numIndices, MeshAttribute::MeshAttributeProfile::POS_COL_TEX_NOR);
	m_pMesh3->createMesh(plane.vertices, plane.indices, plane.numVertices, plane.numIndices, MeshAttribute::MeshAttributeProfile::POS_COL_TEX_NOR);
	m_pMesh4->createMesh(cube.vertices, cube.indices, cube.numVertices, cube.numIndices, MeshAttribute::MeshAttributeProfile::POS_COL_TEX_NOR);
	m_pMesh5->createMesh(torus.vertices, torus.indices, torus.numVertices, torus.numIndices, MeshAttribute::MeshAttributeProfile::POS_COL_TEX_NOR);
	m_pMesh6->createMesh(sphere.vertices, sphere.indices, sphere.numVertices, sphere.numIndices, MeshAttribute::MeshAttributeProfile::POS_COL_TEX_NOR);
	m_pTexMesh->createMesh(texCube.vertices, texCube.indices, texCube.numVertices, texCube.numIndices, MeshAttribute::MeshAttributeProfile::POS_COL_TEX_NOR);
	/*m_pMesh1->createMesh(teapot.vertices, teapot.indices, teapot.numVertices, teapot.numIndices);
	m_pMesh2->createMesh(arrow.vertices, arrow.indices, arrow.numVertices, arrow.numIndices);
	m_pMesh3->createMesh(plane.vertices, plane.indices, plane.numVertices, plane.numIndices);
	m_pMesh4->createMesh(cube.vertices, cube.indices, cube.numVertices, cube.numIndices);
	m_pMesh5->createMesh(torus.vertices, torus.indices, torus.numVertices, torus.numIndices);
	m_pMesh6->createMesh(sphere.vertices, sphere.indices, sphere.numVertices, sphere.numIndices);
	m_pTexMesh->createMesh(texCube.vertices, texCube.indices, texCube.numVertices, texCube.numIndices);*/

	/* Load Textures */
	m_pTex1->loadTexture();
	m_pTex2->loadTexture();
	m_pTex3->loadTexture();

	/* Clean up */
	teapot.cleanUp();
	arrow.cleanUp();
	plane.cleanUp();
	cube.cleanUp();
	torus.cleanUp();
	sphere.cleanUp();
	texCube.cleanUp();

	/*
	* Attribute data can also be sent to shader
	* using glVertexAttrib3f(attribId, <data>)
	* without enabling the corresponding attrib-Array.
	* Note: can be used to switch from solid color & dynamic color etc.
	*/
}

void Renderer::draw()
{
	if (m_pCamera)
		m_pCamera->update();

	GLCALL(glEnable(GL_DEPTH_TEST));
	GLCALL(glEnable(GL_CULL_FACE));

	/* Clear depth buffer */
	GLCALL(glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT));

	/* Viewport */
	GLCALL(glViewport(0, 0, m_width * m_PixRatio, m_height * m_PixRatio));

	/* Matrix transformations */
	//Done only to learn | not optimal memory wise
	glm::mat4 MVPMtx = glm::mat4(1.0f);
	glm::mat4 projMtx = glm::perspective(glm::radians(45.0f),
						((float)m_width) / m_height, 0.1f, 100.0f);//view to projection
	glm::mat4 viewMtx = m_pCamera->getWorldToViewMtx();//world to view
	glm::mat4 VPMtx = projMtx * viewMtx;//world to projection
	glm::vec3 eyePos = m_pCamera->getPosition();//eye position|world space

	//Ambient-light|Diffused-light
	m_pDirectionalLight->setIntensity(0.15f, 0.15f);
	m_pDirectionalLight->setColor(1.0f, 1.0f, 1.0f);

	//Light-source
	if (m_pLightData)
	{
		glm::vec3 skylightDir = (m_pLightData->pos - glm::vec3(0.0f, 0.0f, 0.0f));
		skylightDir /= skylightDir.length();
		for each (auto var in m_PointLightVec)
		{
			var->setIntensity(0.55f, 0.55f);
			var->setPosition(m_pLightData->pos.x + (pow(-1, var->getLightIndex()) * (var->getLightIndex()>0) * 9.0f),
							 m_pLightData->pos.y,
							 m_pLightData->pos.z + (pow(-1, var->getLightIndex()) * (var->getLightIndex()>0) * 9.0f));
			//Light attenuation(kC, kL, kQ)
			var->setAttenuation(m_pLightData->attenuation);
			/*switch (var->getLightIndex())
			{
			case 1:
				var->setColor(0, 1.0f, 0);
				break;
			case 2:
				var->setColor(0, 0, 1.0f);
				break;
			case 0:default:
				var->setColor(1.0f, 0, 0);
				break;
			}*/
		}
		m_pDirectionalLight->setDirection(skylightDir);//(glm::vec3(0.0f, 0.0f, -1.0f));
	}

	/* Select texture */
	m_pTex1->activate();

	/* Select shader program */
	m_pTexShader->activate();

	m_pTexShader->setUniform("u_pointLightCount", PointLight::getLightCount());
	m_pTexShader->setUniform("u_material.specularIntensity", 1.0f);
	m_pTexShader->setUniform("u_material.shininess", 50.0f);

	//Shape-5|cube
	glm::mat4 shModelMtx = glm::translate(glm::vec3(0.5f, 0.25f, 1.0f)) *
								glm::scale(glm::vec3(0.25f, 0.25f, 0.25f));//model to world
	MVPMtx = VPMtx * shModelMtx;//model to projection

	//Set Uniform
	m_pTexShader->setUniform("u_eyePos", eyePos);
	m_pTexShader->setUniform("u_MVPMtx", MVPMtx);//model to projection
	m_pTexShader->setUniform("u_MWMtx", shModelMtx);//model to world
	//Set-up light
	for each (auto var in m_PointLightVec)
	{
		var->UseLight(m_pTexShader);
	}
	m_pDirectionalLight->UseLight(m_pTexShader);

	m_pTexMesh->renderMesh();

	//Shape-6|sphere
	shModelMtx = glm::translate(glm::vec3(1.0f, 0.25f, -0.5f)) *
								glm::scale(glm::vec3(0.25f, 0.25f, 0.25f));//model to world
	MVPMtx = VPMtx * shModelMtx;//model to projection

	//Set Uniform
	m_pTexShader->setUniform("u_MVPMtx", MVPMtx);//model to projection
	m_pTexShader->setUniform("u_MWMtx", shModelMtx);//model to world

	m_pMesh6->renderMesh();
#if 0
	//Shape-5|torus
	shModelMtx = glm::translate(glm::vec3(0.0f, 0.15f, 0.0f)) *
								glm::scale(glm::vec3(0.25f, 0.25f, 0.25f));//model to world
	MVPMtx = VPMtx * shModelMtx;//model to projection

	//Set Uniform
	m_pTexShader->setUniform("u_MVPMtx", MVPMtx);//model to projection
	m_pTexShader->setUniform("u_MWMtx", shModelMtx);//model to world

	m_pMesh5->renderMesh();
#endif
	/* Select texture */
	m_pTex2->activate();
	//Shape-3|plane
	shModelMtx = glm::mat4(1.0f);//model to world
	MVPMtx = VPMtx * shModelMtx;//model to projection

	//Set Uniform
	m_pTexShader->setUniform("u_MVPMtx", MVPMtx);//model to projection
	m_pTexShader->setUniform("u_MWMtx", shModelMtx);//model to world

	m_pMesh3->renderMesh();

	/* Select texture */
	m_pTex3->activate();
	//teapot-1
	shModelMtx = glm::translate(glm::vec3(-0.5f, 0.0f, -1.0f)) *
								glm::scale(glm::vec3(0.25f, 0.25f, 0.25f)) *
								glm::rotate(glm::radians(-90.0f),
								glm::vec3(1.0f, 0.0f, 0.0f));//model to world
	MVPMtx = VPMtx * shModelMtx;//model to projection

	//Set Uniform
	m_pTexShader->setUniform("u_MVPMtx", MVPMtx);//model to projection
	m_pTexShader->setUniform("u_MWMtx", shModelMtx);//model to world

	m_pMesh1->renderMesh();
#if 0
	/* Select shader program */
	m_pShaderPT->activate();

	//Shape-4|sphere|light source
	shModelMtx = glm::translate(m_PointLightVec[0]->getPosition()) * glm::scale(glm::vec3(0.0625f, 0.0625f, 0.0625f));//model to world
	MVPMtx = VPMtx * shModelMtx;//model to projection

	//Set Uniform
	m_pShaderPT->setUniform("u_MVPMtx", MVPMtx);//model to projection

	m_pMesh6->renderMesh();
#endif
	GLCALL(glDisable(GL_DEPTH_TEST));
}