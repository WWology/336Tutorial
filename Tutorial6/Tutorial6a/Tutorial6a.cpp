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

#include "bmpfuncs.h"
#include "shader.h"

// struct for vertex attributes
typedef struct Vertex
{
	GLfloat position[3];
	GLfloat normal[3];
	GLfloat texCoord[2];
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
	// Front: triangle 1
	// vertex 1
	-0.5f, 0.5f, 0.5f, // position
	0.0f, 0.0f, 1.0f,  // normal
	0.0f, 1.0f,		   // texture coordinate
	// vertex 2
	-0.5f, -0.5f, 0.5f, // position
	0.0f, 0.0f, 1.0f,	// normal
	0.0f, 0.0f,			// texture coordinate
	// vertex 3
	0.5f, 0.5f, 0.5f, // position
	0.0f, 0.0f, 1.0f, // normal
	1.0f, 1.0f,		  // texture coordinate

	// triangle 2
	// vertex 1
	0.5f, 0.5f, 0.5f, // position
	0.0f, 0.0f, 1.0f, // normal
	1.0f, 1.0f,		  // texture coordinate
	// vertex 2
	-0.5f, -0.5f, 0.5f, // position
	0.0f, 0.0f, 1.0f,	// normal
	0.0f, 0.0f,			// texture coordinate
	// vertex 3
	0.5f, -0.5f, 0.5f, // position
	0.0f, 0.0f, 1.0f,  // normal
	1.0f, 0.0f,		   // texture coordinate

	// Right: triangle 3
	// vertex 1
	0.5f, 0.5f, 0.5f, // position
	1.0f, 0.0f, 0.0f, // normal
	0.0f, 1.0f,		  // texture coordinate
	// vertex 2
	0.5f, -0.5f, 0.5f, // position
	1.0f, 0.0f, 0.0f,  // normal
	0.0f, 0.0f,		   // texture coordinate
	// vertex 3
	0.5f, 0.5f, -0.5f, // position
	1.0f, 0.0f, 0.0f,  // normal
	1.0f, 1.0f,		   // texture coordinate

	// triangle 4
	// vertex 1
	0.5f, 0.5f, -0.5f, // position
	1.0f, 0.0f, 0.0f,  // normal
	1.0f, 1.0f,		   // texture coordinate
	// vertex 2
	0.5f, -0.5f, 0.5f, // position
	1.0f, 0.0f, 0.0f,  // normal
	0.0f, 0.0f,		   // texture coordinate
	// vertex 3
	0.5f, -0.5f, -0.5f, // position
	1.0f, 0.0f, 0.0f,	// normal
	1.0f, 0.0f,			// texture coordinate

	// Back: triangle 5
	// vertex 1
	0.5f, -0.5f, -0.5f, // position
	0.0f, 0.0f, -1.0f,	// normal
	0.0f, 0.0f,			// texture coordinate
	// vertex 2
	-0.5f, -0.5f, -0.5f, // position
	0.0f, 0.0f, -1.0f,	 // normal
	1.0f, 0.0f,			 // texture coordinate
	// vertex 3
	0.5f, 0.5f, -0.5f, // position
	0.0f, 0.0f, -1.0f, // normal
	0.0f, 1.0f,		   // texture coordinate

	// triangle 6
	// vertex 1
	0.5f, 0.5f, -0.5f, // position
	0.0f, 0.0f, -1.0f, // normal
	0.0f, 1.0f,		   // texture coordinate
	// vertex 2
	-0.5f, -0.5f, -0.5f, // position
	0.0f, 0.0f, -1.0f,	 // normal
	1.0f, 0.0f,			 // texture coordinate
	// vertex 3
	-0.5f, 0.5f, -0.5f, // position
	0.0f, 0.0f, -1.0f,	// normal
	1.0f, 1.0f,			// texture coordinate

	// Left: triangle 7
	// vertex 1
	-0.5f, -0.5f, 0.5f, // position
	-1.0f, 0.0f, 0.0f,	// normal
	1.0f, 0.0f,			// texture coordinate
	// vertex 2
	-0.5f, 0.5f, 0.5f, // position
	-1.0f, 0.0f, 0.0f, // normal
	1.0f, 1.0f,		   // texture coordinate
	// vertex 3
	-0.5f, -0.5f, -0.5f, // position
	-1.0f, 0.0f, 0.0f,	 // normal
	0.0f, 0.0f,			 // texture coordinate

	// triangle 8
	// vertex 1
	-0.5f, -0.5f, -0.5f, // position
	-1.0f, 0.0f, 0.0f,	 // normal
	0.0f, 0.0f,			 // texture coordinate
	// vertex 2
	-0.5f, 0.5f, 0.5f, // position
	-1.0f, 0.0f, 0.0f, // normal
	1.0f, 1.0f,		   // texture coordinate
	// vertex 3
	-0.5f, 0.5f, -0.5f, // position
	-1.0f, 0.0f, 0.0f,	// normal
	0.0f, 1.0f,			// texture coordinate

	// Top: triangle 9
	// vertex 1
	-0.5f, 0.5f, 0.5f, // position
	0.0f, 1.0f, 0.0f,  // normal
	0.0f, 0.0f,		   // texture coordinate
	// vertex 2
	0.5f, 0.5f, 0.5f, // position
	0.0f, 1.0f, 0.0f, // normal
	1.0f, 0.0f,		  // texture coordinate
	// vertex 3
	-0.5f, 0.5f, -0.5f, // position
	0.0f, 1.0f, 0.0f,	// normal
	0.0f, 1.0f,			// texture coordinate

	// triangle 10
	// vertex 1
	-0.5f, 0.5f, -0.5f, // position
	0.0f, 1.0f, 0.0f,	// normal
	0.0f, 1.0f,			// texture coordinate
	// vertex 2
	0.5f, 0.5f, 0.5f, // position
	0.0f, 1.0f, 0.0f, // normal
	1.0f, 0.0f,		  // texture coordinate
	// vertex 3
	0.5f, 0.5f, -0.5f, // position
	0.0f, 1.0f, 0.0f,  // normal
	1.0f, 1.0f,		   // texture coordinate

	// Bottom: triangle 11
	// vertex 1
	0.5f, -0.5f, 0.5f, // position
	0.0f, -1.0f, 0.0f, // normal
	1.0f, 1.0f,		   // texture coordinate
	// vertex 2
	-0.5f, -0.5f, 0.5f, // position
	0.0f, -1.0f, 0.0f,	// normal
	0.0f, 1.0f,			// texture coordinate
	// vertex 3
	0.5f, -0.5f, -0.5f, // position
	0.0f, -1.0f, 0.0f,	// normal
	1.0f, 0.0f,			// texture coordinate

	// triangle 12
	// vertex 1
	0.5f, -0.5f, -0.5f, // position
	0.0f, -1.0f, 0.0f,	// normal
	1.0f, 0.0f,			// texture coordinate
	// vertex 2
	-0.5f, -0.5f, 0.5f, // position
	0.0f, -1.0f, 0.0f,	// normal
	0.0f, 1.0f,			// texture coordinate
	// vertex 3
	-0.5f, -0.5f, -0.5f, // position
	0.0f, -1.0f, 0.0f,	 // normal
	0.0f, 0.0f,			 // texture coordinate
};

