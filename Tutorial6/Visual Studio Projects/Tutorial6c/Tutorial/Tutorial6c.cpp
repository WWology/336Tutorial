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
#include "bmpfuncs.h"
#include "Camera.h"

#define MOVEMENT_SENSITIVITY 3.0f		// camera movement sensitivity
#define ROTATION_SENSITIVITY 0.3f		// camera rotation sensitivity

// struct for vertex attributes
typedef struct Vertex
{
	GLfloat position[3];
	GLfloat normal[3];
	GLfloat tangent[3];
	GLfloat texCoord[2];
} Vertex;

// light and material structs
typedef struct Light
{
	glm::vec3 position;
	glm::vec3 direction;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	int type;
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
	// Front: triangle 1
	// vertex 1
	-1.0f, 1.0f, 0.0f,	// position
	0.0f, 0.0f, 1.0f,	// normal
	1.0f, 0.0f, 0.0f,	// tangent
	0.0f, 1.0f,			// texture coordinate
	// vertex 2
	-1.0f, -1.0f, 0.0f,	// position
	0.0f, 0.0f, 1.0f,	// normal
	1.0f, 0.0f, 0.0f,	// tangent
	0.0f, 0.0f,			// texture coordinate
	// vertex 3
	1.0f, 1.0f, 0.0f,	// position
	0.0f, 0.0f, 1.0f,	// normal
	1.0f, 0.0f, 0.0f,	// tangent
	1.0f, 1.0f,			// texture coordinate

	// triangle 2
	// vertex 1
	1.0f, 1.0f, 0.0f,	// position
	0.0f, 0.0f, 1.0f,	// normal
	1.0f, 0.0f, 0.0f,	// tangent
	1.0f, 1.0f,			// texture coordinate
	// vertex 2
	-1.0f, -1.0f, 0.0f,	// position
	0.0f, 0.0f, 1.0f,	// normal
	1.0f, 0.0f, 0.0f,	// tangent
	0.0f, 0.0f,			// texture coordinate
	// vertex 3
	1.0f, -1.0f, 0.0f,	// position
	0.0f, 0.0f, 1.0f,	// normal
	1.0f, 0.0f, 0.0f,	// tangent
	1.0f, 0.0f,			// texture coordinate
};

GLuint g_VBO = 0;				// vertex buffer object identifier
GLuint g_VAO = 0;				// vertex array object identifier
GLuint g_shaderProgramID = 0;	// shader program identifier

// locations in shader
GLuint g_MVP_Index = 0;
GLuint g_MV_Index = 0;
GLuint g_V_Index = 0;
GLuint g_texSamplerIndex;
GLuint g_normalSamplerIndex;
GLuint g_lightPositionIndex = 0;
GLuint g_lightDirectionIndex = 0;
GLuint g_lightAmbientIndex = 0;
GLuint g_lightDiffuseIndex = 0;
GLuint g_lightSpecularIndex = 0;
GLuint g_lightTypeIndex = 0;
GLuint g_materialAmbientIndex = 0;
GLuint g_materialDiffuseIndex = 0;
GLuint g_materialSpecularIndex = 0;
GLuint g_materialShininessIndex = 0;

glm::mat4 g_modelMatrix;		// object's model matrix

Light g_lightPoint;				// light properties
Light g_lightDirectional;		// light properties
Material g_material;			// material properties
bool g_directional = false;		// directional light source on or off

unsigned char* g_texImage[2];	//image data
GLuint g_textureID[2];			//texture id

GLuint g_windowWidth = 800;		// window dimensions
GLuint g_windowHeight = 600;

float g_frameTime = 0.0f;
Camera g_camera;
bool g_moveCamera = false;
bool g_centreCursor = false;

