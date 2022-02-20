#pragma once

//forward declaration
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
	/* OpenGL buffer IDs */
	GLuint m_Sh1Vao;
	GLuint m_Sh2Vao;
	GLuint m_Sh3Vao;
	GLuint m_Sh4Vao;
	GLuint m_Sh5Vao;
	GLuint m_Sh6Vao;
	GLuint m_VIbo;
	/* OpenGL shader vars */
	unsigned int m_NumTri;
	GLuint m_Sh1VtxOff;
	GLuint m_Sh1IndxOff;
	GLuint m_Sh2VtxOff;
	GLuint m_Sh2IndxOff;
	GLuint m_Sh3VtxOff;
	GLuint m_Sh3IndxOff;
	GLuint m_Sh4VtxOff;
	GLuint m_Sh4IndxOff;
	GLuint m_Sh5VtxOff;
	GLuint m_Sh5IndxOff;
	GLuint m_Sh6VtxOff;
	GLuint m_Sh6IndxOff;
	unsigned int m_Sh1NumIndcs;
	unsigned int m_Sh2NumIndcs;
	unsigned int m_Sh3NumIndcs;
	unsigned int m_Sh4NumIndcs;
	unsigned int m_Sh5NumIndcs;
	unsigned int m_Sh6NumIndcs;
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