GLuint g_VBO = 0;			  // vertex buffer object identifier
GLuint g_VAO = 0;			  // vertex array object identifier
GLuint g_shaderProgramID = 0; // shader program identifier

// locations in shader
GLuint g_MVP_Index = 0;
GLuint g_MV_Index = 0;
GLuint g_V_Index = 0;
GLuint g_texSampler_Index = 0;
GLuint g_lightPositionIndex = 0;
GLuint g_lightAmbientIndex = 0;
GLuint g_lightDiffuseIndex = 0;
GLuint g_lightSpecularIndex = 0;
GLuint g_materialAmbientIndex = 0;
GLuint g_materialDiffuseIndex = 0;
GLuint g_materialSpecularIndex = 0;
GLuint g_materialShininessIndex = 0;

glm::mat4 g_modelMatrix;	  // object's model matrix
glm::mat4 g_viewMatrix;		  // view matrix
glm::mat4 g_projectionMatrix; // projection matrix

Light g_light;		 // light properties
Material g_material; // material properties

unsigned char* g_texImage = NULL; //image data
GLuint g_textureID;				  //texture id

GLuint g_windowWidth = 800; // window dimensions
GLuint g_windowHeight = 600;

static void init(GLFWwindow* window)
{
	glEnable(GL_DEPTH_TEST); // enable depth buffer test

	// create and compile our GLSL program from the shader files
	g_shaderProgramID = loadShaders("LightAndTextureVS.vert", "LightAndTextureFS.frag");

	// find the location of shader variables
	GLuint positionIndex = glGetAttribLocation(g_shaderProgramID, "aPosition");
	GLuint normalIndex = glGetAttribLocation(g_shaderProgramID, "aNormal");
	GLuint texCoordIndex = glGetAttribLocation(g_shaderProgramID, "aTexCoord");
	g_MVP_Index = glGetUniformLocation(g_shaderProgramID, "uModelViewProjectionMatrix");
	g_MV_Index = glGetUniformLocation(g_shaderProgramID, "uModelViewMatrix");
	g_V_Index = glGetUniformLocation(g_shaderProgramID, "uViewMatrix");

	g_texSampler_Index = glGetUniformLocation(g_shaderProgramID, "uTextureSampler");

	g_lightPositionIndex = glGetUniformLocation(g_shaderProgramID, "uLight.position");
	g_lightAmbientIndex = glGetUniformLocation(g_shaderProgramID, "uLight.ambient");
	g_lightDiffuseIndex = glGetUniformLocation(g_shaderProgramID, "uLight.diffuse");
	g_lightSpecularIndex = glGetUniformLocation(g_shaderProgramID, "uLight.specular");

	g_materialAmbientIndex = glGetUniformLocation(g_shaderProgramID, "uMaterial.ambient");
	g_materialDiffuseIndex = glGetUniformLocation(g_shaderProgramID, "uMaterial.diffuse");
	g_materialSpecularIndex = glGetUniformLocation(g_shaderProgramID, "uMaterial.specular");
	g_materialShininessIndex = glGetUniformLocation(g_shaderProgramID, "uMaterial.shininess");

	// initialise model matrix to the identity matrix
	g_modelMatrix = glm::mat4(1.0f);

	// initialise view matrix
	g_viewMatrix = glm::lookAt(glm::vec3(0.0f, 0.0f, 4.0f), glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	float aspectRatio = static_cast<float>(width) / height;

	// initialise projection matrix
	g_projectionMatrix = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f);

	// initialise point light properties
	g_light.position = glm::vec3(3.0f, 3.0f, 3.0f);
	g_light.ambient = glm::vec3(1.0f, 1.0f, 1.0f);
	g_light.diffuse = glm::vec3(0.0f, 0.5f, 1.0f);
	g_light.specular = glm::vec3(0.0f, 0.5f, 1.0f);

	// initialise material properties
	g_material.ambient = glm::vec3(0.4f, 0.4f, 0.4f);
	g_material.diffuse = glm::vec3(0.2f, 0.7f, 1.0f);
	g_material.specular = glm::vec3(0.2f, 0.7f, 1.0f);
	g_material.shininess = 40.0f;

	// read the image data
	GLint imageWidth;  //image width info
	GLint imageHeight; //image height info
	g_texImage = readBitmapRGBImage("images/check.bmp", &imageWidth, &imageHeight);

	// generate identifier for texture object and set texture properties
	glGenTextures(1, &g_textureID);
	glBindTexture(GL_TEXTURE_2D, g_textureID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_BGR, GL_UNSIGNED_BYTE, g_texImage);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
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
	glVertexAttribPointer(texCoordIndex, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, texCoord)));

	glEnableVertexAttribArray(positionIndex); // enable vertex attributes
	glEnableVertexAttribArray(normalIndex);
	glEnableVertexAttribArray(texCoordIndex);
}

