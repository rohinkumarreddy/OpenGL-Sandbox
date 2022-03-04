#include "common.h"

//#include "glm/glm.hpp"//included in ShapeGenerator
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"

#include "DrawWidget.h"
#include "Camera.h"
#include "Renderer.h"

#include <QWidget>
#include <QMouseEvent>
#include <QKeyEvent>

#include <iostream>
#include <fstream>

//const float X_DELTA = 0.1f;
//const uint NUM_VERTICES_PER_TRI = 3;
//const uint NUM_FLOATS_PER_VTX = 6;
//const uint TRIANGLE_BYTE_SIZE = NUM_VERTICES_PER_TRI * NUM_FLOATS_PER_VTX * sizeof(float);
//const uint MAX_TRIS = 20;

DrawWidget::DrawWidget(QWidget* parent)
	: QGLWidget(parent),
	m_pCamera(new Camera),
	m_pRenderer(nullptr),
	m_prevX(0),
	m_prevY(0),
	m_timeDelta(0),
	m_leftBtnPressed(false),
	m_rightBtnPressed(false),
	m_middleBtnPressed(false)
{
#ifndef NDEBUG
	std::cout << "QT OpenGL version " << this->format().majorVersion() << "." << this->format().minorVersion() << std::endl;
	std::cout << "swap interval " << format().swapInterval() << "\n";
#endif
	/*QGLFormat sf = this->format();
	sf.setSwapInterval(0);
	this->setFormat(sf);*/
	m_pRenderer = new Renderer(width(), height(), devicePixelRatio());
	m_Rtimer = startTimer(0);
	QueryPerformanceFrequency(&m_freq);
	QueryPerformanceCounter(&m_prevTime);
	//std::cout<<"double buffer "<<(doubleBuffer() ? "true\n" : "false\n");
}

DrawWidget::~DrawWidget()
{
	//stop existing timers
	killTimer(m_Rtimer);
	//Clean-up camera instance
	if (m_pCamera != nullptr)
		delete m_pCamera;
	m_pCamera = nullptr;
	//Clean-up renderer instance
	if (m_pRenderer != nullptr)
		delete m_pRenderer;
	m_pRenderer = nullptr;
}

void DrawWidget::setLDataPtr(lightData* vlightData)
{
	m_pRenderer->setLDataPtr(vlightData);
}

void DrawWidget::initializeGL()
{
	m_pRenderer->initialize();
	m_pRenderer->setCameraRef(m_pCamera);
}

void DrawWidget::paintGL()
{
	LARGE_INTEGER curTime;
	/* update timing */
	QueryPerformanceCounter(&curTime);
	//if (m_prevTime > 0)
	{
		double timeDelta = (double)(curTime.QuadPart - m_prevTime.QuadPart) / (double)m_freq.QuadPart;
		double maxTimeDelta = 1.0f / 60.0f;
		//std::cout << "maxTimeDelta " << maxTimeDelta << "\n";
		//std::cout << "timeDelta " << timeDelta * 1000 << "\n";
		if (timeDelta > maxTimeDelta)
			timeDelta = maxTimeDelta;
		m_timeDelta = timeDelta;
		if (m_pCamera)
		{
			m_pCamera->setTimeDelta(timeDelta);
			//m_pCamera->update();
		}
		//std::cout << "time delta " << timeDelta << std::endl;
	}
	QueryPerformanceCounter(&m_prevTime);//update previous time
	//std::cout << "chk2\n";
	m_pRenderer->draw();
	//std::cout << "chk5\n";
}

void DrawWidget::resizeEvent(QResizeEvent* e)
{
	m_pRenderer->setWidth(width());
	m_pRenderer->setHeight(height());
	//repaint();
	Q_UNUSED(e);
}

void DrawWidget::timerEvent(QTimerEvent* e)
{
	int tID = e->timerId();
	if (tID == m_Rtimer)
	{
		//query/poll for key press events
		pollKeyState();
		repaint();//update();//draw imediate mode & animation
	}
}

