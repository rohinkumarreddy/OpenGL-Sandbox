#include "Camera.h"
#include "glm/gtx/transform.hpp"

const float Camera::MOVEMENT_SPEED = 7.0f;//0.1f;
const float Camera::MOUSE_TURN_SPEED = 0.5f;
const float Camera::MAX_MOUSE_DELTA = 5.0f;
const float Camera::MIN_MOUSE_DELTA = -5.0f;
const float Camera::MAX_PITCH = 89.0f;
const float Camera::MIN_PITCH = -89.0f;

void vec2Quaternion(const glm::vec3 front, float& yaw, float& pitch)
{
	yaw = glm::degrees(atan(front.z / front.x));
	pitch = glm::degrees(atan(front.y * cos(glm::radians(yaw)) / front.x));
}

void quaternion2Vec(glm::vec3& front, const float yaw, const float pitch)
{
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
}

Camera::Camera()
//Camera is always facing '-Z' axis.
//use unit-vector as view direction.
: m_viewDirection(0.02f, -0.39f, -0.92f),
//Up vector is along Y-axis.
m_worldUp(0.0f, 1.0f, 0.0f),
//Initialize camera position at world-origin.
m_position(0.201957405f, 3.63646984f, 4.53021145f),
//Initialize strafe direction as X-axis.
m_strafeDirection(1.0f, 0.0f, 0.0f),
m_oldMousePos(0.0f, 0.0f),
m_timeDelta(0),
m_yaw(-90.0f),
m_pitch(0.0f)
{
	m_front = m_viewDirection;
	m_right = m_strafeDirection;
	m_up = m_worldUp;
	m_moveSpeed = MOVEMENT_SPEED;
	m_turnSpeed = MOUSE_TURN_SPEED;

	vec2Quaternion(m_front, m_yaw, m_pitch);

	update();
}


Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch, float moveSpeed, float turnSpeed)
	:
	m_position(position),
	m_worldUp(up),
	m_up(up),
	m_yaw(yaw),
	m_pitch(pitch),
	m_front(glm::vec3(0.0f, 0.0f, -1.0f)),
	m_moveSpeed(moveSpeed),
	m_turnSpeed(turnSpeed),
	m_oldMousePos(0.0f, 0.0f),
	m_timeDelta(0),
	m_right(1.0f, 0.0f, 0.0f)
{
	m_viewDirection = m_front;
	m_strafeDirection = m_right;

	update();
}

Camera::~Camera()
{
}

glm::mat4 Camera::getWorldToViewMtx() const
{
	return glm::lookAt(m_position, m_position + m_viewDirection, m_up);
}

glm::vec3 Camera::getPosition() const
{
	return m_position;
}

void Camera::update()
{
	//Calculate vector from quaternion
	quaternion2Vec(m_front, m_yaw, m_pitch);
	m_front = glm::normalize(m_front);//direction
	//Update camera's right vector
	m_right = glm::normalize(glm::cross(m_front, m_worldUp));
	//Update camera's up vector
	m_up = glm::normalize(glm::cross(m_right, m_front));
	//Redundant vars
	m_viewDirection = m_front;
	m_strafeDirection = m_right;
}

void Camera::mouseUpdate(const glm::vec2& mouseDelta)
{
#if 1
	//Scale x & y changes by turnspeed
	float xChange = mouseDelta.x * m_turnSpeed;
	float yChange = mouseDelta.y * m_turnSpeed;

	//Clamp scaled x & y changes
	if (xChange > MAX_MOUSE_DELTA)
		xChange = MAX_MOUSE_DELTA;
	if (xChange < MIN_MOUSE_DELTA)
		xChange = MIN_MOUSE_DELTA;

	if (yChange > MAX_MOUSE_DELTA)
		yChange = MAX_MOUSE_DELTA;
	if (yChange < MIN_MOUSE_DELTA)
		yChange = MIN_MOUSE_DELTA;

	//Update yaw & pitch
	m_yaw += xChange;
	m_pitch -= yChange;

	//Clamp pitch value
	if (m_pitch > MAX_PITCH)
		m_pitch = MAX_PITCH;

	if (m_pitch < MIN_PITCH)
		m_pitch = MIN_PITCH;

	update();
#else
	glm::vec3 strafeDir = m_strafeDirection;

	//Rotate along the vector perpendicular to the plane
	//of view direction & Y-axis for vertical rotation.
	//if(m_viewDirection != m_up || m_viewDirection != -m_up)//sin(0)==sin(pi)==0
		m_strafeDirection = glm::cross(m_viewDirection, m_worldUp);

	//Rotate along Y-axis for horizontal rotation
	glm::mat4 rotator = glm::rotate(-mouseDelta.x * MOUSE_TURN_SPEED, m_up)*
						glm::rotate(-mouseDelta.y * MOUSE_TURN_SPEED, m_strafeDirection);
	//Apply rotation if valid
	if (glm::length(mouseDelta) < MAX_MOUSE_DELTA)
		m_viewDirection = glm::mat3(rotator) * m_viewDirection;
#endif
}

void Camera::moveForward()
{
	//Move forward along view direction.
	m_position += (MOVEMENT_SPEED * m_timeDelta) * m_viewDirection;
	update();
	//m_position += MOVEMENT_SPEED * m_viewDirection;
}

void Camera::moveBackward()
{
	//Move backward along view direction.
	m_position += -(MOVEMENT_SPEED * m_timeDelta) * m_viewDirection;
	update();
	//m_position += -MOVEMENT_SPEED * m_viewDirection;
}

void Camera::strafeLeft()
{
	//Vector perpendicular to the view-direction & Y-axis plane.
	m_position += -(MOVEMENT_SPEED * m_timeDelta) * m_strafeDirection;
	update();
	//m_position += -MOVEMENT_SPEED * m_strafeDirection;
}

void Camera::strafeRight()
{
	//Vector perpendicular to the view-direction & Y-axis plane.
	m_position += (MOVEMENT_SPEED * m_timeDelta) * m_strafeDirection;
	update();
	//m_position += MOVEMENT_SPEED * m_strafeDirection;
}

void Camera::moveUp()
{
	//Y-axis is upward
	m_position += (MOVEMENT_SPEED * m_timeDelta) * m_worldUp;
	update();
	//m_position += MOVEMENT_SPEED * m_up;
}

void Camera::moveDown()
{
	//Y-axis is upward
	m_position += -(MOVEMENT_SPEED * m_timeDelta) * m_worldUp;
	update();
	//m_position += -MOVEMENT_SPEED * m_up;
}
