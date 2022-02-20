#pragma once

#include <QGLWidget>

//forward declaration
class QWidget;
class Camera;
class Renderer;
struct lightData;

class DrawWidget : public QGLWidget
{
	Q_OBJECT

public:
	DrawWidget(QWidget* parent = nullptr);
	~DrawWidget();
	void setLDataPtr(lightData* vlightData);
protected:
	void initializeGL();
	void paintGL();
	void mouseMoveEvent(QMouseEvent*);
	void keyPressEvent(QKeyEvent*);
	void resizeEvent(QResizeEvent*);
	void timerEvent(QTimerEvent*);
private:
	void initOpenGLData();
	void initVertexArrays();
	void installShader();
	void drawIncTriangle();
private:
	Camera* m_pCamera;				//view camera
	Renderer* m_pRenderer;			//renderer object
	int m_Rtimer;					//render timer
};