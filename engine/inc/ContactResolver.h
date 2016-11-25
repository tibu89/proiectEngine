#pragma once

#include "RigidBody.h"
#include "CollisionShapes.h"
#include "ContactGenerator.h"

class ContactResolver
{
private:
	unsigned int k_maxPositionIterations;
	unsigned int k_maxVelocityIterations;

	std::vector<RigidBody> *m_RigidBodies;
	std::vector<Collidable> *m_Collidables;

	void PrepareContacts(std::vector<Contact> &_contacts, int _numContacts, float _fDt);
};