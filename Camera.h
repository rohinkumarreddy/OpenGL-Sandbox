#pragma once

#include "glm/glm.hpp"

class Camera
{
public:
	Camera();
	~Camera();

	glm::mat4 getWorldToViewMtx() const;
	void mouseUpdate(const glm::vec2& newMousePos);
	void moveForward();
	void moveBackward();
	void strafeLeft();
	void strafeRight();
	void moveUp();
	void moveDown();

private:
	glm::vec3 m_Position;
	glm::vec3 m_ViewDirection;
	const glm::vec3 m_Up;
	glm::vec2 m_OldMousePos;
	static const float Camera::MOVEMENT_SPEED;
	glm::vec3 m_StrafeDirection;
};