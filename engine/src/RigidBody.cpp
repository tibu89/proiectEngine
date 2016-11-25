#include "RigidBody.h"

void RigidBody::Integrate(float dt)
{
	lastFrameAcceleration = acceleration;
	lastFrameAcceleration += forceAccum * invMass;

	glm::vec3 angularAcceleration = inverseInertiaTensor * torqueAccum;
	velocity += lastFrameAcceleration * dt;

	angularVelocity += angularAcceleration * dt;

	velocity		*= powf(linearDamping, dt);
	angularVelocity *= powf(angularDamping, dt);

	position += velocity * dt;
	orientation = orientation + glm::quat(0, angularVelocity * dt) * orientation * 0.5f;

	CalculateDerivedData();
	ClearAccumulators();
}

void RigidBody::AddForce(const glm::vec3 &force)
{
	forceAccum += force;
}

void RigidBody::AddForceAtBodyPoint(const glm::vec3 &force, const glm::vec3 &point)
{
	glm::vec3 p = orientation * point + position;
	AddForceAtPoint(force, p);
}

void RigidBody::AddForceAtPoint(const glm::vec3 &force, const glm::vec3 &point)
{
	glm::vec3 p = point.xyz;
	p -= position;

	forceAccum += force;
	torqueAccum += glm::cross(p, force);
}

void RigidBody::ClearAccumulators()
{
	forceAccum = torqueAccum = glm::zero<glm::vec3>();
}

void RigidBody::CalculateDerivedData()
{
	orientation = glm::normalize(orientation);
	modelTransformMatrix = glm::translate(position) * glm::toMat4(orientation);
	glm::mat3 rotationMatrix = glm::toMat3(orientation);
	inverseInertiaTensorWorld = rotationMatrix * inverseInertiaTensor * glm::transpose(rotationMatrix);
}