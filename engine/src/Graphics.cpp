#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <glm\gtx\transform.hpp>
#include "Graphics.h"

Graphics* Graphics::singleton = NULL;

void Graphics::Init(int w, int h, std::string title)
{
	if(!glfwInit())
	{
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL 

	window = glfwCreateWindow(w, h, title.c_str(), NULL, NULL);
	if(!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	programID = 0;

	glfwMakeContextCurrent(window);
	check_gl_error();

	glewExperimental=true; // Needed in core profile 
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		exit(EXIT_FAILURE);
	}
	check_gl_error();
	
	// Dark blue background
	glClearColor(0.25f, 0.6f, 1.0f, 0.0f);
	check_gl_error();

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	check_gl_error();
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);
	check_gl_error();
	glEnable(GL_CULL_FACE);
	check_gl_error();
	
	glGenVertexArrays(1, &vertexArrayID);
	check_gl_error();
	glBindVertexArray(vertexArrayID);
	check_gl_error();

	//for debug point
	glEnable( GL_POINT_SMOOTH );
	check_gl_error();
    glEnable( GL_BLEND );
	check_gl_error();
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	check_gl_error();
	glPointSize(6.0f);
	check_gl_error();

	LoadShaders("basicVS.vertexshader", "debugPS.pixelshader", programID);
	check_gl_error();
	//LoadShaders("basicVS.vertexshader", "basicPS.pixelshader", programID);
}

void Graphics::Shutdown()
{
	glDeleteBuffers(1, &vertexBuffer);
	glDeleteBuffers(1, &normalBuffer);
	glDeleteProgram(programID);
	//glDeleteProgram(debugProgramID);

	glfwDestroyWindow(window);
	glfwTerminate();
}

void Graphics::LoadShaders(std::string vsPath, std::string psPath, GLuint &_programID)
{
 
    // Create the shaders
    GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
 
    // Read the Vertex Shader code from the file
    std::string VertexShaderCode;
    std::ifstream VertexShaderStream(vsPath, std::ios::in);
    if(VertexShaderStream.is_open())
    {
        std::string Line = "";
        while(getline(VertexShaderStream, Line))
            VertexShaderCode += "\n" + Line;
        VertexShaderStream.close();
    }
 
    // Read the Fragment Shader code from the file
    std::string FragmentShaderCode;
    std::ifstream FragmentShaderStream(psPath, std::ios::in);
    if(FragmentShaderStream.is_open()){
        std::string Line = "";
        while(getline(FragmentShaderStream, Line))
            FragmentShaderCode += "\n" + Line;
        FragmentShaderStream.close();
    }
 
    GLint Result = GL_FALSE;
    int InfoLogLength;
 
    // Compile Vertex Shader
    printf("Compiling shader : %s\n", vsPath.c_str());
    char const * VertexSourcePointer = VertexShaderCode.c_str();
    glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
    glCompileShader(VertexShaderID);
 
    // Check Vertex Shader
    glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	fprintf(stdout, "glGetShaderiv VertexShaderID GL_COMPILE_STATUS %d\n", Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0)
	{
		std::vector<char> VertexShaderErrorMessage(InfoLogLength);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		fprintf(stdout, "%s\n", &VertexShaderErrorMessage[0]);
	}
 
    // Compile Fragment Shader
    printf("Compiling shader : %s\n", psPath.c_str());
    char const * FragmentSourcePointer = FragmentShaderCode.c_str();
    glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
    glCompileShader(FragmentShaderID);
 
    // Check Fragment Shader
    glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	fprintf(stdout, "glGetShaderiv FragmentShaderID GL_COMPILE_STATUS %d\n", Result);
    glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0)
	{
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		fprintf(stdout, "%s\n", &FragmentShaderErrorMessage[0]);
	}
 
    // Link the program
    fprintf(stdout, "Linking program\n");
    _programID = glCreateProgram();
    glAttachShader(_programID, VertexShaderID);
    glAttachShader(_programID, FragmentShaderID);
    glLinkProgram(_programID);
 
    // Check the program
    glGetProgramiv(_programID, GL_LINK_STATUS, &Result);
    glGetProgramiv(_programID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    std::vector<char> ProgramErrorMessage( std::max(InfoLogLength, int(1)) );
    glGetProgramInfoLog(_programID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
    fprintf(stdout, "%s\n", &ProgramErrorMessage[0]);
 
    glDeleteShader(VertexShaderID);
    glDeleteShader(FragmentShaderID);

	glUseProgram(_programID);

	projectionMatrixID = glGetUniformLocation(_programID, "P");
	viewMatrixID = glGetUniformLocation(_programID, "V");
	modelMatrixID = glGetUniformLocation(_programID, "M");
	difuseColorID = glGetUniformLocation(_programID, "difuseColor");
}

//TEMP, will load from properly loaded .obj-s
int Graphics::AddMesh(const float _vertices[], const float _normals[], int _numVertices)
{
	Mesh newMesh(vertexData.size() / 3, _numVertices);

	vertexData.insert(vertexData.end(), _vertices, _vertices + _numVertices * 3);
	normalData.insert(normalData.end(), _normals,  _normals  + _numVertices * 3);

	meshes.push_back(newMesh);

	return meshes.size() - 1;
}

void Graphics::UpdateVBOs()
{
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float), &vertexData[0], GL_STATIC_DRAW);


	glGenBuffers(1, &normalBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
	glBufferData(GL_ARRAY_BUFFER, normalData.size() * sizeof(float), &normalData[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Graphics::DrawMesh(unsigned int meshID, glm::mat4 const &modelMatrix, glm::vec3 const &color)
{
	assert(meshID < meshes.size());

	Mesh &mesh = meshes[meshID];

	//glUseProgram(programID);

	glUniformMatrix4fv(modelMatrixID, 1, GL_FALSE, &modelMatrix[0][0]);
	glUniform3fv(difuseColorID, 1, &color[0]);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexAttribPointer(
		0,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0
	);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
	glVertexAttribPointer(
		1,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0
	);

	glDrawArrays(GL_TRIANGLES, mesh.start, mesh.numVertices);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	
	//glUseProgram(0);
}

void _check_gl_error(const char *file, int line) {
	GLenum err(glGetError());

	while (err != GL_NO_ERROR) {
		std::string error;

		switch (err) {
		case GL_INVALID_OPERATION:      error = "INVALID_OPERATION";      break;
		case GL_INVALID_ENUM:           error = "INVALID_ENUM";           break;
		case GL_INVALID_VALUE:          error = "INVALID_VALUE";          break;
		case GL_OUT_OF_MEMORY:          error = "OUT_OF_MEMORY";          break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:  error = "INVALID_FRAMEBUFFER_OPERATION";  break;
		}

		std::cout << "GL_" << error.c_str() << " - " << file << ":" << line << std::endl;
		err = glGetError();
	}
}