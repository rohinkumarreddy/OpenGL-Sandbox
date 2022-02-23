#pragma once

#include "glm/glm.hpp"

class Camera
{
public:
	Camera();
	Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch, float moveSpeed, float turnSpeed);
	~Camera();

	glm::mat4 getWorldToViewMtx() const;
	glm::vec3 getPosition() const;
	void mouseUpdate(const glm::vec2& mouseDelta);
	void moveForward();
	void moveBackward();
	void strafeLeft();
	void strafeRight();
	void moveUp();
	void moveDown();
	inline void setTimeDelta(float deltaT) { m_timeDelta = deltaT; }

private:
	glm::vec3 m_position;
	glm::vec3 m_front;
	glm::vec3 m_up;
	glm::vec3 m_right;
	glm::vec3 m_worldUp;

	float m_yaw;
	float m_pitch;

	float m_moveSpeed;
	float m_turnSpeed;

	glm::vec3 m_strafeDirection;//right
	glm::vec3 m_viewDirection;//front

	glm::vec2 m_oldMousePos;
	float m_timeDelta;

	void update();

	static const float Camera::MOVEMENT_SPEED;
	static const float Camera::MOUSE_TURN_SPEED;
	static const float Camera::MAX_MOUSE_DELTA;
	static const float Camera::MIN_MOUSE_DELTA;
	static const float Camera::MAX_PITCH;
	static const float Camera::MIN_PITCH;
};