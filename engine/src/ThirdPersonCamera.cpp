#include <glm\gtc\matrix_transform.hpp>
#include "ThirdPersonCamera.h"

void ThirdPersonCamera::UpdateViewMatrix()
{
	glm::vec3 direction = glm::vec3(cos(pitch) * sin(yaw), sin(pitch), cos(pitch) * cos(yaw));
	glm::vec3 right = glm::vec3(sin(yaw - glm::half_pi<float>()), 0.f, cos(yaw - glm::half_pi<float>()));
	glm::vec3 up = glm::cross(right, direction);
	viewMatrix = glm::lookAt(target - direction * zoom, target, up);
}

void ThirdPersonCamera::Translate(glm::vec3 offset)
{
	target += offset;

	UpdateViewMatrix();
}

void ThirdPersonCamera::Zoom(float amount)
{
	zoom += amount;
	if(zoom < 3.0f)
		zoom = 3.0f;

	UpdateViewMatrix();
}