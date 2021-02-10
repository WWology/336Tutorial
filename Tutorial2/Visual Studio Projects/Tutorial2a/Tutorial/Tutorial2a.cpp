#include <cstdio>		// for C++ i/o
#include <iostream>
using namespace std;	// to avoid having to use std::

#include <GLEW/glew.h>	// include GLEW
#include <GLFW/glfw3.h>	// include GLFW (which includes the OpenGL header)

// global variables
GLfloat g_vertexPos[] = {
	0.0f, 0.0f, 0.0f	// (x, y, z) coordinate
};

/*
GLfloat g_vertexPos[] = {
	-1.0f, -1.0f, 0.0f,		// vertex 1 (x, y, z) coordinate
	1.0f, -1.0f, 0.0f,		// vertex 2 (x, y, z) coordinate
	0.0f, 1.0f, 0.0f		// vertex 3 (x, y, z) coordinate
};
*/

GLuint g_VBO = 0;	// vertex buffer object identifier
GLuint g_VAO = 0;	// vertex array object identifier

// function to initialise various render states
static void init()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);	// set clear background colour

	glGenBuffers(1, &g_VBO);				// generate unused VBO identifier
	glBindBuffer(GL_ARRAY_BUFFER, g_VBO);	// bind the VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertexPos), g_vertexPos, GL_STATIC_DRAW); 	// copy data to buffer

	glGenVertexArrays(1, &g_VAO);			// generate unused VAO identifier
	glBindVertexArray(g_VAO);				// create VAO
	glBindBuffer(GL_ARRAY_BUFFER, g_VBO);	// bind the VBO
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);	// specify the form of the data

	glEnableVertexAttribArray(0);			// enable vertex attributes at index 0
}

// function used to render the scene
static void render_scene()
{
	glClear(GL_COLOR_BUFFER_BIT);	// clear colour buffer

	glBindBuffer(GL_ARRAY_BUFFER, g_VAO);	// make the VAO active

	glDrawArrays(GL_POINTS, 0, 1);			// display the vertices based on the primitive type
	//glDrawArrays(GL_TRIANGLES, 0, 3);			// display the vertices based on the primitive type

	glFlush();	// flush the pipeline
}

// key press or release callback function
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// quit if the ESCAPE key was press
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) 
	{
		// set flag to close the window
		glfwSetWindowShouldClose(window, GL_TRUE);
		return;
	}
}

// error callback function
static void error_callback(int error, const char* description)
{
    cerr << description << endl;	// output error description
}

int main(void)
{
	GLFWwindow* window = NULL;	// pointer to a GLFW window handle

    glfwSetErrorCallback(error_callback);	// set error callback function

	// initialise GLFW
	if(!glfwInit()) 
	{
		// if failed to initialise GLFW
		exit(EXIT_FAILURE);
	}

	// minimum OpenGL version 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// create a window and its OpenGL context
	window = glfwCreateWindow(800, 600, "Tutorial", NULL, NULL);

	// if failed to create window
	if(window == NULL)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);	// set window context as the current context
	glfwSwapInterval(1);			// swap buffer interval

	// initialise GLEW
	if(glewInit() != GLEW_OK) 
	{
		// if failed to initialise GLEW
		cerr << "GLEW initialisation failed" << endl;
		exit(EXIT_FAILURE);
	}

	// set key callback function
	glfwSetKeyCallback(window, key_callback);

	// initialise render states
	init();

	// the rendering loop
	while(!glfwWindowShouldClose(window))
	{
		render_scene();				// render the scene

		glfwSwapBuffers(window);	// swap buffers
		glfwPollEvents();			// poll for events
	}

	// clean up
	glDeleteBuffers(1, &g_VBO);
	glDeleteVertexArrays(1, &g_VAO);

	// close the window and terminate GLFW
    glfwDestroyWindow(window);
	glfwTerminate();

    exit(EXIT_SUCCESS);
}