void DrawWidget::mouseMoveEvent(QMouseEvent* e)
{
	//current mouse position
	glm::vec2 prevMousePos(m_prevX, m_prevY);
	//current mouse position
	glm::vec2 curMousePos(e->x(), e->y());
	//Calculate mouse delta
	glm::vec2 mouseDelta = curMousePos - prevMousePos;
	//update mouse position
	m_prevX = e->x();
	m_prevY = e->y();
	if (m_leftBtnPressed)
	{
		//update camera
		m_pCamera->mouseUpdate(mouseDelta, Camera::mouseKeyType::LEFT_BTN);
	}
	else if (m_middleBtnPressed)
	{
		//update camera
		m_pCamera->mouseUpdate(mouseDelta, Camera::mouseKeyType::MIDDLE_BTN);
	}
	else if (m_rightBtnPressed)
	{
		//update camera
		m_pCamera->mouseUpdate(mouseDelta, Camera::mouseKeyType::RIGHT_BTN);
	}
	//repaint();
}

void DrawWidget::mousePressEvent(QMouseEvent* e)
{
	m_leftBtnPressed = false;
	m_rightBtnPressed = false;
	m_middleBtnPressed = false;
	if (e->button() == Qt::MouseButton::LeftButton)
	{
		m_leftBtnPressed = true;
	}
	else if (e->button() == Qt::MouseButton::RightButton)
	{
		m_rightBtnPressed = true;
	}
	else if (e->button() == Qt::MouseButton::MiddleButton)
	{
		m_middleBtnPressed = true;
	}
	//save first mouse position
	m_prevX = e->x();
	m_prevY = e->y();
	//repaint();
}

void DrawWidget::mouseReleaseEvent(QMouseEvent* e)
{
	m_leftBtnPressed = false;
	m_rightBtnPressed = false;
	m_middleBtnPressed = false;
	//m_pCamera->mouseUpdate(glm::vec2(e->x(), e->y()));
	//repaint();
	Q_UNUSED(e)
}

void DrawWidget::keyPressEvent(QKeyEvent* e)
{
	Q_UNUSED(e)
	//std::cout << "key pressed\n";
	//repaint();//get the latest delatime
	/*switch(e->key())
	{
	case Qt::Key::Key_W:
		m_pCamera->moveForward();
		break;
	case Qt::Key::Key_S:
		m_pCamera->moveBackward();
		break;
	case Qt::Key::Key_A:
		m_pCamera->strafeLeft();
		break;
	case Qt::Key::Key_D:
		m_pCamera->strafeRight();
		break;
	case Qt::Key::Key_R:
		m_pCamera->moveUp();
		break;
	case Qt::Key::Key_F:
		m_pCamera->moveDown();
		break;
	case Qt::Key::Key_Escape:
		emit sigClose();
		break;
	case Qt::Key::Key_Return:
	{
		emit sigToggleFullScreen();
		break;
	}
	}*/
	//repaint();// to trigger immediately
}

void DrawWidget::pollKeyState()
{
	const int KEY_W = 0x57;
	const int KEY_S = 0x53;
	const int KEY_A = 0x41;
	const int KEY_D = 0x44;
	const int KEY_R = 0x52;
	const int KEY_F = 0x46;
	if(GetAsyncKeyState(KEY_W) || GetAsyncKeyState(VK_UP))
		m_pCamera->moveForward();
	if(GetAsyncKeyState(KEY_S) || GetAsyncKeyState(VK_DOWN))
		m_pCamera->moveBackward();
	if(GetAsyncKeyState(KEY_A) || GetAsyncKeyState(VK_LEFT))
		m_pCamera->strafeLeft();
	if(GetAsyncKeyState(KEY_D) || GetAsyncKeyState(VK_RIGHT))
		m_pCamera->strafeRight();
	if(GetAsyncKeyState(KEY_R))
		m_pCamera->moveUp();
	if(GetAsyncKeyState(KEY_F))
		m_pCamera->moveDown();
	if(GetAsyncKeyState(VK_ESCAPE))
		emit sigClose();
	if(GetAsyncKeyState(VK_RETURN))
		emit sigToggleFullScreen();
}