static void init(GLFWwindow* window)
{
	glEnable(GL_DEPTH_TEST);	// enable depth buffer test

	// create and compile our GLSL program from the shader files
	g_shaderProgramID = loadShaders("NormalMapVS.vert", "NormalMapFS.frag");

	// find the location of shader variables
	GLuint positionIndex = glGetAttribLocation(g_shaderProgramID, "aPosition");
	GLuint normalIndex = glGetAttribLocation(g_shaderProgramID, "aNormal");
	GLuint tangentIndex = glGetAttribLocation(g_shaderProgramID, "aTangent");
	GLuint texCoordIndex = glGetAttribLocation(g_shaderProgramID, "aTexCoord");

	g_MVP_Index = glGetUniformLocation(g_shaderProgramID, "uModelViewProjectionMatrix");
	g_MV_Index = glGetUniformLocation(g_shaderProgramID, "uModelViewMatrix");
	g_V_Index = glGetUniformLocation(g_shaderProgramID, "uViewMatrix");

	g_texSamplerIndex = glGetUniformLocation(g_shaderProgramID, "uTextureSampler");
	g_normalSamplerIndex = glGetUniformLocation(g_shaderProgramID, "uNormalSampler");

	g_lightPositionIndex = glGetUniformLocation(g_shaderProgramID, "uLight.position");
	g_lightDirectionIndex = glGetUniformLocation(g_shaderProgramID, "uLight.direction");
	g_lightAmbientIndex = glGetUniformLocation(g_shaderProgramID, "uLight.ambient");
	g_lightDiffuseIndex = glGetUniformLocation(g_shaderProgramID, "uLight.diffuse");
	g_lightSpecularIndex = glGetUniformLocation(g_shaderProgramID, "uLight.specular");
	g_lightTypeIndex = glGetUniformLocation(g_shaderProgramID, "uLight.type");

	g_materialAmbientIndex = glGetUniformLocation(g_shaderProgramID, "uMaterial.ambient");
	g_materialDiffuseIndex = glGetUniformLocation(g_shaderProgramID, "uMaterial.diffuse");
	g_materialSpecularIndex = glGetUniformLocation(g_shaderProgramID, "uMaterial.specular");
	g_materialShininessIndex = glGetUniformLocation(g_shaderProgramID, "uMaterial.shininess");

	// initialise model matrix to the identity matrix
	g_modelMatrix = glm::mat4(1.0f);

	// initialise view matrix
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	float aspectRatio = static_cast<float>(width) / height;

	g_camera.setViewMatrix(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	g_camera.setProjection(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f);

	// initialise point light properties
	g_lightPoint.position = glm::vec3(1.0f, 1.0f, 1.0f);
	g_lightPoint.ambient = glm::vec3(1.0f, 1.0f, 1.0f);
	g_lightPoint.diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	g_lightPoint.specular = glm::vec3(1.0f, 1.0f, 1.0f);
	g_lightPoint.type = 0;

	// initialise directional light properties
	g_lightDirectional.direction = glm::vec3(-1.0f, -1.0f, -1.0f);
	g_lightDirectional.ambient = glm::vec3(1.0f, 1.0f, 1.0f);
	g_lightDirectional.diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	g_lightDirectional.specular = glm::vec3(1.0f, 1.0f, 1.0f);
	g_lightDirectional.type = 1;

	// initialise material properties
	g_material.ambient = glm::vec3(0.3f, 0.3f, 0.3f);
	g_material.diffuse = glm::vec3(0.2f, 0.7f, 1.0f);
	g_material.specular = glm::vec3(0.2f, 0.7f, 1.0f);
	g_material.shininess = 40.0f;

	// read the image data
	GLint imageWidth[2];			//image width info
	GLint imageHeight[2];			//image height info
	g_texImage[0] = readBitmapRGBImage("images/Fieldstone.bmp", &imageWidth[0], &imageHeight[0]);
	g_texImage[1] = readBitmapRGBImage("images/FieldstoneBumpDOT3.bmp", &imageWidth[1], &imageHeight[1]);

	// generate identifier for texture object and set texture properties
	glGenTextures(2, g_textureID);
	glBindTexture(GL_TEXTURE_2D, g_textureID[0]);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth[0], imageHeight[0], 0, GL_BGR, GL_UNSIGNED_BYTE, g_texImage[0]);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindTexture(GL_TEXTURE_2D, g_textureID[1]);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth[1], imageHeight[1], 0, GL_BGR, GL_UNSIGNED_BYTE, g_texImage[1]);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

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
	glVertexAttribPointer(tangentIndex, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, tangent)));
	glVertexAttribPointer(texCoordIndex, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, texCoord)));

	glEnableVertexAttribArray(positionIndex);	// enable vertex attributes
	glEnableVertexAttribArray(normalIndex);
	glEnableVertexAttribArray(tangentIndex);
	glEnableVertexAttribArray(texCoordIndex);
}

