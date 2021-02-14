#include <cstdio>		// for C++ i/o
#include <iostream>
#include <string>
#include <cstddef>
using namespace std;	// to avoid having to use std::

#include <GLEW/glew.h>	// include GLEW
#include <GLFW/glfw3.h>	// include GLFW (which includes the OpenGL header)
#include <glm/glm.hpp>	// include GLM (ideally should only use the GLM headers that are actually used)
#include <glm/gtx/transform.hpp>
using namespace glm;	// to avoid having to use glm::

#include <AntTweakBar.h>
#include "shader.h"

// struct for vertex attributes
typedef struct Vertex
{
	GLfloat position[3];
	GLfloat colour[3];
} Vertex;

// Global variables
Vertex g_vertices[] = {
	// plane
	-0.5f, 0.5f, 0.0f,	// position
	1.0f, 0.0f, 0.0f,	// colour
	-0.5f, -0.5f, 0.0f,	// position
	0.0f, 1.0f, 0.0f,	// colour
	0.5f, 0.5f, 0.0f,	// position
	0.0f, 0.0f, 1.0f,	// colour
	0.5f, 0.5f, 0.0f,	// position
	0.0f, 0.0f, 1.0f,	// colour
	-0.5f, -0.5f, 0.0f,	// position
	0.0f, 1.0f, 0.0f,	// colour
	0.5f, -0.5f, 0.0f,	// position
	1.0f, 1.0f, 0.0f,	// colour
};

GLuint g_VBO = 0;				// vertex buffer object identifier
GLuint g_VAO = 0;				// vertex array object identifier
GLuint g_shaderProgramID = 0;	// shader program identifier

// locations in shader
GLuint g_MVP_Index;
GLuint g_alphaIndex;

glm::mat4 g_modelMatrix[2];		// object's model matrix
glm::mat4 g_viewMatrix;			// view matrix
glm::mat4 g_projectionMatrix;	// projection matrix

glm::vec3 g_translation = glm::vec3(0.3f, 0.3f, 0.1f);
glm::vec3 g_backgroundColour = glm::vec3(0.0f, 0.0f, 0.0f);
float g_rotation = 90.0f;
float g_alpha = 0.5f;

bool g_bBlend = true;
bool g_bDepth = true;
bool g_bOrder = true;

GLuint g_windowWidth = 800;		// window dimensions
GLuint g_windowHeight = 600;

static void init(GLFWwindow* window)
{
	glEnable(GL_DEPTH_TEST);	// enable depth buffer test

	glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);

	// create and compile our GLSL program from the shader files
	g_shaderProgramID = loadShaders("AlphaBlendingVS.vert", "AlphaBlendingFS.frag");

	// find the location of shader variables
	GLuint positionIndex = glGetAttribLocation(g_shaderProgramID, "aPosition");
	GLuint colourIndex = glGetAttribLocation(g_shaderProgramID, "aColour");

	g_MVP_Index = glGetUniformLocation(g_shaderProgramID, "uModelViewProjectionMatrix");
	g_alphaIndex = glGetUniformLocation(g_shaderProgramID, "uAlpha");

	// initialise model matrix to the identity matrix
	g_modelMatrix[0] = glm::mat4(1.0f);
	g_modelMatrix[1] = glm::mat4(1.0f);

	// initialise view matrix
	g_viewMatrix = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	float aspectRatio = static_cast<float>(width) / height;

	// initialise projection matrix
	g_projectionMatrix = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f);

	// generate identifier for VBOs and copy data to GPU
	glGenBuffers(1, &g_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, g_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertices), g_vertices, GL_STATIC_DRAW);

	// generate identifiers for VAO
	glGenVertexArrays(1, &g_VAO);

	// create VAO and specify VBO data
	glBindVertexArray(g_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, g_VBO);
	glVertexAttribPointer(positionIndex, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, position)));
	glVertexAttribPointer(colourIndex, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, colour)));

	glEnableVertexAttribArray(positionIndex);	// enable vertex attributes
	glEnableVertexAttribArray(colourIndex);
}

// function used to update the scene
static void update_scene()
{
	// update model matrix
	g_modelMatrix[1] = glm::translate(g_translation)
		* glm::rotate(glm::radians(g_rotation), vec3(0.0f, 0.0f, 1.0f));
}

