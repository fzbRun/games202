#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include "../../OpenGL-Mathematics-master//glm/glm.hpp"
#include "../../OpenGL-Mathematics-master/glm/gtc/matrix_transform.hpp"

#include <vector>

enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

class MyCamera {
public:
	glm::vec3 Position = glm::vec3(0.0f,0.0f,0.0f);
	glm::vec3 Front = glm::vec3(0.0f,0.0f,-1.0f);
	glm::vec3 Up = glm::vec3(0.0f,1.0f,0.0f);
	glm::vec3 Right;
	glm::vec3 WorldUp;

	float Yaw = -90.0f;
	float Pitch = 0.0f;
	float MoveSpeed = 2.5f;
	float MouseSensitivity = 0.1f;
	float Fov = 45.0f;

	MyCamera();
	MyCamera(glm::vec3 position, glm::vec3 wroldup, float pitch, float yaw, float fov);
	glm::mat4 getView();
	void cameraKeyMove(Camera_Movement direction,float deltaTime);
	void cameraMouseMove(float xoffset,float yoffset);
	void cameraScroll(float yoffset);
private:
	void updateCamera();
};

#endif
