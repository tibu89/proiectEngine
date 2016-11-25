#include "ContactGenerator.h"
#include <glm\gtc\matrix_access.hpp>
#include <glm\gtx\quaternion.hpp>
#include <iostream>

float ProjectToAxis(const glm::mat4& _trMatrix, const glm::vec3& _halfSize, const glm::vec3& _axis)
{
	return 
        abs(glm::dot(glm::vec3(glm::column(_trMatrix, 0)), _axis)) * _halfSize.x +
        abs(glm::dot(glm::vec3(glm::column(_trMatrix, 1)), _axis)) * _halfSize.y +
        abs(glm::dot(glm::vec3(glm::column(_trMatrix, 2)), _axis)) * _halfSize.z;
}

glm::vec3 ContactGenerator::cubeVertices[] = {
	glm::vec3(-0.5f, -0.5f, -0.5f),
	glm::vec3(-0.5f, -0.5f,  0.5f),
	glm::vec3(-0.5f,  0.5f, -0.5f),
	glm::vec3(-0.5f,  0.5f,  0.5f),
	glm::vec3( 0.5f, -0.5f, -0.5f),
	glm::vec3( 0.5f, -0.5f,  0.5f),
	glm::vec3( 0.5f,  0.5f, -0.5f),
	glm::vec3( 0.5f,  0.5f,  0.5f)
};

glm::vec3 ContactGenerator::cubeFaceNormals[] = {
	glm::vec3( 1.f, 0.f, 0.f),
	glm::vec3(-1.f, 0.f, 0.f),
	glm::vec3( 0.f, 1.f, 0.f),
	glm::vec3( 0.f,-1.f, 0.f),
	glm::vec3( 0.f, 0.f, 1.f),
	glm::vec3( 0.f, 0.f,-1.f),
};

unsigned char ContactGenerator::cubeFaceVertexIndices[] = {
	4, 6, 7, 5,
	0, 1, 3, 2,
	2, 3, 7, 6,
	0, 4, 5, 1,
	1, 5, 7, 3,
	0, 2, 6, 4,
};

ContactGenerator::ContactGenerator(std::vector<RigidBody> *_rigidBodies, std::vector<Collidable> *_collidables, unsigned int _maxContacts)
{
	m_RigidBodies = _rigidBodies;
	m_Collidables = _collidables;
	m_MaxContacts = _maxContacts;
	m_Contacts.resize(m_MaxContacts);
	m_NumContacts = 0;
}

int ContactGenerator::GetBestCubeFaceToDirection(const glm::quat &orientation, const glm::vec3 &dir)
{
	int bestFaceIndex = -1;
	glm::mat3 rotationMatrix = glm::toMat3(orientation);

	float minF = FLT_MAX;

	//looking for face most facing direction, so for minimum dot product
	//!!! assuming normalized vectors !!!
	for(unsigned short i = 0; i < 6; i++)
	{
		float f = glm::dot(rotationMatrix * cubeFaceNormals[i], dir);

		if( f < minF )
		{
			minF = f;
			bestFaceIndex = i;
		}
	}

	assert(bestFaceIndex != -1);
	return bestFaceIndex;
}

void ContactGenerator::CheckAndAddContact(CollidableID _collidableId1, CollidableID _collidableId2)
{
	Collidable &collidable1 = m_Collidables->at(_collidableId1);
	Collidable &collidable2 = m_Collidables->at(_collidableId2);
	
    if(collidable1.m_iShapeType == ShapeType::PLANE && collidable2.m_iShapeType == ShapeType::CUBE)
    {
        CheckAndAddContactCubePlane(_collidableId2, _collidableId1); return;
    }
    
    if(collidable2.m_iShapeType == ShapeType::PLANE && collidable1.m_iShapeType == ShapeType::CUBE)
    {
        CheckAndAddContactCubePlane(_collidableId1, _collidableId2); return;
    }
}

void ContactGenerator::CheckAndAddContactCubePlane(CollidableID _collidableId1, CollidableID _collidableId2)
{
	if(m_NumContacts >= m_MaxContacts)
	{
		std::cerr<<"Max contacts reached!!!\n";
		return;
	}

	Collidable &collidable1 = m_Collidables->at(_collidableId1);
	Collidable &collidable2 = m_Collidables->at(_collidableId2);

	glm::vec3 planeNormal = collidable2.m_vData.xyz;
	float planeDist = collidable2.m_vData.w;
	RigidBody &body = m_RigidBodies->at(collidable1.m_iRigidBodyIndex);
	glm::mat4 cubeTrMatrix = body.GetTransformMatrix();
	glm::vec3 cubeHalfSize = collidable1.m_vData.xyz;

	float projectedRadius = ProjectToAxis(cubeTrMatrix, cubeHalfSize, planeNormal);

	if(glm::dot(planeNormal, body.GetPosition()) - projectedRadius > planeDist)
	{
		return; //cube too far to potentially contact
	}

	int incidentFaceIndex = GetBestCubeFaceToDirection(m_RigidBodies->at(collidable1.m_iRigidBodyIndex).GetOrientation(), planeNormal);
	glm::vec4 currentVertex;

	for(unsigned short faceVertIndex = incidentFaceIndex * 4; faceVertIndex < incidentFaceIndex * 4 + 4; faceVertIndex++)
	{
		currentVertex = glm::vec4(cubeVertices[cubeFaceVertexIndices[faceVertIndex]], 1.f);
		
		currentVertex = cubeTrMatrix * currentVertex;
		float distance = glm::dot(planeNormal, glm::vec3(currentVertex.xyz));

		if(distance < planeDist)
		{
			Contact &newContact = m_Contacts[m_NumContacts++];

			newContact.m_iCollidableIndex1 = _collidableId1;
			newContact.m_iCollidableIndex2 = -1;
			newContact.m_ContactNormal = planeNormal;
			newContact.m_ContactPoint = currentVertex.xyz;
			newContact.m_Penetration = planeDist - distance;
		}
	}
}

void ContactGenerator::DebugContacts( Graphics *graphics )
{
	for(unsigned int i = 0; i < m_NumContacts; i++)
	{
		Contact &contact = m_Contacts[i];
		//graphics->DrawDebugPoint(contact.m_ContactPoint);

		//naivest resolve ever
		m_RigidBodies->at(m_Collidables->at(contact.m_iCollidableIndex1).m_iRigidBodyIndex).AddForceAtPoint(contact.m_ContactNormal * contact.m_Penetration * 0.4f, contact.m_ContactPoint);
	}
}

void Contact::CalculateInternals(float _fDt)
{

}