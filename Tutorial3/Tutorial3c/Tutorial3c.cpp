// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <cstddef>
#include <cstdio> // for C++ i/o
#include <iostream>
#include <string>

#include <GL/glew.h>	// include GLEW
#include <GLFW/glfw3.h> // include GLFW (which includes the OpenGL header)
#include <glm/glm.hpp>	// include GLM (ideally should only use the GLM headers that are actually used)
#include <glm/gtx/transform.hpp>

#include "shader.h"

#define MOVEMENT_SENSITIVITY 3.0f
#define ROTATION_SENSITIVITY 3.0f
#define SCALE_SENSITIVITY 3.0f

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
	-0.2f, 0.2f, 0.0f,	// position
	1.0f, 0.0f, 0.0f,	// colour
						// vertex 2
	-0.2f, -0.2f, 0.0f, // position
	1.0f, 0.0f, 0.0f,	// colour
						// vertex 3
	0.2f, 0.2f, 0.0f,	// position
	1.0f, 0.0f, 0.0f,	// colour

	// triangle 2
	// vertex 1
	0.2f, 0.2f, 0.0f,	// position
	0.0f, 1.0f, 0.0f,	// colour
						// vertex 2
	-0.2f, -0.2f, 0.0f, // position
	0.0f, 1.0f, 0.0f,	// colour
						// vertex 3
	0.2f, -0.2f, 0.0f,	// position
	0.0f, 1.0f, 0.0f,	// colour
};

GLuint g_VBO = 0;			   // vertex buffer object identifier
GLuint g_VAO = 0;			   // vertex array object identifier
GLuint g_shaderProgramID = 0;  // shader program identifier
GLuint g_modelMatrixIndex = 0; // location in shader
glm::mat4 g_modelMatrix;	   // object's model matrix

static void init()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // set clear background colour

	// create and compile our GLSL program from the shader files
	g_shaderProgramID = loadShaders("ModelSpaceVS.vert", "ColorFS.frag");

	// find the location of shader variables
	GLuint positionIndex = glGetAttribLocation(g_shaderProgramID, "aPosition");
	GLuint colorIndex = glGetAttribLocation(g_shaderProgramID, "aColor");
	g_modelMatrixIndex = glGetUniformLocation(g_shaderProgramID, "uModelMatrix");

	// initialise model matrix to the identity matrix
	g_modelMatrix = glm::mat4(1.0f);

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

// function used to update the scene
static void update_scene(GLFWwindow* window, float frameTime)
{
	// declare variables to transform the object
	glm::vec3 moveVec(0.0f, 0.0f, 0.0f);
	glm::vec3 scaleVec(1.0f, 1.0f, 1.0f);
	float rotateAngle = 0.0f;

	// update the variables based on keyboard input
	// update translation vector
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		moveVec.x -= MOVEMENT_SENSITIVITY * frameTime;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		moveVec.x += MOVEMENT_SENSITIVITY * frameTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		moveVec.y += MOVEMENT_SENSITIVITY * frameTime;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		moveVec.y -= MOVEMENT_SENSITIVITY * frameTime;

	// update rotation angle
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		rotateAngle += ROTATION_SENSITIVITY * frameTime;
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		rotateAngle -= ROTATION_SENSITIVITY * frameTime;

	// update scale vector
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		scaleVec.x += SCALE_SENSITIVITY * frameTime;
		scaleVec.y += SCALE_SENSITIVITY * frameTime;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		scaleVec.x -= SCALE_SENSITIVITY * frameTime;
		scaleVec.y -= SCALE_SENSITIVITY * frameTime;
	}

	// update model matrix
	g_modelMatrix *= glm::translate(moveVec) * glm::rotate(rotateAngle, glm::vec3(0.0f, 0.0f, 1.0f)) * glm::scale(scaleVec);
}

// function used to render the scene
static void render_scene()
{
	glClear(GL_COLOR_BUFFER_BIT); // clear colour buffer

	glUseProgram(g_shaderProgramID); // use the shaders associated with the shader program

	glBindVertexArray(g_VAO); // make VAO active

	// set uniform model transformation matrix
	glUniformMatrix4fv(g_modelMatrixIndex, 1, GL_FALSE, &g_modelMatrix[0][0]);

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

	// scale the object based on the I and J keys
	if (key == GLFW_KEY_I && action == GLFW_REPEAT)
	{
		g_modelMatrix *= glm::scale(glm::vec3(SCALE_SENSITIVITY + 1.0f, SCALE_SENSITIVITY + 1.0f, 1.0f));
		return;
	}
	if (key == GLFW_KEY_K && action == GLFW_REPEAT)
	{
		g_modelMatrix *= glm::scale(glm::vec3(-SCALE_SENSITIVITY + 1.0f, -SCALE_SENSITIVITY + 1.0f, 1.0f));
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
	window = glfwCreateWindow(512, 512, "Tutorial", NULL, NULL);

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

	// use sticky mode to avoid missing state changes from polling
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// initialise rendering states
	init();

	double lastUpdateTime = glfwGetTime(); // last update time
	double elapsedTime = lastUpdateTime;   // time elapsed since last update
	float frameTime = 0.0f;				   // frame time
	int frameCount = 0;					   // number of frames since last update

	// the rendering loop
	while (!glfwWindowShouldClose(window))
	{
		update_scene(window, frameTime); // update the scene
		render_scene();					 // render the scene

		glfwSwapBuffers(window); // swap buffers
		glfwPollEvents();		 // poll for events

		frameCount++;
		elapsedTime = glfwGetTime() - lastUpdateTime; // current time - last update time

		if (elapsedTime >= 1.0f) // if time since last update >= to 1 second
		{
			frameTime = static_cast<float>(1.0f / frameCount); // calculate frame time

			std::string str = "FPS = " + std::to_string(frameCount) + "; FT = " + std::to_string(frameTime);

			glfwSetWindowTitle(window, str.c_str()); // update window title

			frameCount = 0;				   // reset frame count
			lastUpdateTime += elapsedTime; // update last update time
		}
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
