#include "PhysicsEngine.h"

PhysicsEngine* PhysicsEngine::singleton = NULL;

void PhysicsEngine::IntegrateDynamicBodies(float _fDt)
{
	int i = 0;
	for(std::vector<RigidBody>::iterator it = m_DynamicBodies.begin(); it != m_DynamicBodies.end(); it++)
	{
		it->Integrate(_fDt);i++;
	}
}

void PhysicsEngine::CheckContacts()
{
	m_ContactGenerator->ClearContacts();

	for(unsigned int i = 0; i < m_Collidables.size(); i++)
	{
		for(unsigned int j = i + 1; j < m_Collidables.size(); j++)
		{
			m_ContactGenerator->CheckAndAddContact(i, j);
		}
	}
}

void PhysicsEngine::Update(float _fDt)
{
	for(std::vector<RigidBody>::iterator it = m_DynamicBodies.begin(); it != m_DynamicBodies.end(); it++)
	{
		it->AddForceAtBodyPoint(glm::vec3(0.f, -0.01f, 0.f), glm::vec3());
	}
	
	IntegrateDynamicBodies(_fDt);

	CheckContacts();
}

RigidBodyID PhysicsEngine::AddCube(const glm::vec3 _pos, const glm::quat _orientation, const glm::vec3 _halfSize)
{
	RigidBodyID cubeID = m_DynamicBodies.size();

	m_DynamicBodies.push_back(RigidBody(_pos, _orientation));
	m_Collidables.push_back(Collidable(cubeID, _halfSize));

	return cubeID;
}

void PhysicsEngine::AddPlane(const glm::vec3 &_normal, float _dist)
{
	m_Collidables.push_back(Collidable(_normal, _dist));
}