#include "Camera.h"
#include "glm/gtx/transform.hpp"

//#define USE_QUEUED_MOVEMENT//save camera movement and perform it in draw call
//#define USE_AVG_MOUSE_MOVE
//#define USE_AVG_KEY_MOVE

const float Camera::MOUSE_PAN_SPEED = 0.05f;
const float Camera::MOVEMENT_SPEED = 1.5f;//0.1f;
const float Camera::MOUSE_TURN_SPEED = 0.05f;
const float Camera::MAX_MOUSE_DELTA = 5.0f;
const float Camera::MIN_MOUSE_DELTA = -5.0f;
const float Camera::MAX_PITCH = 89.0f;
const float Camera::MIN_PITCH = -89.0f;
const unsigned int Camera::MAX_SAMPLE_MOVE_AVG = 5;

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

void smooth_mouse(float time_d, float& dx, float& dy)
{
	const float springiness = 75; // tweak to taste.
	double d = 1 - exp(log(0.5) * springiness * time_d);

	dx += dx * d;
	dy += dy * d;
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
m_pitch(0.0f),
m_mouseDeltaAvgCnt(0),
m_camMoveDeltaAvgCnt(0)
{
	m_front = m_viewDirection;
	m_right = m_strafeDirection;
	m_up = m_worldUp;
	m_moveSpeed = MOVEMENT_SPEED;
	m_turnSpeed = MOUSE_TURN_SPEED;
	m_panSpeed = MOUSE_PAN_SPEED;

	vec2Quaternion(m_front, m_yaw, m_pitch);

	update();
}


Camera::Camera( glm::vec3 position,
				glm::vec3 up,
				float yaw,
				float pitch,
				float moveSpeed,
				float turnSpeed,
				float panSpeed)
	:
	m_position(position),
	m_worldUp(up),
	m_up(up),
	m_yaw(yaw),
	m_pitch(pitch),
	m_front(glm::vec3(0.0f, 0.0f, -1.0f)),
	m_moveSpeed(moveSpeed),
	m_turnSpeed(turnSpeed),
	m_panSpeed(panSpeed),
	m_oldMousePos(0.0f, 0.0f),
	m_timeDelta(0),
	m_right(1.0f, 0.0f, 0.0f),
	m_mouseDeltaAvgCnt(0),
	m_camMoveDeltaAvgCnt(0)
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

glm::vec3 Camera::getViewDirection() const
{
	return m_viewDirection;
}

glm::vec3 Camera::getUpDirection() const
{
	return m_up;
}

void Camera::update()
{
#ifdef USE_QUEUED_MOVEMENT
	//perform queued moves
	move();
#endif
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

void Camera::move()
{
	if (!m_moveQueue.empty())
	{
		glm::vec3 oldPos = m_position, newPos = m_position;
		switch (m_moveQueue[0])
		{
		case(moveType::MV_FORWARD):
		{
			//Move forward along view direction.
			newPos += (m_moveSpeed * m_timeDelta) * m_viewDirection;
			break;
		}
		case(moveType::MV_BACKWARD):
		{
			//Move backward along view direction.
			newPos += -(m_moveSpeed * m_timeDelta) * m_viewDirection;
			break;
		}
		case(moveType::MV_RIGHT):
		{
			//Vector perpendicular to the view-direction & Y-axis plane.
			newPos += (m_moveSpeed * m_timeDelta) * m_strafeDirection;
			break;
		}
		case(moveType::MV_LEFT):
		{
			//Vector perpendicular to the view-direction & Y-axis plane.
			newPos += -(m_moveSpeed * m_timeDelta) * m_strafeDirection;
			break;
		}
		case(moveType::MV_UP):
		{
			//Y-axis is upward
			newPos += (m_moveSpeed * m_timeDelta) * m_worldUp;
			break;
		}
		case(moveType::MV_DOWN):
		{
			//Y-axis is upward
			newPos += -(m_moveSpeed * m_timeDelta) * m_worldUp;
			break;
		}
		default:
			break;
		}
		if (newPos != oldPos)
		{
#ifdef USE_AVG_KEY_MOVE
			glm::vec3 mDelta = newPos-oldPos;
			//smoothen the mouse delta using average
			{
				//save atleast MAX_SAMPLE_MOVE_AVG
				if (m_camMoveDeltaAvg.size() < MAX_SAMPLE_MOVE_AVG)
					m_camMoveDeltaAvg.push_back(mDelta);
				else
					m_camMoveDeltaAvg[m_camMoveDeltaAvgCnt++] = mDelta;
				//reset index
				if (m_camMoveDeltaAvgCnt == MAX_SAMPLE_MOVE_AVG)
					m_camMoveDeltaAvgCnt = 0;
				//calculate average
				if (m_camMoveDeltaAvg.size() == MAX_SAMPLE_MOVE_AVG)
				{
					float smoothX = 0, smoothY = 0, smoothZ = 0;
					for each (auto var in m_camMoveDeltaAvg)
					{
						smoothX += var.x;
						smoothY += var.y;
						smoothZ += var.z;
					}
					mDelta.x = smoothX / MAX_SAMPLE_MOVE_AVG;
					mDelta.y = smoothY / MAX_SAMPLE_MOVE_AVG;
					mDelta.z = smoothZ / MAX_SAMPLE_MOVE_AVG;
				}
			}
			m_position = oldPos + mDelta;
#else
			m_position = newPos;
#endif
		}
		m_moveQueue.erase(m_moveQueue.begin());
	}
}

void Camera::mouseUpdate(const glm::vec2& mouseDelta, mouseKeyType type)
{
#if 1
	glm::vec2 mDelta = mouseDelta;
#ifdef USE_AVG_MOUSE_MOVE
	//smoothen the mouse delta using average
	{
		//save atleast MAX_SAMPLE_MOVE_AVG
		if (m_mouseDeltaAvg.size() < MAX_SAMPLE_MOVE_AVG)
			m_mouseDeltaAvg.push_back(mouseDelta);
		else
			m_mouseDeltaAvg[m_mouseDeltaAvgCnt++] = mouseDelta;
		//reset index
		if (m_mouseDeltaAvgCnt == MAX_SAMPLE_MOVE_AVG)
			m_mouseDeltaAvgCnt = 0;
		//calculate average
		if (m_mouseDeltaAvg.size() == MAX_SAMPLE_MOVE_AVG)
		{
			float smoothX = 0, smoothY = 0;
			for each (auto var in m_mouseDeltaAvg)
			{
				smoothX += var.x;
				smoothY += var.y;
			}
			mDelta.x = smoothX / MAX_SAMPLE_MOVE_AVG;
			mDelta.y = smoothY / MAX_SAMPLE_MOVE_AVG;
		}
	}
#endif
	if (type == mouseKeyType::LEFT_BTN)
	{
		//Scale x & y changes by turnspeed
		float xChange = mDelta.x * m_turnSpeed;
		float yChange = mDelta.y * m_turnSpeed;

		//Exponential decay & delta time based smoothing
		smooth_mouse(m_timeDelta, mDelta.x, mDelta.y);

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
	}
	else if (type == mouseKeyType::MIDDLE_BTN)
	{
		//Scale x & y changes by turnspeed
		float xChange = mDelta.x * m_panSpeed;
		float yChange = mDelta.y * m_panSpeed;

		//Exponential decay & delta time based smoothing
		smooth_mouse(m_timeDelta, mDelta.x, mDelta.y);

		//Clamp scaled x & y changes
		if (xChange > MAX_MOUSE_DELTA)
			xChange = MAX_MOUSE_DELTA;
		if (xChange < MIN_MOUSE_DELTA)
			xChange = MIN_MOUSE_DELTA;

		if (yChange > MAX_MOUSE_DELTA)
			yChange = MAX_MOUSE_DELTA;
		if (yChange < MIN_MOUSE_DELTA)
			yChange = MIN_MOUSE_DELTA;

		m_position += xChange * m_strafeDirection;
		m_position -= yChange * m_up;
	}
	else if (type == mouseKeyType::RIGHT_BTN)
	{
		//Scale x & y changes by turnspeed
		float xChange = mDelta.x * m_panSpeed;
		float yChange = mDelta.y * m_panSpeed;

		//Exponential decay & delta time based smoothing
		smooth_mouse(m_timeDelta, mDelta.x, mDelta.y);

		//Clamp scaled x & y changes
		if (xChange > MAX_MOUSE_DELTA)
			xChange = MAX_MOUSE_DELTA;
		if (xChange < MIN_MOUSE_DELTA)
			xChange = MIN_MOUSE_DELTA;

		if (yChange > MAX_MOUSE_DELTA)
			yChange = MAX_MOUSE_DELTA;
		if (yChange < MIN_MOUSE_DELTA)
			yChange = MIN_MOUSE_DELTA;

		//m_position.x += xChange * m_viewDirection.x;
		m_position -= yChange * m_viewDirection;
	}
	//m_moveQueue.clear();//ignore pending move events
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
#ifdef USE_QUEUED_MOVEMENT
	m_moveQueue.push_back(moveType::MV_FORWARD);
#else
	//Move forward along view direction.
	m_position += (m_moveSpeed * m_timeDelta) * m_viewDirection;
	update();
	//m_position += MOVEMENT_SPEED * m_viewDirection;
#endif
}

void Camera::moveBackward()
{
#ifdef USE_QUEUED_MOVEMENT
	m_moveQueue.push_back(moveType::MV_BACKWARD);
#else
	//Move backward along view direction.
	m_position += -(m_moveSpeed * m_timeDelta) * m_viewDirection;
	update();
	//m_position += -MOVEMENT_SPEED * m_viewDirection;
#endif
}

void Camera::strafeLeft()
{
#ifdef USE_QUEUED_MOVEMENT
	m_moveQueue.push_back(moveType::MV_LEFT);
#else
	//Vector perpendicular to the view-direction & Y-axis plane.
	m_position += -(m_moveSpeed * m_timeDelta) * m_strafeDirection;
	update();
	//m_position += -MOVEMENT_SPEED * m_strafeDirection;
#endif
}

void Camera::strafeRight()
{
#ifdef USE_QUEUED_MOVEMENT
	m_moveQueue.push_back(moveType::MV_RIGHT);
#else
	//Vector perpendicular to the view-direction & Y-axis plane.
	m_position += (m_moveSpeed * m_timeDelta) * m_strafeDirection;
	update();
	//m_position += MOVEMENT_SPEED * m_strafeDirection;
#endif
}

void Camera::moveUp()
{
#ifdef USE_QUEUED_MOVEMENT
	m_moveQueue.push_back(moveType::MV_UP);
#else
	//Y-axis is upward
	m_position += (m_moveSpeed * m_timeDelta) * m_worldUp;
	update();
	//m_position += MOVEMENT_SPEED * m_up;
#endif
}

void Camera::moveDown()
{
#ifdef USE_QUEUED_MOVEMENT
	m_moveQueue.push_back(moveType::MV_DOWN);
#else
	//Y-axis is upward
	m_position += -(m_moveSpeed * m_timeDelta) * m_worldUp;
	update();
	//m_position += -MOVEMENT_SPEED * m_up;
#endif
}
