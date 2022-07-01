#pragma once

#include <QOpenGLWidget>

//forward declaration
class QWidget;
class Camera;
class Renderer;
struct lightData;

class DrawWidget : public QOpenGLWidget
{
	Q_OBJECT

public:
	DrawWidget(QWidget* parent = nullptr);
	~DrawWidget();
	void setLDataPtr(lightData* vlightData);
protected:
	void initializeGL();
	void paintGL();
	//void paintEvent(QPaintEvent*);
	void mouseMoveEvent(QMouseEvent*);
	void mousePressEvent(QMouseEvent*);
	void mouseReleaseEvent(QMouseEvent*);
	void keyPressEvent(QKeyEvent*);
	void resizeEvent(QResizeEvent*);
	void timerEvent(QTimerEvent*);
private:
	void initOpenGLData();
	void initVertexArrays();
	void installShader();
	void drawIncTriangle();
	void pollKeyState();
signals:
	void sigClose();
	void sigToggleFullScreen();
private:
	Camera* m_pCamera;				//view camera
	Renderer* m_pRenderer;			//renderer object
	int m_Rtimer;					//render timer
	float m_prevX;					//prev mouse x
	float m_prevY;					//prev mouse y
	bool m_leftBtnPressed;
	bool m_middleBtnPressed;
	bool m_rightBtnPressed;
	/* Frame timing */
	double m_timeDelta;
	//LARGE_INTEGER m_curTime;
	LARGE_INTEGER m_freq;
	LARGE_INTEGER m_prevTime;
};