// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <cstddef>
#include <cstdio> // for C I/O
#include <iostream>

#include <GL/glew.h>	// include GLEW
#include <GLFW/glfw3.h> // include GLFW (which includes the OpenGL header)

#include "shader.h"

// struct for vertex attributes
struct Vertex
{
	GLfloat position[3];
	GLfloat color[3];
};

// global variables
Vertex g_vertices[] = {
	// triangle 1
	// vertex 1
	-0.5f, 0.5f, 0.0f,	// position
	1.0f, 0.0f, 0.0f,	// colour
						//vertex 2
	-0.5f, -0.5f, 0.0f, // position
	1.0f, 0.0f, 0.0f,	// colour
						// vertex 3
	0.5f, 0.5f, 0.0f,	// position
	1.0f, 0.0f, 0.0f,	// colour

	// triangle 2
	// vertex 1
	0.5f, 0.5f, 0.0f,	// position
	0.0f, 1.0f, 0.0f,	// colour
						// vertex 2
	-0.5f, -0.5f, 0.0f, // position
	0.0f, 1.0f, 0.0f,	// colour
						// vertex 3
	0.5f, -0.5f, 0.0f,	// position
	0.0f, 1.0f, 0.0f,	// colour
};

GLuint g_VBO = 0;			  // vertex buffer object identifier
GLuint g_VAO = 0;			  // vertex array object identifier
GLuint g_shaderProgramID = 0; // shader program identifier

static void init()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // set clear background colour

	// create and compile a GLSL program from the shader files
	g_shaderProgramID = loadShaders("ColorVS.vert", "ColorFS.frag");

	// find the location of shader variables
	GLuint positionIndex = glGetAttribLocation(g_shaderProgramID, "aPosition");
	GLuint colorIndex = glGetAttribLocation(g_shaderProgramID, "aColor");

	// generate identifier for VBO and copy data to GPU
	glGenBuffers(1, &g_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, g_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertices), g_vertices, GL_STATIC_DRAW);

	// generate identifier for VAO
	glGenVertexArrays(1, &g_VAO);

	// create VAO and specify VBO data
	glBindVertexArray(g_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, g_VBO);

	// interleaved attributes
	glVertexAttribPointer(positionIndex, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, position)));
	glVertexAttribPointer(colorIndex, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, color)));

	glEnableVertexAttribArray(positionIndex); // enable vertex attributes
	glEnableVertexAttribArray(colorIndex);
}

// function used to render the scene
static void render_scene()
{
	glClear(GL_COLOR_BUFFER_BIT); // clear colour buffer

	glUseProgram(g_shaderProgramID); // use the shaders associated with the shader program

	glBindVertexArray(g_VAO); // make VAO active

	glDrawArrays(GL_TRIANGLES, 0, 6); // display the vertices based on the primitive type

	glFlush(); // flush the pipeline
}

// key press or release callback function
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// quit if the ESCAPE key was press
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		// set flag to close the window
		glfwSetWindowShouldClose(window, GL_TRUE);
		return;
	}
}

// error callback function
static void error_callback(int error, const char* description)
{
	std::cerr << description << std::endl; // output error description
}

int main(void)
{
	GLFWwindow* window = NULL;			  // pointer to a GLFW window handle
	glfwSetErrorCallback(error_callback); // set error callback function

	// initialise GLFW
	if (!glfwInit())
	{
		// if failed to initialise GLFW
		exit(EXIT_FAILURE);
	}

	// minimum OpenGL version 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// create a window and its OpenGL context
	window = glfwCreateWindow(512, 512, "Tutorial2d", NULL, NULL);

	// if failed to create window
	if (window == NULL)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window); // set window context as the current context
	glfwSwapInterval(1);			// swap buffer interval

	// initialise GLEW
	if (glewInit() != GLEW_OK)
	{
		// if failed to initialize GLEW
		std::cerr << "GLEW initialization failed" << std::endl;
		exit(EXIT_FAILURE);
	}

	// set key callback function
	glfwSetKeyCallback(window, key_callback);

	// initialize rendering states
	init();

	// the rendering loop
	while (!glfwWindowShouldClose(window))
	{
		render_scene(); // render the scene

		glfwSwapBuffers(window); // swap buffers
		glfwPollEvents();		 // poll for events
	}

	// clean up
	glDeleteProgram(g_shaderProgramID);
	glDeleteBuffers(1, &g_VBO);
	glDeleteVertexArrays(1, &g_VAO);

	// close the window and terminate GLFW
	glfwDestroyWindow(window);
	glfwTerminate();

	exit(EXIT_SUCCESS);
}