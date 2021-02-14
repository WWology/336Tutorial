// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <cstddef>
#include <cstdio> // for C++ i/o
#include <iostream>

#include <GL/glew.h>	// include GLEW
#include <GLFW/glfw3.h> // include GLFW (which includes the OpenGL header)
#include <glm/glm.hpp>	// include GLM (ideally should only use the GLM headers that are actually used)
#include <glm/gtx/transform.hpp>

#include "shader.h"

#define MOVEMENT_SENSITIVITY 0.05f
#define ROTATION_SENSITIVITY 0.05f
#define SCALE_SENSITIVITY 0.05f

// struct for vertex attributes
struct Vertex
{
	GLfloat position[3];
	GLfloat color[3];
};

// global variables

Vertex g_vertices[] = {
	// vertex 1
	-0.5f, 0.5f, 0.5f, // position
	1.0f, 0.0f, 1.0f,  // colour
	// vertex 2
	-0.5f, -0.5f, 0.5f, // position
	1.0f, 0.0f, 0.0f,	// colour
	// vertex 3
	0.5f, 0.5f, 0.5f, // position
	1.0f, 1.0f, 1.0f, // colour
	// vertex 4
	0.5f, -0.5f, 0.5f, // position
	1.0f, 1.0f, 0.0f,  // colour
	// vertex 5
	-0.5f, 0.5f, -0.5f, // position
	0.0f, 0.0f, 1.0f,	// colour
	// vertex 6
	-0.5f, -0.5f, -0.5f, // position
	0.0f, 0.0f, 0.0f,	 // colour
	// vertex 7
	0.5f, 0.5f, -0.5f, // position
	0.0f, 1.0f, 1.0f,  // colour
	// vertex 8
	0.5f, -0.5f, -0.5f, // position
	0.0f, 1.0f, 0.0f,	// colour
};

GLuint g_indices[] = {
	0, 1, 2, // triangle 1
	2, 1, 3, // triangle 2
	4, 5, 0, // triangle 3
	0, 5, 1, // ...
	2, 3, 6,
	6, 3, 7,
	4, 0, 6,
	6, 0, 2,
	1, 5, 3,
	3, 5, 7,
	5, 4, 7,
	7, 4, 6, // triangle 12
};

GLuint g_IBO = 0;			  // index buffer object identifier
GLuint g_VBO = 0;			  // vertex buffer object identifier
GLuint g_VAO = 0;			  // vertex array object identifier
GLuint g_shaderProgramID = 0; // shader program identifier
GLuint g_MVP_Index = 0;		  // location in shader
GLuint g_windowWidth = 512;
GLuint g_windowHeight = 512;
glm::mat4 g_modelMatrix;	  // object's model matrix
glm::mat4 g_viewMatrix;		  // view matrix
glm::mat4 g_projectionMatrix; // projection matrix

static void init(GLFWwindow* window)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // set clear background colour

	//glEnable(GL_DEPTH_TEST);	// enable depth buffer test

	// create and compile our GLSL program from the shader files
	g_shaderProgramID = loadShaders("MVP_VS.vert", "ColorFS.frag");

	// find the location of shader variables
	GLuint positionIndex = glGetAttribLocation(g_shaderProgramID, "aPosition");
	GLuint colorIndex = glGetAttribLocation(g_shaderProgramID, "aColor");
	g_MVP_Index = glGetUniformLocation(g_shaderProgramID, "uModelViewProjectionMatrix");

	// initialise model matrix to the identity matrix
	g_modelMatrix = glm::mat4(1.0f);

	// initialise the view matrix
	g_viewMatrix = glm::lookAt(glm::vec3(0.0f, 0.0f, 4.0f), glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	// get the framebuffer width and height in order to calculate the aspect ratio
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	float aspectRatio = static_cast<float>(width) / height;

	// initialise the projection matrix
	g_projectionMatrix = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f);

	// generate identifier for VBO and copy data to GPU
	glGenBuffers(1, &g_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, g_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertices), g_vertices, GL_STATIC_DRAW);

	// generate identifier for IBO and copy data to GPU
	glGenBuffers(1, &g_IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(g_indices), g_indices, GL_STATIC_DRAW);

	// generate identifiers for VAO
	glGenVertexArrays(1, &g_VAO);

	// create VAO and specify VBO data
	glBindVertexArray(g_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, g_VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_IBO);
	// interleaved attributes
	glVertexAttribPointer(positionIndex, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, position)));
	glVertexAttribPointer(colorIndex, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, color)));

	glEnableVertexAttribArray(positionIndex); // enable vertex attributes
	glEnableVertexAttribArray(colorIndex);
}