// function used to render the scene
static void render_scene()
{
	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(g_shaderProgramID);	// use the shaders associated with the shader program

	glBindVertexArray(g_VAO);		// make VAO active

	glm::mat4 MVP;

	if (g_bOrder)
	{
		// set shader variables
		MVP = g_projectionMatrix * g_viewMatrix * g_modelMatrix[0];
		glUniformMatrix4fv(g_MVP_Index, 1, GL_FALSE, &MVP[0][0]);
		glUniform1f(g_alphaIndex, 0.5);

		// draw plane
		glDrawArrays(GL_TRIANGLES, 0, 6);

		// set shader variables
		MVP = g_projectionMatrix * g_viewMatrix * g_modelMatrix[1];
		glUniformMatrix4fv(g_MVP_Index, 1, GL_FALSE, &MVP[0][0]);
		glUniform1fv(g_alphaIndex, 1, &g_alpha);

		// draw plane
		glDrawArrays(GL_TRIANGLES, 0, 6);

	}
	else
	{
		// set shader variables
		MVP = g_projectionMatrix * g_viewMatrix * g_modelMatrix[1];
		glUniformMatrix4fv(g_MVP_Index, 1, GL_FALSE, &MVP[0][0]);
		glUniform1fv(g_alphaIndex, 1, &g_alpha);

		// draw plane
		glDrawArrays(GL_TRIANGLES, 0, 6);

		// set shader variables
		MVP = g_projectionMatrix * g_viewMatrix * g_modelMatrix[0];
		glUniformMatrix4fv(g_MVP_Index, 1, GL_FALSE, &MVP[0][0]);
		glUniform1f(g_alphaIndex, 0.5);

		// draw plane
		glDrawArrays(GL_TRIANGLES, 0, 6);

	}

	glFlush();	// flush the pipeline
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

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	// pass mouse data to tweak bar
	TwEventMousePosGLFW(xpos, ypos);
}

static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	// pass mouse data to tweak bar
	TwEventMouseButtonGLFW(button, action);
}

// error callback function
static void error_callback(int error, const char* description)
{
	cerr << description << endl;	// output error description
}

int main(void)
{
	GLFWwindow* window = NULL;	// pointer to a GLFW window handle
	TwBar *TweakBar;			// pointer to a tweak bar

	glfwSetErrorCallback(error_callback);	// set error callback function

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

	glfwMakeContextCurrent(window);	// set window context as the current context
	glfwSwapInterval(1);			// swap buffer interval

	// initialise GLEW
	if (glewInit() != GLEW_OK)
	{
		// if failed to initialise GLEW
		cerr << "GLEW initialisation failed" << endl;
		exit(EXIT_FAILURE);
	}

	// set key callback function
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, cursor_position_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	// initialise AntTweakBar
	TwInit(TW_OPENGL_CORE, NULL);

	// give tweak bar the size of graphics window
	TwWindowSize(g_windowWidth, g_windowHeight);
	TwDefine(" TW_HELP visible=false ");	// disable help menu
	TwDefine(" GLOBAL fontsize=3 ");		// set large font size

	// create a tweak bar
	TweakBar = TwNewBar("Main");
	TwDefine(" Main label='Controls' refresh=0.02 text=light size='220 300' ");

	// blend entries
	TwAddVarRW(TweakBar, "Blend", TW_TYPE_BOOLCPP, &g_bBlend, " group='Blending' ");
	TwAddVarRW(TweakBar, "DepthTest", TW_TYPE_BOOLCPP, &g_bDepth, " group='Blending' ");
	TwAddVarRW(TweakBar, "Order", TW_TYPE_BOOLCPP, &g_bOrder, " group='Blending' ");
	TwAddVarRW(TweakBar, "Alpha", TW_TYPE_FLOAT, &g_alpha, " group='Blending' min=0.0 max=1.0 step=0.01 ");

	TwAddVarRW(TweakBar, "Colour", TW_TYPE_COLOR3F, &g_backgroundColour[0], " group='Background' ");

	// create transformation entries
	TwAddVarRW(TweakBar, "Translate x", TW_TYPE_FLOAT, &g_translation[0], " group='Transformation' min=-2.0 max=2.0 step=0.1 ");
	TwAddVarRW(TweakBar, "Translate y", TW_TYPE_FLOAT, &g_translation[1], " group='Transformation' min=-2.0 max=2.0 step=0.1 ");
	TwAddVarRW(TweakBar, "Translate z", TW_TYPE_FLOAT, &g_translation[2], " group='Transformation' min=-5.0 max=5.0 step=0.1 ");

	TwAddVarRW(TweakBar, "Rotation", TW_TYPE_FLOAT, &g_rotation, " group='Transformation' min=-180.0 max=180.0 step=1.0 ");

	// initialise rendering states
	init(window);

	// the rendering loop
	while (!glfwWindowShouldClose(window))
	{
		update_scene();		// update the scene

		glClearColor(g_backgroundColour[0], g_backgroundColour[1], g_backgroundColour[2], 1.0f);

		if (g_bBlend)
			glEnable(GL_BLEND);
		else
			glDisable(GL_BLEND);

		if (g_bDepth)
			glEnable(GL_DEPTH_TEST);
		else
			glDisable(GL_DEPTH_TEST);

		render_scene();		// render the scene

		TwDraw();			// draw tweak bar(s)

		glfwSwapBuffers(window);	// swap buffers
		glfwPollEvents();			// poll for events
	}

	// clean up
	glDeleteProgram(g_shaderProgramID);
	glDeleteBuffers(1, &g_VBO);
	glDeleteVertexArrays(1, &g_VAO);

	// uninitialise tweak bar
	TwTerminate();

	// close the window and terminate GLFW
	glfwDestroyWindow(window);
	glfwTerminate();

	exit(EXIT_SUCCESS);
}
