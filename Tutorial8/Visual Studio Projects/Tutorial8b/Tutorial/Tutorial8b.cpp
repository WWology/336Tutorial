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
	GLfloat normal[3];
} Vertex;

// light and material structs
typedef struct Light
{
	glm::vec3 position;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
} Light;

typedef struct Material
{
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shininess;
} Material;

// Global variables
Vertex g_vertices[] = {
	// plane
	-5.0f, -1.0f, 5.0f,	// position
	0.0f, 1.0f, 0.0f,	// normal
	5.0f, -1.0f, 5.0f,	// position
	0.0f, 1.0f, 0.0f,	// normal
	-5.0f, -1.0f, -5.0f,	// position
	0.0f, 1.0f, 0.0f,	// normal
	-5.0f, -1.0f, -5.0f,	// position
	0.0f, 1.0f, 0.0f,	// normal
	5.0f, -1.0f, 5.0f,	// position
	0.0f, 1.0f, 0.0f,	// normal
	5.0f, -1.0f, -5.0f,	// position
	0.0f, 1.0f, 0.0f,	// normal

	// unit cube
	-0.5f, 0.5f, 0.5f,	// position
	0.0f, 0.0f, 1.0f,	// normal
	-0.5f, -0.5f, 0.5f,	// position
	0.0f, 0.0f, 1.0f,	// normal
	0.5f, 0.5f, 0.5f,	// position
	0.0f, 0.0f, 1.0f,	// normal
	0.5f, 0.5f, 0.5f,	// position
	0.0f, 0.0f, 1.0f,	// normal
	-0.5f, -0.5f, 0.5f, // position
	0.0f, 0.0f, 1.0f,	// normal
	0.5f, -0.5f, 0.5f,	// position
	0.0f, 0.0f, 1.0f,	// normal
	0.5f, 0.5f, 0.5f,	// position
	1.0f, 0.0f, 0.0f,	// normal
	0.5f, -0.5f, 0.5f,	// position
	1.0f, 0.0f, 0.0f,	// normal
	0.5f, 0.5f, -0.5f,	// position
	1.0f, 0.0f, 0.0f,	// normal
	0.5f, 0.5f, -0.5f,	// position
	1.0f, 0.0f, 0.0f,	// normal
	0.5f, -0.5f, 0.5f,	// position
	1.0f, 0.0f, 0.0f,	// normal
	0.5f, -0.5f, -0.5f, // position
	1.0f, 0.0f, 0.0f,	// normal
	0.5f, -0.5f, -0.5f, // position
	0.0f, 0.0f, -1.0f,	// normal
	-0.5f, -0.5f, -0.5f,// position
	0.0f, 0.0f, -1.0f,	// normal
	0.5f, 0.5f, -0.5f,	// position
	0.0f, 0.0f, -1.0f,	// normal
	0.5f, 0.5f, -0.5f,	// position
	0.0f, 0.0f, -1.0f,	// normal
	-0.5f, -0.5f, -0.5f,// position
	0.0f, 0.0f, -1.0f,	// normal
	-0.5f, 0.5f, -0.5f, // position
	0.0f, 0.0f, -1.0f,	// normal
	-0.5f, -0.5f, 0.5f, // position
	-1.0f, 0.0f, 0.0f,	// normal
	-0.5f, 0.5f, 0.5f,	// position
	-1.0f, 0.0f, 0.0f,	// normal
	-0.5f, -0.5f, -0.5f, // position
	-1.0f, 0.0f, 0.0f,	// normal
	-0.5f, -0.5f, -0.5f, // position
	-1.0f, 0.0f, 0.0f,	// normal
	-0.5f, 0.5f, 0.5f,	// position
	-1.0f, 0.0f, 0.0f,	// normal
	-0.5f, 0.5f, -0.5f, // position
	-1.0f, 0.0f, 0.0f,	// normal
	-0.5f, 0.5f, 0.5f,	// position
	0.0f, 1.0f, 0.0f,	// normal
	0.5f, 0.5f, 0.5f,	// position
	0.0f, 1.0f, 0.0f,	// normal
	-0.5f, 0.5f, -0.5f, // position
	0.0f, 1.0f, 0.0f,	// normal
	-0.5f, 0.5f, -0.5f, // position
	0.0f, 1.0f, 0.0f,	// normal
	0.5f, 0.5f, 0.5f,	// position
	0.0f, 1.0f, 0.0f,	// normal
	0.5f, 0.5f, -0.5f,	// position
	0.0f, 1.0f, 0.0f,	// normal
	0.5f, -0.5f, 0.5f,	// position
	0.0f, -1.0f, 0.0f,	// normal
	-0.5f, -0.5f, 0.5f, // position
	0.0f, -1.0f, 0.0f,	// normal
	0.5f, -0.5f, -0.5f, // position
	0.0f, -1.0f, 0.0f,	// normal
	0.5f, -0.5f, -0.5f, // position
	0.0f, -1.0f, 0.0f,	// normal
	-0.5f, -0.5f, 0.5f, // position
	0.0f, -1.0f, 0.0f,	// normal
	-0.5f, -0.5f, -0.5f,// position
	0.0f, -1.0f, 0.0f,	// normal
};