// function used to update the scene
static void update_scene(float frameTime)
{
	static float rotateAngle = 0.0f;

	rotateAngle += 50.0f * frameTime;

	// update model matrix
	g_modelMatrix = glm::rotate(glm::radians(rotateAngle), glm::vec3(1.0f, 1.0f, 0.0f));
}

// function used to render the scene
static void render_scene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear colour buffer and depth buffer

	glUseProgram(g_shaderProgramID); // use the shaders associated with the shader program

	glBindVertexArray(g_VAO); // make VAO active

	// set uniform shader variables
	glm::mat4 MVP = g_projectionMatrix * g_viewMatrix * g_modelMatrix;
	glm::mat4 MV = g_viewMatrix * g_modelMatrix;
	glUniformMatrix4fv(g_MVP_Index, 1, GL_FALSE, &MVP[0][0]);
	glUniformMatrix4fv(g_MV_Index, 1, GL_FALSE, &MV[0][0]);
	glUniformMatrix4fv(g_V_Index, 1, GL_FALSE, &g_viewMatrix[0][0]);

	glUniform3fv(g_lightPositionIndex, 1, &g_light.position[0]);
	glUniform3fv(g_lightAmbientIndex, 1, &g_light.ambient[0]);
	glUniform3fv(g_lightDiffuseIndex, 1, &g_light.diffuse[0]);
	glUniform3fv(g_lightSpecularIndex, 1, &g_light.specular[0]);

	glUniform3fv(g_materialAmbientIndex, 1, &g_material.ambient[0]);
	glUniform3fv(g_materialDiffuseIndex, 1, &g_material.diffuse[0]);
	glUniform3fv(g_materialSpecularIndex, 1, &g_material.specular[0]);
	glUniform1fv(g_materialShininessIndex, 1, &g_material.shininess);

	glUniform1i(g_texSampler_Index, 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, g_textureID);

	glDrawArrays(GL_TRIANGLES, 0, 36);

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
	GLFWwindow* window = NULL;			   // pointer to a GLFW window handle
	double lastUpdateTime = glfwGetTime(); // last update time
	double elapsedTime = lastUpdateTime;   // time elapsed since last update
	double frameTime = 0.0f;			   // frame time
	int frameCount = 0;					   // number of frames since last update
	int FPS = 0;						   // frames per second

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

	// initialise rendering states
	init(window);

	// the rendering loop
	while (!glfwWindowShouldClose(window))
	{
		update_scene(frameTime); // update the scene
		render_scene();			 // render the scene

		glfwSwapBuffers(window); // swap buffers
		glfwPollEvents();		 // poll for events

		frameCount++;
		elapsedTime = glfwGetTime() - lastUpdateTime; // current time - last update time

		if (elapsedTime >= 1.0f) // if time since last update >= to 1 second
		{
			frameTime = 1.0f / frameCount; // calculate frame time

			std::string str = "FPS = " + std::to_string(frameCount) + "; FT = " + std::to_string(frameTime);

			glfwSetWindowTitle(window, str.c_str()); // update window title

			FPS = frameCount;
			frameCount = 0;				   // reset frame count
			lastUpdateTime += elapsedTime; // update last update time
		}
	}

	// clean up
	if (g_texImage)
		delete[] g_texImage;

	glDeleteProgram(g_shaderProgramID);
	glDeleteBuffers(1, &g_VBO);
	glDeleteVertexArrays(1, &g_VAO);
	glDeleteTextures(1, &g_textureID);

	// close the window and terminate GLFW
	glfwDestroyWindow(window);
	glfwTerminate();

	exit(EXIT_SUCCESS);
}
