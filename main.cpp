#include <iostream>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\gtc\random.hpp>

#include "Graphics.h"
#include "PhysicsEngine.h"
#include "InputManager.h"
#include "FirstPersonCamera.h"
#include "ThirdPersonCamera.h"

#include "tempModels.h"

#define MOUSE_SPEED (0.002f)
#define TRANSLATE_SPEED (0.02f)

PhysicsEngine* physics;

//ugly, should use event system
void CheckCameraInputs(Camera* camera)
{
	InputManager* inputManager = InputManager::Get();

	float mouseDeltaX = inputManager->GetDeltaX();
	float mouseDeltaY = inputManager->GetDeltaY();

	if( inputManager->GetMouseStatus(GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS )
	{
		camera->Translate( glm::vec3( 0.f, mouseDeltaY * TRANSLATE_SPEED, 0.f ) );
	}
	else
	{
		camera->RotateYaw  (mouseDeltaX * MOUSE_SPEED);
		camera->RotatePitch(mouseDeltaY * MOUSE_SPEED);
	}

	glm::vec3 cameraDirectionVector;

	cameraDirectionVector += glm::vec3( 0.f, 0.f, -1.f) * float(inputManager->GetKeyStatus(GLFW_KEY_W) != GLFW_RELEASE);
	cameraDirectionVector += glm::vec3(-1.f, 0.f,  0.f) * float(inputManager->GetKeyStatus(GLFW_KEY_A) != GLFW_RELEASE);
	cameraDirectionVector += glm::vec3( 0.f, 0.f,  1.f) * float(inputManager->GetKeyStatus(GLFW_KEY_S) != GLFW_RELEASE);
	cameraDirectionVector += glm::vec3( 1.f, 0.f,  0.f) * float(inputManager->GetKeyStatus(GLFW_KEY_D) != GLFW_RELEASE);

	camera->Translate( cameraDirectionVector * TRANSLATE_SPEED );
}

void InitSystems()
{
	Graphics::Get()->Init(640, 480, "Coliziunatorul3000");
	InputManager::Get()->Init(Graphics::Get()->GetWindow());
	PhysicsEngine::Get()->Init();
}

int main()
{
	InitSystems();
	check_gl_error();

	Graphics* graphics = Graphics::Get();
	check_gl_error();
	InputManager* input = InputManager::Get();
	PhysicsEngine* physics = PhysicsEngine::Get();

	int cubeMesh  = graphics->AddMesh(cubeVertexData,  cubeNormalData,  sizeof(cubeVertexData)  / sizeof(float) / 3);
	int planeMesh = graphics->AddMesh(planeVertexData, planeNormalData, sizeof(planeVertexData) / sizeof(float) / 3);

	graphics->UpdateVBOs();
	check_gl_error();

	glm::vec3 cubeColor(1.f, 0.f, 0.f);
	glm::vec3 planeColor(0.f, 1.f, 0.f);
	
	std::cout<<glGetString(GL_VERSION)<<std::endl;
	
    Camera *camera = new FirstPersonCamera(0.f, glm::pi<float>(), glm::vec3(0.f, 0.f,5.f));

	glm::mat4 perspectiveMatrix = glm::perspective(45.f, 640.f / 480.f, 1.0f, 200.0f);
	glm::mat4 viewMatrix;

	glm::mat4 planeModelMatrix = glm::translate(glm::vec3(0.0f, -10.0f, 0.0f)) * glm::scale(glm::vec3(100.0f, 100.0f, 100.0f));

	physics->AddPlane(glm::vec3(0.f, 1.f, 0.f), -10.f);
	
	graphics->SetPerspective(perspectiveMatrix);

	std::vector<RigidBodyID> cubes(20);
	for(std::vector<RigidBodyID>::iterator it = cubes.begin(); it != cubes.end(); it++)
	{
		*it = physics->AddCube(glm::sphericalRand(5.f));
		physics->GetRigidBodyByID(*it).AddForceAtBodyPoint(glm::sphericalRand(2.f), glm::sphericalRand(0.5f));
	}
	check_gl_error();

	do
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		input->UpdateMouseDeltas();
		
		CheckCameraInputs(camera);

		graphics->SetView(camera->GetViewMatrix());

		for(std::vector<RigidBodyID>::iterator it = cubes.begin(); it != cubes.end(); it++)
		{
			graphics->DrawMesh(cubeMesh, physics->GetRigidBodyByID(*it).GetTransformMatrix(), cubeColor);
		}

		graphics->DrawMesh(planeMesh, planeModelMatrix, planeColor);

		physics->Update(1 / 10.f);
		physics->DebugContacts(graphics);

		glfwSwapBuffers(graphics->GetWindow());	
        glfwPollEvents();
	}
	while( !glfwGetKey(graphics->GetWindow(), GLFW_KEY_ESCAPE ));

	graphics->Shutdown();
}