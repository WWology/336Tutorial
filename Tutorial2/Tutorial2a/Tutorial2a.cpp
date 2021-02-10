// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <cstdio>
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

GLfloat g_vertexPos[] = {
	0.0f, 0.0f, 0.0f // (X, Y, Z)
};

GLuint g_VBO = 0; // Vertex Buffer object identifier
GLuint g_VAO = 0; // Vertex Buffer object identifier

// Initialize various render states
static void init()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glGenBuffers(1, &g_VBO);														 // Generate unused VBO identifier
	glBindBuffer(GL_ARRAY_BUFFER, g_VBO);											 // Bind the VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertexPos), g_vertexPos, GL_STATIC_DRAW); // Copy data to buffer

	glGenVertexArrays(1, &g_VBO);
	glBindVertexArray(g_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, g_VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glEnableVertexAttribArray(0);
}

static void render_scene()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glBindBuffer(GL_ARRAY_BUFFER, g_VAO);

	glDrawArrays(GL_POINTS, 0, 1);

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

	glfwSetErrorCallback(error_callback);

	if (!glfwInit())
	{
		std::cerr << "GLFW Failed to initialize" << std::endl;
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(800, 800, "Tutorial2a", NULL, NULL);

	if (window == NULL)
	{
		glfwTerminate();
		std::cerr << "Window is NULL" << std::endl;
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Glew failed to initialize" << std::endl;
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

	glDeleteBuffers(1, &g_VBO);
	glDeleteVertexArrays(1, &g_VAO);

	glfwDestroyWindow(window);
	glfwTerminate();

	exit(EXIT_SUCCESS);
}