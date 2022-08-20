#include "MyCamera.h"

MyCamera::MyCamera() {
	updateCamera();
}

MyCamera::MyCamera(glm::vec3 position, glm::vec3 worldup, float pitch, float yaw, float fov) {
	Position = position;
	WorldUp = worldup;
	Pitch = pitch;
	Yaw = yaw;
	Fov = fov;
}

void MyCamera::cameraKeyMove(Camera_Movement direction,float deltaTime){
	float speed = MoveSpeed * deltaTime;
	if (direction == FORWARD) {
		Position += Front * speed;
	}
	if (direction == BACKWARD) {
		Position -= Front * speed;
	}
	if (direction == LEFT) {
		Position -= Right * speed;
	}
	if (direction == RIGHT) {
		Position += Right * speed;
	}
}

void MyCamera::cameraMouseMove(float xoffset,float yoffset) {
	xoffset *= MouseSensitivity;
	yoffset *= MouseSensitivity;

	Yaw += xoffset;
	Pitch += yoffset;

	if (Pitch < -89.0f) {
		Pitch = -89.0f;
	}
	else if (Pitch > 89.0f) {
		Pitch = 89.0f;
	}
	updateCamera();
}

void MyCamera::cameraScroll(float yoffset) {
	if (Fov >= 1.0f && Fov <= 45.0f) {
		Fov -= yoffset;
	}
	if (Fov < 1.0f) {
		Fov = 1.0f;
	}
	else if (Fov > 45.0f) {
		Fov = 45.0f;
	}
}

glm::mat4 MyCamera::getView() {
	return glm::lookAt(Position, Position + Front, Up);
}

void MyCamera::updateCamera() {
	glm::vec3 front;
	front.x = cos(glm::radians(Pitch)) * cos(glm::radians(Yaw));
	front.y = sin(glm::radians(Pitch));
	front.z = cos(glm::radians(Pitch)) * sin(glm::radians(Yaw));
	Front = glm::normalize(front);
	Right = glm::normalize(glm::cross(Front,WorldUp));
	Up = glm::normalize(glm::cross(Right,Front));
}