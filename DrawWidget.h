#pragma once

#include <QGLWidget>

//forward declaration
class QWidget;
class Camera;
class lightData;

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCALL(x) GLClearError();\
        x;\
        ASSERT(GLLogCall(#x, __FILE__, __LINE__))

/* Clear all errors */
void GLClearError();

/* Log errors */
bool GLLogCall(const char* function, const char* file, int line);

class DrawWidget : public QGLWidget
{
	Q_OBJECT

public:
	DrawWidget(QWidget* parent = nullptr);
	~DrawWidget();
protected:
	void initializeGL();
	void paintGL();
	void mouseMoveEvent(QMouseEvent*);
	void keyPressEvent(QKeyEvent*);
private:
	void initOpenGLData();
	void initVertexArrays();
	void installShader();
	bool validateShader(GLuint);
	bool validateProgram(GLuint);
	std::string readShader(const char*);
	void drawIncTriangle();
public:
	void setLDataPtr(lightData* vlightData);
private:
	Camera* m_pCamera;				//camera object
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
	GLuint m_Program;
	GLuint m_ProgramPT;
	GLint m_u_MVPMtxLoc;
	GLint m_u_ambientLightLoc;
	GLint m_u_lightPosLoc;
	GLint m_u_eyePosLoc;
	GLint m_u_MWMtxLoc;
	GLint m_u_MVPMtxPTLoc;
	GLint m_u_lAttenuationFacLoc;
	std::string m_VertexShaderSrc;
	std::string m_FragmentShaderSrc;
	std::string m_VertexShaderPTSrc;
	std::string m_FragmentShaderPTSrc;
	uint m_NumTri;
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
	uint m_Sh1NumIndcs;
	uint m_Sh2NumIndcs;
	uint m_Sh3NumIndcs;
	uint m_Sh4NumIndcs;
	uint m_Sh5NumIndcs;
	uint m_Sh6NumIndcs;
};