// function used to update the scene
static void update_scene(GLFWwindow* window)
{
	// declare variables to transform the object
	static glm::vec3 moveVec(0.0f, 0.0f, 0.0f);
	static glm::vec3 scaleVec(1.0f, 1.0f, 1.0f);
	static float rotateAngleX = 0.0f;
	static float rotateAngleY = 0.0f;

	// update the variables based on keyboard input
	// update translation vector
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		moveVec.x -= MOVEMENT_SENSITIVITY;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		moveVec.x += MOVEMENT_SENSITIVITY;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		moveVec.y += MOVEMENT_SENSITIVITY;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		moveVec.y -= MOVEMENT_SENSITIVITY;

	// update rotation angle
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
		rotateAngleY -= ROTATION_SENSITIVITY;
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
		rotateAngleY += ROTATION_SENSITIVITY;
	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
		rotateAngleX -= ROTATION_SENSITIVITY;
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
		rotateAngleX += ROTATION_SENSITIVITY;

	// update scale vector
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		scaleVec.x += SCALE_SENSITIVITY;
		scaleVec.y += SCALE_SENSITIVITY;
		scaleVec.z += SCALE_SENSITIVITY;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		scaleVec.x -= SCALE_SENSITIVITY;
		scaleVec.y -= SCALE_SENSITIVITY;
		scaleVec.z -= SCALE_SENSITIVITY;
	}

	// update model matrix
	g_modelMatrix = glm::translate(moveVec) * glm::rotate(rotateAngleX, glm::vec3(1.0f, 0.0f, 0.0f)) * glm::rotate(rotateAngleY, glm::vec3(0.0f, 1.0f, 0.0f)) * glm::scale(scaleVec);
}

// function used to render the scene
static void render_scene()
{
	glClear(GL_COLOR_BUFFER_BIT); // clear colour buffer
								  //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// clear colour buffer and depth buffer

	// set the viewport transformation
	glViewport(0, 0, g_windowWidth, g_windowHeight);
	//glViewport(100, 100, g_windowWidth - 200, g_windowHeight - 200);

	glUseProgram(g_shaderProgramID); // use the shaders associated with the shader program

	glBindVertexArray(g_VAO); // make VAO active

	glm::mat4 MVP = g_projectionMatrix * g_viewMatrix * g_modelMatrix;
	// set uniform model transformation matrix
	glUniformMatrix4fv(g_MVP_Index, 1, GL_FALSE, &MVP[0][0]);

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0); // display the vertices based on their indices and primitive type

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

// resize callback function
static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	g_windowWidth = width;
	g_windowHeight = height;
}

// error callback function
static void error_callback(int error, const char* description)
{
	std::cerr << description << std::endl; // output error description
}

int main(void)
{
	GLFWwindow* window = NULL; // pointer to a GLFW window handle

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
	window = glfwCreateWindow(g_windowWidth, g_windowHeight, "Tutorial", NULL, NULL);

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
		// if failed to initialise GLEW
		std::cerr << "GLEW initialisation failed" << std::endl;
		exit(EXIT_FAILURE);
	}

	// set key callback function
	glfwSetKeyCallback(window, key_callback);
	//glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// use sticky mode to avoid missing state changes from polling
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// initialise rendering states
	init(window);

	// the rendering loop
	while (!glfwWindowShouldClose(window))
	{
		update_scene(window); // update the scene
		render_scene();		  // render the scene

		glfwSwapBuffers(window); // swap buffers
		glfwPollEvents();		 // poll for events
	}

	// clean up
	glDeleteProgram(g_shaderProgramID);
	glDeleteBuffers(1, &g_IBO);
	glDeleteBuffers(1, &g_VBO);
	glDeleteVertexArrays(1, &g_VAO);

	// close the window and terminate GLFW
	glfwDestroyWindow(window);
	glfwTerminate();

	exit(EXIT_SUCCESS);
}
