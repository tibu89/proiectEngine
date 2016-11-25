#pragma once

#include <glfw3.h>
#include <vector>

class InputManager
{
private:
	InputManager(){}
	static InputManager* singleton;

	std::vector<int> keyStatus;
	std::vector<int> mouseStatus;
	float mouseDeltaX;
	float mouseDeltaY;
	float mouseScroll;
	
	GLFWwindow *window;

	//bellow is assumed singleton initialised (to avoid the if every callback)
	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		singleton->keyStatus.at(key) = action;
	}

	static void MouseCallback(GLFWwindow* window, int button, int action, int mods)
	{
		singleton->mouseStatus.at(button) = action;
	}

	static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
	{
		singleton->mouseScroll = (float)yoffset;
	}

public:
	void Init(GLFWwindow *_window);
	static InputManager* Get()
	{
		if(singleton == NULL)
		{
			singleton = new InputManager();
		}

		return singleton;
	}
	
	int GetKeyStatus(int key){return keyStatus.at(key);}
	int GetMouseStatus(int button){return mouseStatus.at(button);}

	float GetDeltaX(){return mouseDeltaX;}
	float GetDeltaY(){return mouseDeltaY;}
	float GetScroll(){return mouseScroll;}

	void UpdateMouseDeltas();
};