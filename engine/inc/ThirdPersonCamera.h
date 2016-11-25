#pragma once

#include "Camera.h"

class ThirdPersonCamera : public Camera
{
private:
	glm::vec3 target;
public:
	ThirdPersonCamera(
		float _pitch,
		float _yaw, 
		glm::vec3 _target, 
		float _zoom = 5.0f)
		: Camera(_pitch, _yaw){target = _target;zoom = _zoom;}

	void UpdateViewMatrix();

	void Translate(glm::vec3 offset);
	void Zoom(float amount);
};