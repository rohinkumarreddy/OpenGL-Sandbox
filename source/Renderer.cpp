#include "common.h"

//#include "glm/glm.hpp"//included in ShapeGenerator
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"
#include <glm/gtc/type_ptr.hpp>

#include "Renderer.h"
#include "Mesh.h"
#include "Model.h"
#include "Texture.h"
#include "Shader.h"
#include "Camera.h"
#include "lightData.h"
#include "SpotLight.h"
#include "DirectionalLight.h"

//#define _2D_DRAW_

#ifdef _2D_DRAW_
void perspectiveGL(GLdouble fovY, GLdouble aspect, GLdouble zNear, GLdouble zFar)
{
	const GLdouble pi = 3.1415926535897932384626433832795;
	GLdouble fW, fH;

	//fH = tan( (fovY / 2) / 180 * pi ) * zNear;
	fH = tan(fovY / 360 * pi) * zNear;
	fW = fH * aspect;

	glFrustum(-fW, fW, -fH, fH, zNear, zFar);
}

//Compat method: gluLookAt deprecated
void util_compat_gluLookAt(GLfloat eyeX, GLfloat eyeY, GLfloat eyeZ, GLfloat lookAtX, GLfloat lookAtY, GLfloat lookAtZ, GLfloat upX, GLfloat upY, GLfloat upZ) {
	glm::vec3 X, Y, Z;
	Z = glm::normalize(glm::vec3(eyeX - lookAtX, eyeY - lookAtY, eyeZ - lookAtZ));
	Y = glm::vec3(upX, upY, upZ);
	X = glm::cross(Y , Z);
	Y = glm::cross(Z , X);
	X = glm::normalize(X);
	Y = glm::normalize(Y);
	// mat is given transposed so OpenGL can handle it.
	glm::mat4x4 mat(X.x, Y.x,   Z.x,   0,
					X.y, Y.y,   Z.y,   0,
					X.z, Y.z,   Z.z,   0,
					-eyeX,     -eyeY,      -eyeZ,      1 );
	glMultMatrixf(glm::value_ptr(mat));
}
#endif

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
	m_pModel1(new Model),
	m_pModel2(new Model),
	m_pTex1(new Texture("Textures/brick.png")),
	m_pTex2(new Texture("Textures/plain.png")),
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
	m_spotLightVec.push_back(new SpotLight());
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

	//clean up Models
	if (m_pModel1 != nullptr)
		delete m_pModel1;
	if (m_pModel2 != nullptr)
		delete m_pModel2;

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
	if (!m_spotLightVec.empty())
	{
		for each (auto var in m_spotLightVec)
		{
			if (var != nullptr)
				delete var;
			var = nullptr;
		}
		m_spotLightVec.clear();
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
		"u_pointLightCount",
		"u_spotLightCount"
	});

	/* Add light sources */
	m_pTexShader->addLightSource(m_pDirectionalLight);
	for each (auto var in m_PointLightVec)
	{
		m_pTexShader->addLightSource(var);
	}
	for each (auto var in m_spotLightVec)
	{
		m_pTexShader->addLightSource(var);
	}

	/* Select shader */
	m_pShaderPT->activate();

	/* Query uniform */
	m_pShaderPT->initUniforms(std::vector<std::string>{"u_MVPMtx"});

	m_pModel1->LoadModel("D:/Workplace/OpenGLTuT/Udemy-BenCook/Sandbox/Models/ISS_2016.obj");//uh60.obj");
	m_pModel2->LoadModel("D:/Workplace/OpenGLTuT/Udemy-BenCook/Sandbox/Models/helmet.obj");//x-wing.obj");
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
	/* Create Meshes */
	m_pMesh1->createMesh(Mesh::shapeType::_TEAPOT_);
	m_pMesh2->createMesh(Mesh::shapeType::_ARROW_);
	m_pMesh3->createMesh(Mesh::shapeType::_PLANE_);
	m_pMesh4->createMesh(Mesh::shapeType::_CUBE_);
	m_pMesh5->createMesh(Mesh::shapeType::_TORUS_);
	m_pMesh6->createMesh(Mesh::shapeType::_SPHERE_);
	m_pTexMesh->createMesh(Mesh::shapeType::_CUBE_);

	/* Load Textures */
	m_pTex1->loadTexture();
	m_pTex2->loadTexture();
	m_pTex3->loadTexture();
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
	glm::mat4 projMtx = glm::perspective(glm::radians(60.0f),
						((float)m_width) / m_height, 0.1f, 100.0f);//view to projection
	glm::mat4 viewMtx = m_pCamera->getWorldToViewMtx();//world to view
	glm::mat4 VPMtx = projMtx * viewMtx;//world to projection
	glm::vec3 eyePos = m_pCamera->getPosition();//eye position|world space
	glm::vec3 viewDir = m_pCamera->getViewDirection();//view direction|world space

	//Ambient-light|Diffused-light
	m_pDirectionalLight->setIntensity(0.005f, 0.3f);
	m_pDirectionalLight->setColor(1.0f, 1.0f, 1.0f);

	//Light-source
	if (m_pLightData)
	{
		glm::vec3 skylightDir = glm::vec3(0.0f, -1.0f, 0.0f);//(glm::vec3(0.0f, 0.0f, 0.0f) - glm::vec3(m_pLightData->pos.x, 20, m_pLightData->pos.z));
		skylightDir /= skylightDir.length();
		for each (auto var in m_PointLightVec)
		{
			var->setIntensity(0.005f, 0.2f);
			var->setPosition(m_pLightData->pos.x + (pow(-1, var->getLightIndex()) * (var->getLightIndex() > 0) * 9.0f),
				m_pLightData->pos.y,
				m_pLightData->pos.z + (pow(-1, var->getLightIndex()) * (var->getLightIndex() > 0) * 9.0f));
			//Light attenuation(kC, kL, kQ)
			var->setAttenuation(m_pLightData->attenuation);
		}
		for each (auto var in m_spotLightVec)
		{
			var->setIntensity(0.005f, 0.05f);
			var->setPosition(m_pLightData->pos.x,
							 m_pLightData->pos.y,
							 m_pLightData->pos.z);
							 //Light attenuation(kC, kL, kQ)
			//var->setAttenuation(m_pLightData->attenuation);
			var->setAttenuation(glm::vec4(0.2,0,0,0));
			var->setDirection(skylightDir);
			var->setEdge(12.0f);
		}
		m_pDirectionalLight->setDirection(skylightDir);//(glm::vec3(0.0f, 0.0f, -1.0f));
	}

	/*std::cout << "view dir : (" << viewDir.x
					<< ", " << viewDir.y
					<< ", " << viewDir.z << ")\n";*/

	if (!m_spotLightVec.empty() && m_pCamera != nullptr)
	{
		m_spotLightVec[0]->SetFlash(eyePos,
									viewDir);
	}

	/* Select texture */
	m_pTex1->activate();

	/* Select shader program */
	m_pTexShader->activate();

	m_pTexShader->setUniform("u_pointLightCount", PointLight::getLightCount());
	m_pTexShader->setUniform("u_spotLightCount", SpotLight::getLightCount());
	m_pTexShader->setUniform("u_material.specularIntensity", 4.0f);
	m_pTexShader->setUniform("u_material.shininess", 256.0f);

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
	for each (auto var in m_spotLightVec)
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
	//shModelMtx = glm::translate(glm::vec3(20.0f, 0.0f, 20.0f)) *
	//	glm::scale(glm::vec3(50.0f, 50.0f, 50.0f));//model to world
	MVPMtx = VPMtx * shModelMtx;//model to projection

	//Set Uniform
	m_pTexShader->setUniform("u_MVPMtx", MVPMtx);//model to projection
	m_pTexShader->setUniform("u_MWMtx", shModelMtx);//model to world

	m_pMesh3->renderMesh();

	//
	//model = glm::mat4(1.0f);
	//model = glm::translate(model, glm::vec3(-7.0f, 0.0f, 10.0f));
	//model = glm::scale(model, glm::vec3(0.006f, 0.006f, 0.006f));
	//
	/*shModelMtx = glm::translate(glm::vec3(2.0f, 0.5f, 0.0f)) *
				 glm::scale(glm::vec3(0.125f, 0.125f, 0.125f)) *
				 glm::rotate(glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f)) *
				 glm::rotate(glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));*/
	shModelMtx = glm::translate(glm::vec3(2.0f, 2.0f, 0.0f)) *
				 glm::rotate(glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f)) *
				 glm::scale(glm::vec3(0.001f, 0.001f, 0.001f));//model to world
	MVPMtx = VPMtx * shModelMtx;//model to projection

	//Set Uniform
	m_pTexShader->setUniform("u_MVPMtx", MVPMtx);//model to projection
	m_pTexShader->setUniform("u_MWMtx", shModelMtx);//model to world
	m_pModel1->RenderModel();
	
	shModelMtx = glm::translate(glm::vec3(-4.0f, 0.5f, 4.0f)) *
				 glm::scale(glm::vec3(0.003f, 0.003f, 0.003f));//model to world
	MVPMtx = VPMtx * shModelMtx;//model to projection

	//Set Uniform
	m_pTexShader->setUniform("u_MVPMtx", MVPMtx);//model to projection
	m_pTexShader->setUniform("u_MWMtx", shModelMtx);//model to world
	m_pModel2->RenderModel();

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

	//m_pMesh1->renderMesh();
