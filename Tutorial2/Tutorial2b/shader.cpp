// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <fstream>
#include <iostream>
#include <string>

#include <GL/glew.h>

#include "shader.h"

GLuint loadShaders(const std::string& vertexShaderFile, const std::string& fragmentShaderFile)
{
	GLint status; // For checking compile and linking status

	// Load vertex shader code from file
	std::string vertexShaderCode;									  // To store shader code
	std::ifstream vertexShaderStream(vertexShaderFile, std::ios::in); // Open File Stream

	// Check whether file was succesfully open
	if (vertexShaderStream.is_open())
	{
		std::string line = "";
		while (std::getline(vertexShaderStream, line))
			vertexShaderCode += line + "\n";

		vertexShaderStream.close();
	}
	else
	{
		std::cout << "Failed to open vertex shader file - " << vertexShaderFile << std::endl;
		exit(EXIT_FAILURE);
	}

	std::string fragmentShaderCode;
	std::ifstream fragmentShaderStream(fragmentShaderFile, std::ios::in);

	if (fragmentShaderStream.is_open())
	{
		std::string line = "";
		while (std::getline(fragmentShaderStream, line))
			fragmentShaderCode += line + "\n";

		fragmentShaderStream.close();
	}
	else
	{
		std::cout << "Failed to open fragment shader file - " << fragmentShaderFile << std::endl;
		exit(EXIT_FAILURE);
	}

	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	const GLchar* vShaderCode = vertexShaderCode.c_str();
	const GLchar* fShaderCode = fragmentShaderCode.c_str();
	glShaderSource(vertexShaderID, 1, &vShaderCode, NULL);
	glShaderSource(fragmentShaderID, 1, &fShaderCode, NULL);

	glCompileShader(vertexShaderID);

	status = GL_FALSE;
	glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &status);

	if (status == GL_FALSE)
	{
		std::cout << "Failed to compile vertex shader - " << vertexShaderFile << std::endl;

		int infoLogLength;
		glGetShaderiv(vertexShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* errorMessage = new char[infoLogLength + 1];
		glGetShaderInfoLog(vertexShaderID, infoLogLength, NULL, errorMessage);
		std::cout << errorMessage << std::endl;
		delete[] errorMessage;

		exit(EXIT_FAILURE);
	}

	glCompileShader(fragmentShaderID);

	status = GL_FALSE;
	glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &status);

	if (status == GL_FALSE)
	{
		std::cout << "Failed to compile fragment shader - " << fragmentShaderFile << std::endl;

		int infoLogLength;
		glGetShaderiv(fragmentShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* errorMessage = new char[infoLogLength + 1];
		glGetShaderInfoLog(fragmentShaderID, infoLogLength, NULL, errorMessage);
		std::cout << errorMessage << std::endl;
		delete[] errorMessage;

		exit(EXIT_FAILURE);
	}

	GLuint programID = glCreateProgram();

	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);

	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);

	glLinkProgram(programID);

	status = GL_FALSE;
	glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &status);

	if (status == GL_FALSE)
	{
		std::cout << "Failed to link program object " << fragmentShaderFile << std::endl;

		int infoLogLength;
		glGetShaderiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* errorMessage = new char[infoLogLength + 1];
		glGetShaderInfoLog(programID, infoLogLength, NULL, errorMessage);
		std::cout << errorMessage << std::endl;
		delete[] errorMessage;

		exit(EXIT_FAILURE);
	}

	return programID;
}