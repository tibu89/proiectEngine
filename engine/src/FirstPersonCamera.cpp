#pragma once

#include "FirstPersonCamera.h"
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\constants.hpp>
#include <glm\gtc\matrix_access.hpp>

#include <stdio.h>

void FirstPersonCamera::UpdateViewMatrix()
{
	glm::vec3 direction = glm::vec3(cos(pitch) * sin(yaw), sin(pitch), cos(pitch) * cos(yaw));
	glm::vec3 right = glm::vec3(sin(yaw - glm::half_pi<float>()), 0.f, cos(yaw - glm::half_pi<float>()));
	glm::vec3 up = glm::cross(right, direction);

	viewMatrix = glm::lookAt(position, position + direction, up);
}

void FirstPersonCamera::Translate(glm::vec3 offset)
{
	glm::vec4 offset4 = glm::vec4(offset, 0.f);
	position += glm::vec3
		(
			glm::dot( glm::column(viewMatrix, 0), offset4 ),
			glm::dot( glm::column(viewMatrix, 1), offset4 ),
			glm::dot( glm::column(viewMatrix, 2), offset4 )
		);

	UpdateViewMatrix();
}

void FirstPersonCamera::Zoom(float amount)
{
}