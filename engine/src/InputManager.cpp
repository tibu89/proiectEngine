#include "InputManager.h"

InputManager* InputManager::singleton = NULL;

void InputManager::Init(GLFWwindow *_window)
{
	window = _window; //check is valid?
	keyStatus.resize(GLFW_KEY_LAST, 0);
	mouseStatus.resize(GLFW_MOUSE_BUTTON_LAST, 0);

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	glfwSetCursorPos(window, width / 2, height / 2);

	glfwSetScrollCallback(window, this->ScrollCallback);
	glfwSetKeyCallback(window, this->KeyCallback);
	glfwSetMouseButtonCallback(window, this->MouseCallback);
}

void InputManager::UpdateMouseDeltas()
{
	int halfWidth, halfHeight;
    glfwGetFramebufferSize(window, &halfWidth, &halfHeight);
	halfWidth /= 2;
	halfHeight /= 2;

	double xpos = 0, ypos = 0;

	glfwGetCursorPos(window, &xpos, &ypos);
	glfwSetCursorPos(window, halfWidth, halfHeight);

	mouseDeltaX = halfWidth  - (float)xpos;
	mouseDeltaY = halfHeight - (float)ypos;
}