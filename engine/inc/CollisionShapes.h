#pragma once

#include "RigidBody.h"

typedef int CollidableID;

enum ShapeType
{
	CUBE = 0,
	PLANE
};

struct Collidable
{
	int m_iRigidBodyIndex;
	ShapeType m_iShapeType;

	glm::vec4 m_vData; //half size for CUBE, normal + dist for PLANE

	Collidable(int _index, const glm::vec3 &_halfSize)
	{
		m_iRigidBodyIndex = _index;
		m_iShapeType = CUBE;

		m_vData = glm::vec4(_halfSize, 0.f);
	}

	Collidable(const glm::vec3 &_normal, float _dist)
	{
		m_iRigidBodyIndex = -1;
		m_iShapeType = PLANE;

		m_vData = glm::vec4(_normal, _dist);
	}
};