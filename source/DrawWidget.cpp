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
  m_pRenderer(nullptr)
{
	std::cout << "QT OpenGL version " << this->format().majorVersion() << "." << this->format().minorVersion() << std::endl;
	m_pRenderer = new Renderer(width(), height(), devicePixelRatio());
	m_Rtimer = startTimer(0);
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
	m_pRenderer->draw();
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
		repaint();//draw imediate mode & animation
	}
}

void DrawWidget::mouseMoveEvent(QMouseEvent* e)
{
	m_pCamera->mouseUpdate(glm::vec2(e->x(), e->y()));
	//repaint();
}

void DrawWidget::keyPressEvent(QKeyEvent* e)
{
	switch(e->key())
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
	}
	//repaint();
}