// function used to update the scene
static void update_scene(GLFWwindow* window, float frameTime)
{
	// variables to store forward/back and strafe movement
	float moveForward = 0;
	float strafeRight = 0;

	// update movement variables based on keyboard input
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		moveForward += 1 * MOVEMENT_SENSITIVITY * frameTime;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		moveForward -= 1 * MOVEMENT_SENSITIVITY * frameTime;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		strafeRight -= 1 * MOVEMENT_SENSITIVITY * frameTime;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		strafeRight += 1 * MOVEMENT_SENSITIVITY * frameTime;

	g_camera.update(moveForward, strafeRight);	// update camera
}

// function used to render the scene
static void render_scene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// clear colour buffer and depth buffer

	glUseProgram(g_shaderProgramID);	// use the shaders associated with the shader program

	glBindVertexArray(g_VAO);		// make VAO active

	// set uniform shader variables
	glm::mat4 MVP = g_camera.getProjectionMatrix() * g_camera.getViewMatrix() * g_modelMatrix;
	glm::mat4 MV = g_camera.getViewMatrix() * g_modelMatrix;
	glm::mat4 V = g_camera.getViewMatrix();
	glUniformMatrix4fv(g_MVP_Index, 1, GL_FALSE, &MVP[0][0]);
	glUniformMatrix4fv(g_MV_Index, 1, GL_FALSE, &MV[0][0]);
	glUniformMatrix4fv(g_V_Index, 1, GL_FALSE, &V[0][0]);

	if (g_directional)
	{
		glUniform3fv(g_lightDirectionIndex, 1, &g_lightDirectional.direction[0]);
		glUniform3fv(g_lightAmbientIndex, 1, &g_lightDirectional.ambient[0]);
		glUniform3fv(g_lightDiffuseIndex, 1, &g_lightDirectional.diffuse[0]);
		glUniform3fv(g_lightSpecularIndex, 1, &g_lightDirectional.specular[0]);
		glUniform1i(g_lightTypeIndex, g_lightDirectional.type);
	}
	else
	{
		glUniform3fv(g_lightPositionIndex, 1, &g_lightPoint.position[0]);
		glUniform3fv(g_lightAmbientIndex, 1, &g_lightPoint.ambient[0]);
		glUniform3fv(g_lightDiffuseIndex, 1, &g_lightPoint.diffuse[0]);
		glUniform3fv(g_lightSpecularIndex, 1, &g_lightPoint.specular[0]);
		glUniform1i(g_lightTypeIndex, g_lightPoint.type);
	}

	glUniform3fv(g_materialAmbientIndex, 1, &g_material.ambient[0]);
	glUniform3fv(g_materialDiffuseIndex, 1, &g_material.diffuse[0]);
	glUniform3fv(g_materialSpecularIndex, 1, &g_material.specular[0]);
	glUniform1fv(g_materialShininessIndex, 1, &g_material.shininess);

	glUniform1i(g_texSamplerIndex, 0);
	glUniform1i(g_normalSamplerIndex, 1);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, g_textureID[0]);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, g_textureID[1]);

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
	// variables to store mouse cursor coordinates
	static double previous_xpos = xpos;
	static double previous_ypos = ypos;
	double delta_x = previous_xpos - xpos;
	double delta_y = previous_ypos - ypos;

	if (g_moveCamera)
	{
		if (!g_centreCursor) // ignore camera update the first time mouse cursor is centred
		{
			// pass mouse movement to camera class to update its yaw and pitch
			g_camera.updateRotation(delta_x * ROTATION_SENSITIVITY * g_frameTime, delta_y * ROTATION_SENSITIVITY * g_frameTime);
		}
		else
		{
			g_centreCursor = false;
		}
	}

	// update previous mouse coordinates
	previous_xpos = xpos;
	previous_ypos = ypos;

	// pass mouse data to tweak bar
	TwEventMousePosGLFW(xpos, ypos);
}