#if 1
	/* Select shader program */
	m_pShaderPT->activate();

	for each (auto var in m_PointLightVec)
	{
		//Shape-4|sphere|light source
		shModelMtx = glm::translate(var->getPosition()) * glm::scale(glm::vec3(0.0625f, 0.0625f, 0.0625f));//model to world
		MVPMtx = VPMtx * shModelMtx;//model to projection

		//Set Uniform
		m_pShaderPT->setUniform("u_MVPMtx", MVPMtx);//model to projection

		m_pMesh6->renderMesh();
	}
#endif

#ifdef _2D_DRAW_
	Texture::deActivate();

	glm::vec3 lookAtPos = eyePos + m_pCamera->getViewDirection();
	glm::vec3 upDir = m_pCamera->getUpDirection();

	//Test skylightDir
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	glMatrixMode(GL_PROJECTION);
	//gluPerspective(45.0f, ((float)m_width / (float)m_height), 0.1, 100.0f);
	perspectiveGL(45.0f, ((float)m_width / (float)m_height), 0.1, 100.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	util_compat_gluLookAt(eyePos.x, eyePos.y, eyePos.z,
			  lookAtPos.x, lookAtPos.y, lookAtPos.z,
			  upDir.x, upDir.y, upDir.z);
	/*gluLookAt(eyePos.x, eyePos.y, eyePos.z,
			  lookAtPos.x, lookAtPos.y, lookAtPos.z,
			  upDir.x, upDir.y, upDir.z);*/
	//glTranslatef(0.0f, 0.0f, 0.0f);
	glColor3f(1, 1, 1);
	glLineWidth(5);
	glBegin(GL_LINES);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 1, 0);
	glEnd();
	glLineWidth(1);
	glPopMatrix();
	//
#endif

	GLCALL(glDisable(GL_DEPTH_TEST));
}