GLuint g_VBO = 0;				// vertex buffer object identifier
GLuint g_VAO = 0;				// vertex array object identifier
GLuint g_shaderProgramID = 0;	// shader program identifier

// locations in shader
GLuint g_MVP_Index = 0;
GLuint g_MV_Index = 0;
GLuint g_V_Index = 0;
GLuint g_lightPositionIndex = 0;
GLuint g_lightAmbientIndex = 0;
GLuint g_lightDiffuseIndex = 0;
GLuint g_lightSpecularIndex = 0;
GLuint g_materialAmbientIndex = 0;
GLuint g_materialDiffuseIndex = 0;
GLuint g_materialSpecularIndex = 0;
GLuint g_materialShininessIndex = 0;

glm::mat4 g_modelMatrix[3];		// object model matrices
glm::mat4 g_viewMatrix;			// view matrix
glm::mat4 g_projectionMatrix;	// projection matrix

Light g_light;					// light properties
Material g_material[3];			// material properties

GLuint g_windowWidth = 800;		// window dimensions
GLuint g_windowHeight = 600;

glm::vec3 g_translation;

bool g_wireFrame = false;		// wireframe on or off
bool g_bDepthTest = true;		// depth test on or off

static void init(GLFWwindow* window)
{
	glEnable(GL_DEPTH_TEST);	// enable depth buffer test

	// create and compile our GLSL program from the shader files
	g_shaderProgramID = loadShaders("PerFragLightingVS.vert", "PerFragLightingFS.frag");

	// find the location of shader variables
	GLuint positionIndex = glGetAttribLocation(g_shaderProgramID, "aPosition");
	GLuint normalIndex = glGetAttribLocation(g_shaderProgramID, "aNormal");
	g_MVP_Index = glGetUniformLocation(g_shaderProgramID, "uModelViewProjectionMatrix");
	g_MV_Index = glGetUniformLocation(g_shaderProgramID, "uModelViewMatrix");
	g_V_Index = glGetUniformLocation(g_shaderProgramID, "uViewMatrix");

	g_lightPositionIndex = glGetUniformLocation(g_shaderProgramID, "uLight.position");
	g_lightAmbientIndex = glGetUniformLocation(g_shaderProgramID, "uLight.ambient");
	g_lightDiffuseIndex = glGetUniformLocation(g_shaderProgramID, "uLight.diffuse");
	g_lightSpecularIndex = glGetUniformLocation(g_shaderProgramID, "uLight.specular");

	g_materialAmbientIndex = glGetUniformLocation(g_shaderProgramID, "uMaterial.ambient");
	g_materialDiffuseIndex = glGetUniformLocation(g_shaderProgramID, "uMaterial.diffuse");
	g_materialSpecularIndex = glGetUniformLocation(g_shaderProgramID, "uMaterial.specular");
	g_materialShininessIndex = glGetUniformLocation(g_shaderProgramID, "uMaterial.shininess");

	// initialise model matrix to the identity matrix
	g_modelMatrix[0] = glm::mat4(1.0f);
	g_modelMatrix[1] = glm::translate(vec3(0.0f, 0.0f, -2.0f)) * glm::scale(glm::vec3(2.0f, 2.0f, 2.0f));
	g_modelMatrix[2] = glm::mat4(1.0f);

	// initialise view matrix
	g_viewMatrix = glm::lookAt(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, 4.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	float aspectRatio = static_cast<float>(width) / height;

	// initialise projection matrix
	g_projectionMatrix = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f);

	// initialise point light properties
	g_light.position = glm::vec3(5.0f, 5.0f, 5.0f);
	g_light.ambient = glm::vec3(1.0f, 1.0f, 1.0f);
	g_light.diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	g_light.specular = glm::vec3(1.0f, 1.0f, 1.0f);

	// initialise material properties
	g_material[0].ambient = glm::vec3(0.3f, 0.3f, 0.3f);
	g_material[0].diffuse = glm::vec3(0.8f, 0.8f, 0.4f);
	g_material[0].specular = glm::vec3(0.8f, 0.8f, 0.4f);
	g_material[0].shininess = 40.0f;

	g_material[1].ambient = glm::vec3(0.3f, 0.3f, 0.3f);
	g_material[1].diffuse = glm::vec3(0.2f, 0.7f, 1.0f);
	g_material[1].specular = glm::vec3(0.2f, 0.7f, 1.0f);
	g_material[1].shininess = 40.0f;

	g_material[2].ambient = glm::vec3(0.3f, 0.3f, 0.3f);
	g_material[2].diffuse = glm::vec3(0.2f, 0.7f, 0.2f);
	g_material[2].specular = glm::vec3(0.2f, 0.7f, 0.2f);
	g_material[2].shininess = 40.0f;

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
	glVertexAttribPointer(normalIndex, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, normal)));

	glEnableVertexAttribArray(positionIndex);	// enable vertex attributes
	glEnableVertexAttribArray(normalIndex);
}

