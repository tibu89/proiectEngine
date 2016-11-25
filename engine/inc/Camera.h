#pragma once

#define GLM_FORCE_SWIZZLE

#include <glm\glm.hpp>
#include <glm\gtc\constants.hpp>

class Camera
{
protected:
	float pitch, yaw;
	float zoom;

	glm::mat4 viewMatrix;

public:
	Camera(float _pitch, float _yaw) {pitch = _pitch; yaw = _yaw;}
	Camera() {pitch = 0.f; yaw = 0.f;}

	glm::mat4 GetViewMatrix(){ return viewMatrix; }
	virtual void UpdateViewMatrix() = 0;

	virtual void Translate(glm::vec3 offset) = 0;
	virtual void Zoom(float amount) = 0;

	virtual void RotatePitch(float rads)
	{
		pitch += rads;
		if(pitch >= glm::half_pi<float>())
		{
			pitch = glm::half_pi<float>();
		}
		else if(pitch <= -glm::half_pi<float>())
		{
			pitch = -glm::half_pi<float>();
		}

		UpdateViewMatrix();
	}
	virtual void RotateYaw(float rads)
	{
		yaw += rads;

		if(yaw >= 2 * glm::pi<float>())
		{
			yaw -= 2 * glm::pi<float>();
		}
		else if(yaw < 0)
		{
			yaw += 2 * glm::pi<float>();
		}
		
		UpdateViewMatrix();
	}
};