#pragma once

#include <vector>
#include <glm\glm.hpp>
#include "RigidBody.h"
#include "ContactGenerator.h"

typedef int RigidBodyID;

class PhysicsEngine
{
private:
	PhysicsEngine() {}
	static PhysicsEngine *singleton;

	std::vector<RigidBody> m_DynamicBodies;
    std::vector<Collidable> m_Collidables;

    ContactGenerator *m_ContactGenerator;

public:

	static PhysicsEngine* Get()
	{
		if(singleton == NULL)
		{
			singleton = new PhysicsEngine();
		}

		return singleton;
	}

    void Init()
    {
        m_ContactGenerator = new ContactGenerator(&m_DynamicBodies, &m_Collidables);
    }

	void Update(float _fDt);

	RigidBodyID AddCube(const glm::vec3 _pos = glm::vec3(0.f), const glm::quat _orientation = glm::quat(), const glm::vec3 _halfSize = glm::vec3(0.5f));
	void AddPlane(const glm::vec3 &_normal, float _dist);

	RigidBody& GetRigidBodyByID(RigidBodyID _id)
	{
		assert((unsigned int)_id < m_DynamicBodies.size());

		return m_DynamicBodies[_id];
	}

	std::vector<RigidBody>& GetRigidBodies(){return m_DynamicBodies;}

	void DebugContacts(Graphics *_graphics){m_ContactGenerator->DebugContacts(_graphics);}

private:
	void IntegrateDynamicBodies(float _fDt);
	void CheckContacts();
};