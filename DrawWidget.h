#pragma once

#include <QGLWidget>

//forward declaration
class QWidget;
class Camera;

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
private:
	Camera* m_pCamera;
	GLuint m_Sh1Vao;
	GLuint m_Sh1VtxOff;
	GLuint m_Sh1IndxOff;
	GLuint m_Sh2Vao;
	GLuint m_Sh2VtxOff;
	GLuint m_Sh2IndxOff;
	GLuint m_Sh3Vao;
	GLuint m_Sh3VtxOff;
	GLuint m_Sh3IndxOff;
	GLuint m_VIbo;
	GLuint m_Program;
	GLint m_u_MVPMtxLoc;
	GLint m_u_ambientLightLoc;
	GLint m_u_lightPosLoc;
	GLint m_u_MWMtxLoc;
	std::string m_VertexShaderSrc;
	std::string m_FragmentShaderSrc;
	uint m_NumTri;
	uint m_Sh1NumIndcs;
	uint m_Sh2NumIndcs;
	uint m_Sh3NumIndcs;
};