// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <cstdio>
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "shader.h"

GLfloat g_vertexPos[] = {
	-1.0f, -1.0f, 0.0f, // Vertex 1
	1.0f, -1.0f, 0.0f,	// Vertex 2
	0.0f, 1.0f, 0.0f,	// Vertex 3
};

GLuint g_VBO = 0;
GLuint g_VAO = 0;
GLuint g_shaderProgramID = 0;

static void init()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	g_shaderProgramID = loadShaders("SimpleVS.vert", "SimpleFS.frag");

	glGenBuffers(1, &g_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, g_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertexPos), g_vertexPos, GL_STATIC_DRAW);

	glGenVertexArrays(1, &g_VAO);
	glBindVertexArray(g_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, g_VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glEnableVertexAttribArray(0);
}

static void render_scene()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(g_shaderProgramID);
	glBindVertexArray(g_VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glFlush();
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
		return;
	}
}

static void error_callback(int error, const char* description)
{
	std::cerr << description << std::endl;
}

int main(void)
{
	GLFWwindow* window = NULL;

	glfwSetErrorCallback(error_callback); // set error callback function

	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(800, 600, "Tutorial 2b", NULL, NULL);

	if (window == NULL)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK)
	{
		std::cerr << "GLEW failed to initialize" << std::endl;
		exit(EXIT_FAILURE);
	}

	glfwSetKeyCallback(window, key_callback);

	init();

	while (!glfwWindowShouldClose(window))
	{
		render_scene();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteProgram(g_shaderProgramID);
	glDeleteBuffers(1, &g_VBO);
	glDeleteVertexArrays(1, &g_VAO);

	glfwDestroyWindow(window);
	glfwTerminate();

	exit(EXIT_SUCCESS);
}