// function used to update the scene
static void update_scene()
{
	// update model matrix
	g_modelMatrix[2] = glm::translate(g_translation);
}

// function used to render the scene
static void render_scene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// clear colour buffer and depth buffer

	glUseProgram(g_shaderProgramID);	// use the shaders associated with the shader program

	glBindVertexArray(g_VAO);		// make VAO active

	// set uniform shader variables
	glm::mat4 MVP = g_projectionMatrix * g_viewMatrix * g_modelMatrix[1];
	glm::mat4 MV = g_viewMatrix * g_modelMatrix[1];
	glUniformMatrix4fv(g_MVP_Index, 1, GL_FALSE, &MVP[0][0]);
	glUniformMatrix4fv(g_MV_Index, 1, GL_FALSE, &MV[0][0]);
	glUniformMatrix4fv(g_V_Index, 1, GL_FALSE, &g_viewMatrix[0][0]);

	glUniform3fv(g_lightPositionIndex, 1, &g_light.position[0]);
	glUniform3fv(g_lightAmbientIndex, 1, &g_light.ambient[0]);
	glUniform3fv(g_lightDiffuseIndex, 1, &g_light.diffuse[0]);
	glUniform3fv(g_lightSpecularIndex, 1, &g_light.specular[0]);

	glUniform3fv(g_materialAmbientIndex, 1, &g_material[1].ambient[0]);
	glUniform3fv(g_materialDiffuseIndex, 1, &g_material[1].diffuse[0]);
	glUniform3fv(g_materialSpecularIndex, 1, &g_material[1].specular[0]);
	glUniform1fv(g_materialShininessIndex, 1, &g_material[1].shininess);

	// draw cube
	glDrawArrays(GL_TRIANGLES, 6, 36);

	MVP = g_projectionMatrix * g_viewMatrix * g_modelMatrix[2];
	MV = g_viewMatrix * g_modelMatrix[2];
	glUniformMatrix4fv(g_MVP_Index, 1, GL_FALSE, &MVP[0][0]);
	glUniformMatrix4fv(g_MV_Index, 1, GL_FALSE, &MV[0][0]);

	glUniform3fv(g_materialAmbientIndex, 1, &g_material[2].ambient[0]);
	glUniform3fv(g_materialDiffuseIndex, 1, &g_material[2].diffuse[0]);
	glUniform3fv(g_materialSpecularIndex, 1, &g_material[2].specular[0]);
	glUniform1fv(g_materialShininessIndex, 1, &g_material[2].shininess);

	// draw cube
	glDrawArrays(GL_TRIANGLES, 6, 36);

	MVP = g_projectionMatrix * g_viewMatrix * g_modelMatrix[0];
	MV = g_viewMatrix * g_modelMatrix[0];
	glUniformMatrix4fv(g_MVP_Index, 1, GL_FALSE, &MVP[0][0]);
	glUniformMatrix4fv(g_MV_Index, 1, GL_FALSE, &MV[0][0]);

	glUniform3fv(g_materialAmbientIndex, 1, &g_material[0].ambient[0]);
	glUniform3fv(g_materialDiffuseIndex, 1, &g_material[0].diffuse[0]);
	glUniform3fv(g_materialSpecularIndex, 1, &g_material[0].specular[0]);
	glUniform1fv(g_materialShininessIndex, 1, &g_material[0].shininess);

	// draw plane
	glDrawArrays(GL_TRIANGLES, 0, 6);

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
	TwDefine(" Main label='Controls' refresh=0.02 text=light size='220 220' ");

	// create display entries
	TwAddVarRW(TweakBar, "Wireframe", TW_TYPE_BOOLCPP, &g_wireFrame, " group='Display' ");

	// display a separator
	TwAddSeparator(TweakBar, NULL, NULL);

	// depth test entries
	TwAddVarRW(TweakBar, "DepthTest", TW_TYPE_BOOLCPP, &g_bDepthTest, " group='Depth Buffer' ");

	// create transformation entries
	TwAddVarRW(TweakBar, "Translate x", TW_TYPE_FLOAT, &g_translation[0], " group='Transformation' min=-2.0 max=2.0 step=0.1 ");
	TwAddVarRW(TweakBar, "Translate y", TW_TYPE_FLOAT, &g_translation[1], " group='Transformation' min=-2.0 max=2.0 step=0.1 ");
	TwAddVarRW(TweakBar, "Translate z", TW_TYPE_FLOAT, &g_translation[2], " group='Transformation' min=-5.0 max=5.0 step=0.1 ");

	// initialise rendering states
	init(window);

	// the rendering loop
	while (!glfwWindowShouldClose(window))
	{
		update_scene();		// update the scene

		if (g_wireFrame)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		// which face to cull
		if (g_bDepthTest)
			glEnable(GL_DEPTH_TEST);	// enable depth buffer test
		else
			glDisable(GL_DEPTH_TEST);	// enable depth buffer test

		render_scene();		// render the scene

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

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
