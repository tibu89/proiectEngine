#pragma once

#define GLM_FORCE_SWIZZLE 
#include <glm\glm.hpp>
#include <vector>
#include "CollisionShapes.h"
#include "RigidBody.h"
#include "Graphics.h"

struct Contact
{
	int m_iCollidableIndex1, m_iCollidableIndex2;

	glm::vec3 m_ContactPoint;
	glm::vec3 m_ContactNormal;
	float m_Penetration;

	void CalculateInternals(float _fDt);
};
/*
struct CollisionData
{
	int m_iCollidableIndex1, m_iCollidableIndex2;
	int m_iNumContacts;
	glm::vec3 m_ContactNormal;

	Contact contacts[4];
};*/

class ContactGenerator
{
private:
	std::vector<Contact> m_Contacts;
	unsigned int m_NumContacts;
	unsigned int m_MaxContacts;

	std::vector<RigidBody> *m_RigidBodies;
	std::vector<Collidable> *m_Collidables;

	static glm::vec3 cubeVertices[];
	static glm::vec3 cubeFaceNormals[];
	static unsigned char cubeFaceVertexIndices[];
public:
	ContactGenerator(std::vector<RigidBody> *_rigidBodies, std::vector<Collidable> *_collidables, unsigned int _maxContacts = 256 * 256);
	void ClearContacts(){m_NumContacts = 0;}

	void CheckAndAddContact(CollidableID _collidable1, CollidableID _collidable2);
    void CheckAndAddContactCubePlane(CollidableID _collidableId1, CollidableID _collidableId2);

	void DebugContacts( Graphics* graphics );

private:
	int GetBestCubeFaceToDirection(const glm::quat &orientation, const glm::vec3 &dir);
};