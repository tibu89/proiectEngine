#pragma once

#include <GL\glew.h>
#include <glfw3.h>
#include <glm\glm.hpp>
#include "Mesh.h"

#include <string>
#include <vector>
#include <iostream>

#define check_gl_error() _check_gl_error(__FILE__,__LINE__)

void _check_gl_error(const char *file, int line);

class Graphics
{
private:
	GLFWwindow* window;
	GLuint programID, debugProgramID;
	GLuint vertexArrayID;

	glm::mat4 perspectiveMatrix;
	glm::mat4 viewMatrix;

	GLuint projectionMatrixID;
	GLuint viewMatrixID;
	GLuint modelMatrixID;

	GLuint difuseColorID;

	GLuint vertexBuffer;
	GLuint normalBuffer;

	std::vector<float> vertexData;
	std::vector<float> normalData;

	std::vector<Mesh> meshes;

	void LoadShaders(std::string vsPath, std::string psPath, GLuint &_programID);

	Graphics(){}

	static Graphics* singleton;

public:
	static Graphics* Get()
	{
		if(singleton == NULL)
		{
			singleton = new Graphics();
		}

		return singleton;
	}

	void Init(int, int, std::string);
	void Shutdown();
	GLFWwindow* GetWindow(){return window;}

	void SetPerspective(glm::mat4 const &P)
	{
		perspectiveMatrix = P;
		glUniformMatrix4fv(projectionMatrixID, 1, GL_FALSE, &perspectiveMatrix[0][0]);
	}

	void SetView(glm::mat4 const &V)
	{
		viewMatrix = V;
		glUniformMatrix4fv(viewMatrixID, 1, GL_FALSE, &viewMatrix[0][0]);
	}

	int AddMesh(const float [], const float[], int);
	void UpdateVBOs();
	void DrawMesh(unsigned int, glm::mat4 const &, glm::vec3 const&);
};