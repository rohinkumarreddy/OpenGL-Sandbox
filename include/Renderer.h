#pragma once

//forward declaration
class Mesh;
class Shader;
class lightData;
class Camera;

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
	void initVertexArrays();
	void initOpenGLData();
	Shader* m_pShader;				//main shader
	Shader* m_pShaderPT;			//pass-through shader
	lightData* m_pLightData;		//light data ptr
	/* Mesh objects */
	Mesh* m_pMesh1;
	Mesh* m_pMesh2;
	Mesh* m_pMesh3;
	Mesh* m_pMesh4;
	Mesh* m_pMesh5;
	Mesh* m_pMesh6;
	/* Camera */
	Camera* m_pCamera;
	/* Frame timing */
	clock_t m_curTime;
	clock_t m_prevTime;
	float m_timeDelta;
	/* Render resolution */
	unsigned int m_width;
	unsigned int m_height;
	float m_PixRatio;
};