static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	// pass mouse data to tweak bar
	TwEventMouseButtonGLFW(button, action);

	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
	{
		// use mouse to move camera, hence use disable cursor mode
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		g_centreCursor = true;	// mouse cursor position is centred
		g_moveCamera = true;
	}
	else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
	{
		// use mouse to move camera, hence use disable cursor mode
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

		g_moveCamera = false;
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
	TwBar *TweakBar;			// pointer to a tweak bar
	double lastUpdateTime = glfwGetTime();	// last update time
	double elapsedTime = lastUpdateTime;	// time elapsed since last update
	int frameCount = 0;						// number of frames since last update
	int FPS = 0;							// frames per second

	glfwSetErrorCallback(error_callback);	// set error callback function

	// initialise GLFW
	if (!glfwInit())
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

	// create transformation entries
	TwAddVarRW(TweakBar, "Directional", TW_TYPE_BOOLCPP, &g_directional, " group='Light Source' ");
	// create light position
	TwAddVarRW(TweakBar, "LightPos: x", TW_TYPE_FLOAT, &g_lightPoint.position[0], " group='Light Position' min=-10.0 max=10.0 step=0.1");
	TwAddVarRW(TweakBar, "LightPos: y", TW_TYPE_FLOAT, &g_lightPoint.position[1], " group='Light Position' min=-10.0 max=10.0 step=0.1");
	TwAddVarRW(TweakBar, "LightPos: z", TW_TYPE_FLOAT, &g_lightPoint.position[2], " group='Light Position' min=-10.0 max=10.0 step=0.1");

	TwAddVarRW(TweakBar, "LightDir: x", TW_TYPE_FLOAT, &g_lightDirectional.direction[0], " group='Light Direction' min=-1.0 max=1.0 step=0.1");
	TwAddVarRW(TweakBar, "LightDir: y", TW_TYPE_FLOAT, &g_lightDirectional.direction[1], " group='Light Direction' min=-1.0 max=1.0 step=0.1");
	TwAddVarRW(TweakBar, "LightDir: z", TW_TYPE_FLOAT, &g_lightDirectional.direction[2], " group='Light Direction' min=-1.0 max=1.0 step=0.1");

	// initialise rendering states
	init(window);

	// the rendering loop
	while (!glfwWindowShouldClose(window))
	{
		update_scene(window, g_frameTime);		// update the scene
		render_scene();		// render the scene

		TwDraw();			// draw tweak bar(s)

		glfwSwapBuffers(window);	// swap buffers
		glfwPollEvents();			// poll for events

		frameCount++;
		elapsedTime = glfwGetTime() - lastUpdateTime;	// current time - last update time

		if (elapsedTime >= 1.0f)	// if time since last update >= to 1 second
		{
			g_frameTime = 1.0f / frameCount;	// calculate frame time

			string str = "FPS = " + to_string(frameCount) + "; FT = " + to_string(g_frameTime);

			glfwSetWindowTitle(window, str.c_str());	// update window title

			FPS = frameCount;
			frameCount = 0;					// reset frame count
			lastUpdateTime += elapsedTime;	// update last update time
		}
	}

	// clean up
	if (g_texImage[0])
		delete[] g_texImage[0];
	if (g_texImage[1])
		delete[] g_texImage[1];

	glDeleteProgram(g_shaderProgramID);
	glDeleteBuffers(1, &g_VBO);
	glDeleteVertexArrays(1, &g_VAO);
	glDeleteTextures(2, g_textureID);

	// uninitialise tweak bar
	TwTerminate();

	// close the window and terminate GLFW
	glfwDestroyWindow(window);
	glfwTerminate();

	exit(EXIT_SUCCESS);
}

