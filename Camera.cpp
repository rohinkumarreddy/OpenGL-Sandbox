#include "Camera.h"
#include "glm/gtx/transform.hpp"

const float Camera::MOVEMENT_SPEED = 0.1f;

Camera::Camera()
//Camera is always facing '-Z' axis.
//use unit-vector as view direction.
: m_ViewDirection(-0.0672515184f, -0.636077762f, -0.768687665f),
//Up vector is along Y-axis.
m_Up(0.0f, 1.0f, 0.0f),
//Initialize camera position at world-origin.
m_Position(0.201957405f, 3.63646984f, 4.53021145f),
//Initialize strafe direction as X-axis.
m_StrafeDirection(1.0f, 0.0f, 0.0f),
m_OldMousePos(0.0f, 0.0f)
{
}

Camera::~Camera()
{
}

glm::mat4 Camera::getWorldToViewMtx() const
{
	return glm::lookAt(m_Position, m_Position + m_ViewDirection, m_Up);
}

void Camera::mouseUpdate(const glm::vec2& newMousePos)
{
	const float MOUSE_MOUSE_SPEED = 0.005f;
	const float MAX_MOUSE_DELTA = 50.0f;

	//Calculate mouse delta
	glm::vec2 mouseDelta = newMousePos - m_OldMousePos;

	glm::vec3 strafeDir = m_StrafeDirection;

	//Rotate along the vector perpendicular to the plane
	//of view direction & Y-axis for vertical rotation.
	//if(m_ViewDirection != m_Up || m_ViewDirection != -m_Up)//sin(0)==sin(pi)==0
		m_StrafeDirection = glm::cross(m_ViewDirection, m_Up);

	//Rotate along Y-axis for horizontal rotation
	glm::mat4 rotator = glm::rotate(-mouseDelta.x * MOUSE_MOUSE_SPEED, m_Up)*
						glm::rotate(-mouseDelta.y * MOUSE_MOUSE_SPEED, m_StrafeDirection);
	//Apply rotation if valid
	if (glm::length(mouseDelta) < MAX_MOUSE_DELTA)
		m_ViewDirection = glm::mat3(rotator) * m_ViewDirection;
	
	//Save old position
	m_OldMousePos = newMousePos;
}

void Camera::moveForward()
{
	//Move forward along view direction.
	m_Position += MOVEMENT_SPEED * m_ViewDirection;
}

void Camera::moveBackward()
{
	//Move backward along view direction.
	m_Position += -MOVEMENT_SPEED * m_ViewDirection;
}

void Camera::strafeLeft()
{
	//Vector perpendicular to the view-direction & Y-axis plane.
	m_Position += -MOVEMENT_SPEED * m_StrafeDirection;
}

void Camera::strafeRight()
{
	//Vector perpendicular to the view-direction & Y-axis plane.
	m_Position += MOVEMENT_SPEED * m_StrafeDirection;
}

void Camera::moveUp()
{
	//Y-axis is upward
	m_Position += MOVEMENT_SPEED * m_Up;
}

void Camera::moveDown()
{
	//Y-axis is upward
	m_Position += -MOVEMENT_SPEED * m_Up;
}
