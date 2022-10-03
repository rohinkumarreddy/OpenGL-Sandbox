#pragma once

#include "glm/glm.hpp"
#include <vector>

class Camera
{
public:
	Camera();
	Camera( glm::vec3 position,
			glm::vec3 up,
			float yaw,
			float pitch,
			float moveSpeed,
			float turnSpeed,
			float panSpeed);
	~Camera();

	enum class mouseKeyType{LEFT_BTN=0, MIDDLE_BTN, RIGHT_BTN};
	enum class moveType{MV_FORWARD=0, MV_BACKWARD, MV_RIGHT, MV_LEFT, MV_UP, MV_DOWN};

	glm::mat4 getWorldToViewMtx() const;
	glm::vec3 getPosition() const;
	glm::vec3 getViewDirection() const;
	glm::vec3 getUpDirection() const;
	void mouseUpdate(const glm::vec2& mouseDelta, mouseKeyType type);
	void moveForward();
	void moveBackward();
	void strafeLeft();
	void strafeRight();
	void moveUp();
	void moveDown();
	inline void setTimeDelta(float deltaT) { m_timeDelta = deltaT; }
	void processEvents();
	void update();

private:
	void move();

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
	float m_panSpeed;

	glm::vec3 m_strafeDirection;//right
	glm::vec3 m_viewDirection;//front

	glm::vec2 m_oldMousePos;
	float m_timeDelta;

	std::vector<moveType> m_moveQueue;
	std::vector<glm::vec2> m_mouseDeltaAvg;//calculate average of recent 5 delta
	int m_mouseDeltaAvgCnt;//count number of saved delta's
	std::vector<glm::vec3> m_camMoveDeltaAvg;//calculate average of recent 5 delta
	int m_camMoveDeltaAvgCnt;//count number of saved delta's

	static const float MOUSE_PAN_SPEED;
	static const float MOVEMENT_SPEED;
	static const float MOUSE_TURN_SPEED;
	static const float MAX_MOUSE_DELTA;
	static const float MIN_MOUSE_DELTA;
	static const float MAX_PITCH;
	static const float MIN_PITCH;
	static const unsigned int MAX_SAMPLE_MOVE_AVG;
};