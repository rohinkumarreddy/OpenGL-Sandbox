#pragma once

#include <vector>

//forward declaration
class Model;
class Mesh;
class Shader;
class lightData;
class Camera;
class Texture;
class PointLight;
class SpotLight;
class DirectionalLight;
class Material;

class Renderer
{
public:
	Renderer(unsigned int width, unsigned int height, float pixelRatio=0.0);
	~Renderer();
	void initialize();
	void draw();
	/* Setters */
	inline void setWidth(unsigned int width) { m_width = width; }
	inline void setHeight(unsigned int height) { m_height = height; }
	inline void setPixelRatio(float pixelRatio) { m_PixRatio = pixelRatio; }
	inline void setCameraRef(Camera* icamera) { m_pCamera = icamera; }
	inline void setLDataPtr(lightData* vlightData) { m_pLightData = vlightData; }
private:
	void installShader();
	void setupScene();
	Shader* m_pShader;				//main shader
	Shader* m_pTexShader;			//texture shader
	Shader* m_pShaderPT;			//pass-through shader
	lightData* m_pLightData;		//light data ptr
	std::vector<PointLight*> m_PointLightVec;		//point lights
	std::vector<SpotLight*> m_spotLightVec;		//spot lights
	DirectionalLight* m_pDirectionalLight;		//directional light
	/* Mesh objects */
	Mesh* m_pMesh1;
	Mesh* m_pMesh2;
	Mesh* m_pMesh3;
	Mesh* m_pMesh4;
	Mesh* m_pMesh5;
	Mesh* m_pMesh6;
	Mesh* m_pTexMesh;
	Model* m_pModel1;
	Model* m_pModel2;
	/* Material */
	Material* m_pMaterial;
	/* Texture */
	Texture* m_pTex1;
	Texture* m_pTex2;
	Texture* m_pTex3;
	/* Camera */
	Camera* m_pCamera;
	/* Frame timing */
	double m_timeDelta;
	/* Render resolution */
	unsigned int m_width;
	unsigned int m_height;
	float m_PixRatio;
};