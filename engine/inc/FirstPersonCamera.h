#pragma once

#include "Camera.h"

class FirstPersonCamera : public Camera
{
private:
	glm::vec3 position;
public:
	FirstPersonCamera(
		float _pitch,
		float _yaw, 
		glm::vec3 _position)
		: Camera(_pitch, _yaw){position = _position;}

	void UpdateViewMatrix();
	void Zoom(float amount);

	void Translate(glm::vec3 offset);
};