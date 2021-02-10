// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <cstdio>
#include <iostream>

// GLEW & GLFW
#include <GL/glew.h>
#include <GLFW/glfw3.h>

static void render_scene()
{
	glClear(GL_COLOR_BUFFER_BIT);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	if (key == GLFW_KEY_0 && action == GLFW_PRESS)
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	}
	if (key == GLFW_KEY_1 && action == GLFW_PRESS)
	{
		glClearColor(1.0f, 0.72f, 0.67, 1.0f);
	}
}

static void mouse_callback(GLFWwindow* window, int button, int action, int mods)
{
	double xpos, ypos;
	// Quit if the right mouse button is pressed
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
	{
		// Set flag to close the window
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		glfwGetCursorPos(window, &xpos, &ypos);
		std::cout << xpos << "," << ypos << std::endl;
	}
}

static void error_callback(int error, const char* description)
{
	std::cerr << description << std::endl;
}

static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glfwSetWindowTitle(window, "Creating a window (Resized)");
}

int main(void)
{
	GLFWwindow* window = NULL;

	glfwSetErrorCallback(error_callback);

	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(800, 600, "Creating a window", NULL, NULL);

	if (window == NULL)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK)
	{
		std::cerr << "GLEW initialisation failed" << std::endl;
		exit(EXIT_FAILURE);
	}

	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_callback);
	glfwSetWindowSizeCallback(window, framebuffer_size_callback);

	// glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	// glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, GL_TRUE);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	while (!glfwWindowShouldClose(window))
	{
		render_scene();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();

	exit(EXIT_SUCCESS);
}