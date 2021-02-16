// This is a personal academic project. Dear PVS-Studio, please check it

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
#include <AntTweakBar.h>

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
	// plane
	-5.0f, -1.0f, 5.0f,	 // position
	0.0f, 1.0f, 0.0f,	 // normal
	0.0f, 0.0f,			 // texture coordinate
	5.0f, -1.0f, 5.0f,	 // position
	0.0f, 1.0f, 0.0f,	 // normal
	5.0f, 0.0f,			 // texture coordinate
	-5.0f, -1.0f, -5.0f, // position
	0.0f, 1.0f, 0.0f,	 // normal
	0.0f, 5.0f,			 // texture coordinate
	-5.0f, -1.0f, -5.0f, // position
	0.0f, 1.0f, 0.0f,	 // normal
	0.0f, 5.0f,			 // texture coordinate
	5.0f, -1.0f, 5.0f,	 // position
	0.0f, 1.0f, 0.0f,	 // normal
	5.0f, 0.0f,			 // texture coordinate
	5.0f, -1.0f, -5.0f,	 // position
	0.0f, 1.0f, 0.0f,	 // normal
	5.0f, 5.0f,			 // texture coordinate

	// unit cube
	-0.5f, 0.5f, 0.5f,	 // position
	0.0f, 0.0f, 1.0f,	 // normal
	0.0f, 1.0f,			 // texture coordinate
	-0.5f, -0.5f, 0.5f,	 // position
	0.0f, 0.0f, 1.0f,	 // normal
	0.0f, 0.0f,			 // texture coordinate
	0.5f, 0.5f, 0.5f,	 // position
	0.0f, 0.0f, 1.0f,	 // normal
	1.0f, 1.0f,			 // texture coordinate
	0.5f, 0.5f, 0.5f,	 // position
	0.0f, 0.0f, 1.0f,	 // normal
	1.0f, 1.0f,			 // texture coordinate
	-0.5f, -0.5f, 0.5f,	 // position
	0.0f, 0.0f, 1.0f,	 // normal
	0.0f, 0.0f,			 // texture coordinate
	0.5f, -0.5f, 0.5f,	 // position
	0.0f, 0.0f, 1.0f,	 // normal
	1.0f, 0.0f,			 // texture coordinate
	0.5f, 0.5f, 0.5f,	 // position
	1.0f, 0.0f, 0.0f,	 // normal
	0.0f, 1.0f,			 // texture coordinate
	0.5f, -0.5f, 0.5f,	 // position
	1.0f, 0.0f, 0.0f,	 // normal
	0.0f, 0.0f,			 // texture coordinate
	0.5f, 0.5f, -0.5f,	 // position
	1.0f, 0.0f, 0.0f,	 // normal
	1.0f, 1.0f,			 // texture coordinate
	0.5f, 0.5f, -0.5f,	 // position
	1.0f, 0.0f, 0.0f,	 // normal
	1.0f, 1.0f,			 // texture coordinate
	0.5f, -0.5f, 0.5f,	 // position
	1.0f, 0.0f, 0.0f,	 // normal
	0.0f, 0.0f,			 // texture coordinate
	0.5f, -0.5f, -0.5f,	 // position
	1.0f, 0.0f, 0.0f,	 // normal
	1.0f, 0.0f,			 // texture coordinate
	0.5f, -0.5f, -0.5f,	 // position
	0.0f, 0.0f, -1.0f,	 // normal
	0.0f, 0.0f,			 // texture coordinate
	-0.5f, -0.5f, -0.5f, // position
	0.0f, 0.0f, -1.0f,	 // normal
	1.0f, 0.0f,			 // texture coordinate
	0.5f, 0.5f, -0.5f,	 // position
	0.0f, 0.0f, -1.0f,	 // normal
	0.0f, 1.0f,			 // texture coordinate
	0.5f, 0.5f, -0.5f,	 // position
	0.0f, 0.0f, -1.0f,	 // normal
	0.0f, 1.0f,			 // texture coordinate
	-0.5f, -0.5f, -0.5f, // position
	0.0f, 0.0f, -1.0f,	 // normal
	1.0f, 0.0f,			 // texture coordinate
	-0.5f, 0.5f, -0.5f,	 // position
	0.0f, 0.0f, -1.0f,	 // normal
	1.0f, 1.0f,			 // texture coordinate
	-0.5f, -0.5f, 0.5f,	 // position
	-1.0f, 0.0f, 0.0f,	 // normal
	1.0f, 0.0f,			 // texture coordinate
	-0.5f, 0.5f, 0.5f,	 // position
	-1.0f, 0.0f, 0.0f,	 // normal
	1.0f, 1.0f,			 // texture coordinate
	-0.5f, -0.5f, -0.5f, // position
	-1.0f, 0.0f, 0.0f,	 // normal
	0.0f, 0.0f,			 // texture coordinate
	-0.5f, -0.5f, -0.5f, // position
	-1.0f, 0.0f, 0.0f,	 // normal
	0.0f, 0.0f,			 // texture coordinate
	-0.5f, 0.5f, 0.5f,	 // position
	-1.0f, 0.0f, 0.0f,	 // normal
	1.0f, 1.0f,			 // texture coordinate
	-0.5f, 0.5f, -0.5f,	 // position
	-1.0f, 0.0f, 0.0f,	 // normal
	0.0f, 1.0f,			 // texture coordinate
	-0.5f, 0.5f, 0.5f,	 // position
	0.0f, 1.0f, 0.0f,	 // normal
	0.0f, 0.0f,			 // texture coordinate
	0.5f, 0.5f, 0.5f,	 // position
	0.0f, 1.0f, 0.0f,	 // normal
	1.0f, 0.0f,			 // texture coordinate
	-0.5f, 0.5f, -0.5f,	 // position
	0.0f, 1.0f, 0.0f,	 // normal
	0.0f, 1.0f,			 // texture coordinate
	-0.5f, 0.5f, -0.5f,	 // position
	0.0f, 1.0f, 0.0f,	 // normal
	0.0f, 1.0f,			 // texture coordinate
	0.5f, 0.5f, 0.5f,	 // position
	0.0f, 1.0f, 0.0f,	 // normal
	1.0f, 0.0f,			 // texture coordinate
	0.5f, 0.5f, -0.5f,	 // position
	0.0f, 1.0f, 0.0f,	 // normal
	1.0f, 1.0f,			 // texture coordinate
	0.5f, -0.5f, 0.5f,	 // position
	0.0f, -1.0f, 0.0f,	 // normal
	1.0f, 1.0f,			 // texture coordinate
	-0.5f, -0.5f, 0.5f,	 // position
	0.0f, -1.0f, 0.0f,	 // normal
	0.0f, 1.0f,			 // texture coordinate
	0.5f, -0.5f, -0.5f,	 // position
	0.0f, -1.0f, 0.0f,	 // normal
	1.0f, 0.0f,			 // texture coordinate
	0.5f, -0.5f, -0.5f,	 // position
	0.0f, -1.0f, 0.0f,	 // normal
	1.0f, 0.0f,			 // texture coordinate
	-0.5f, -0.5f, 0.5f,	 // position
	0.0f, -1.0f, 0.0f,	 // normal
	0.0f, 1.0f,			 // texture coordinate
	-0.5f, -0.5f, -0.5f, // position
	0.0f, -1.0f, 0.0f,	 // normal
	0.0f, 0.0f,			 // texture coordinate
};

GLfloat g_screenspaceQuad[] = {
	-1.0f,
	1.0f,
	0.0f,
	-1.0f,
	-1.0f,
	0.0f,
	1.0f,
	1.0f,
	0.0f,
	1.0f,
	1.0f,
	0.0f,
	-1.0f,
	-1.0f,
	0.0f,
	1.0f,
	-1.0f,
	0.0f,
};

GLuint g_FBO = 0;			 // framebuffer object identifier
GLuint g_VBO[2];			 // vertex buffer object identifier
GLuint g_VAO[2];			 // vertex array object identifier
GLuint g_shaderProgramID[2]; // shader program identifier

// locations in shader
GLuint g_MVP_Index = 0;
GLuint g_MV_Index = 0;
GLuint g_V_Index = 0;
GLuint g_texSampler_Index[2];
GLuint g_renderDepth_Index;
GLuint g_lightPositionIndex = 0;
GLuint g_lightAmbientIndex = 0;
GLuint g_lightDiffuseIndex = 0;
GLuint g_lightSpecularIndex = 0;
GLuint g_materialAmbientIndex = 0;
GLuint g_materialDiffuseIndex = 0;
GLuint g_materialSpecularIndex = 0;
GLuint g_materialShininessIndex = 0;

glm::mat4 g_modelMatrix[3];	  // object's model matrix
glm::mat4 g_viewMatrix;		  // view matrix
glm::mat4 g_projectionMatrix; // projection matrix

Light g_light;			// light properties
Material g_material[3]; // material properties

unsigned char* g_texImage[2]; // image data
GLuint g_textureID[4];		  // texture identifiers

GLuint g_windowWidth = 800; // window dimensions
GLuint g_windowHeight = 600;

glm::vec3 g_translation;

bool g_bRenderDepth = false; // render depth true or false
bool g_bDepthTest = true;	 // depth test on or off

static void init(GLFWwindow* window)
{
	glEnable(GL_DEPTH_TEST); // enable depth buffer test

	// create and compile our GLSL program from the shader files
	g_shaderProgramID[0] = loadShaders("LightAndTextureVS.vert", "LightAndTextureFS.frag");
	g_shaderProgramID[1] = loadShaders("ScreenQuadVS.vert", "ScreenQuadFS.frag");

	// find the location of shader variables
	GLuint positionIndex[2];
	positionIndex[0] = glGetAttribLocation(g_shaderProgramID[0], "aPosition");
	GLuint normalIndex = glGetAttribLocation(g_shaderProgramID[0], "aNormal");
	GLuint texCoordIndex = glGetAttribLocation(g_shaderProgramID[0], "aTexCoord");
	g_MVP_Index = glGetUniformLocation(g_shaderProgramID[0], "uModelViewProjectionMatrix");
	g_MV_Index = glGetUniformLocation(g_shaderProgramID[0], "uModelViewMatrix");
	g_V_Index = glGetUniformLocation(g_shaderProgramID[0], "uViewMatrix");

	g_texSampler_Index[0] = glGetUniformLocation(g_shaderProgramID[0], "uTextureSampler");

	g_lightPositionIndex = glGetUniformLocation(g_shaderProgramID[0], "uLight.position");
	g_lightAmbientIndex = glGetUniformLocation(g_shaderProgramID[0], "uLight.ambient");
	g_lightDiffuseIndex = glGetUniformLocation(g_shaderProgramID[0], "uLight.diffuse");
	g_lightSpecularIndex = glGetUniformLocation(g_shaderProgramID[0], "uLight.specular");

	g_materialAmbientIndex = glGetUniformLocation(g_shaderProgramID[0], "uMaterial.ambient");
	g_materialDiffuseIndex = glGetUniformLocation(g_shaderProgramID[0], "uMaterial.diffuse");
	g_materialSpecularIndex = glGetUniformLocation(g_shaderProgramID[0], "uMaterial.specular");
	g_materialShininessIndex = glGetUniformLocation(g_shaderProgramID[0], "uMaterial.shininess");

	positionIndex[1] = glGetAttribLocation(g_shaderProgramID[1], "aPosition");
	g_texSampler_Index[1] = glGetUniformLocation(g_shaderProgramID[1], "uTextureSampler");
	g_renderDepth_Index = glGetUniformLocation(g_shaderProgramID[1], "uRenderDepth");

	// initialise model matrix to the identity matrix
	g_modelMatrix[0] = glm::mat4(1.0f);
	g_modelMatrix[1] = glm::translate(glm::vec3(0.0f, 0.0f, -2.0f)) * glm::scale(glm::vec3(2.0f, 2.0f, 2.0f));
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

	// read image data
	GLint imageWidth[2];  // image width info
	GLint imageHeight[2]; // image height info
	g_texImage[0] = readBitmapRGBImage("images/check.bmp", &imageWidth[0], &imageHeight[0]);
	g_texImage[1] = readBitmapRGBImage("images/smile.bmp", &imageWidth[1], &imageHeight[1]);

	// generate identifier for texture object and set texture properties
	glGenTextures(4, g_textureID);
	glBindTexture(GL_TEXTURE_2D, g_textureID[0]);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth[0], imageHeight[0], 0, GL_BGR, GL_UNSIGNED_BYTE, g_texImage[0]);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindTexture(GL_TEXTURE_2D, g_textureID[1]);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imageWidth[1], imageHeight[1], 0, GL_BGR, GL_UNSIGNED_BYTE, g_texImage[1]);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// textures to be rendered to (i.e. render-to-texture)
	glBindTexture(GL_TEXTURE_2D, g_textureID[2]);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, g_windowWidth, g_windowHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_2D, g_textureID[3]);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, g_windowWidth, g_windowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// set up framebuffer object
	glGenFramebuffers(1, &g_FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, g_FBO);

	GLuint depthRenderBufferID;
	glGenRenderbuffers(1, &depthRenderBufferID);
	glBindRenderbuffer(GL_RENDERBUFFER, depthRenderBufferID);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, g_windowWidth, g_windowHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderBufferID);

	// associate respective textures with framebuffer attachments
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, g_textureID[2], 0);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, g_textureID[3], 0);

	// number of draw buffers
	GLenum drawBuffers[1] = {GL_COLOR_ATTACHMENT0};
	glDrawBuffers(1, drawBuffers);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		exit(EXIT_FAILURE);

	// generate identifier for VBOs and copy data to GPU
	glGenBuffers(2, g_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, g_VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertices), g_vertices, GL_STATIC_DRAW);

	// generate identifiers for VAO
	glGenVertexArrays(2, g_VAO);

	// create VAO and specify VBO data
	glBindVertexArray(g_VAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, g_VBO[0]);
	glVertexAttribPointer(positionIndex[0], 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, position)));
	glVertexAttribPointer(normalIndex, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, normal)));
	glVertexAttribPointer(texCoordIndex, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, texCoord)));

	glEnableVertexAttribArray(positionIndex[0]); // enable vertex attributes
	glEnableVertexAttribArray(normalIndex);
	glEnableVertexAttribArray(texCoordIndex);

	// to display the render-to-texture's texture on a screen space quad
	glBindBuffer(GL_ARRAY_BUFFER, g_VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_screenspaceQuad), g_screenspaceQuad, GL_STATIC_DRAW);

	glBindVertexArray(g_VAO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, g_VBO[1]);
	glVertexAttribPointer(positionIndex[1], 3, GL_FLOAT, GL_FALSE, 0, 0);

	glEnableVertexAttribArray(positionIndex[1]); // enable vertex attributes
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
	// draw to frame buffer object
	glBindFramebuffer(GL_FRAMEBUFFER, g_FBO);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear colour buffer and depth buffer

	glUseProgram(g_shaderProgramID[0]); // use the shaders associated with the shader program

	glBindVertexArray(g_VAO[0]); // make VAO active

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
	glUniform1f(g_materialShininessIndex, g_material[1].shininess);

	glUniform1i(g_texSampler_Index[0], 0);

	// draw cube
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, g_textureID[0]);

	glDrawArrays(GL_TRIANGLES, 6, 36);

	// set shader variables
	MVP = g_projectionMatrix * g_viewMatrix * g_modelMatrix[2];
	MV = g_viewMatrix * g_modelMatrix[2];
	glUniformMatrix4fv(g_MVP_Index, 1, GL_FALSE, &MVP[0][0]);
	glUniformMatrix4fv(g_MV_Index, 1, GL_FALSE, &MV[0][0]);

	glUniform3fv(g_materialAmbientIndex, 1, &g_material[2].ambient[0]);
	glUniform3fv(g_materialDiffuseIndex, 1, &g_material[2].diffuse[0]);
	glUniform3fv(g_materialSpecularIndex, 1, &g_material[2].specular[0]);
	glUniform1f(g_materialShininessIndex, g_material[2].shininess);

	// draw cube
	glBindTexture(GL_TEXTURE_2D, g_textureID[1]);

	glDrawArrays(GL_TRIANGLES, 6, 36);

	// set uniform shader variables
	MVP = g_projectionMatrix * g_viewMatrix * g_modelMatrix[0];
	MV = g_viewMatrix * g_modelMatrix[0];
	glUniformMatrix4fv(g_MVP_Index, 1, GL_FALSE, &MVP[0][0]);
	glUniformMatrix4fv(g_MV_Index, 1, GL_FALSE, &MV[0][0]);

	glUniform3fv(g_materialAmbientIndex, 1, &g_material[0].ambient[0]);
	glUniform3fv(g_materialDiffuseIndex, 1, &g_material[0].diffuse[0]);
	glUniform3fv(g_materialSpecularIndex, 1, &g_material[0].specular[0]);
	glUniform1f(g_materialShininessIndex, g_material[0].shininess);

	// draw plane
	glBindTexture(GL_TEXTURE_2D, g_textureID[0]);

	glDrawArrays(GL_TRIANGLES, 0, 6);

	// draw to normal display
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear colour buffer and depth buffer

	glUseProgram(g_shaderProgramID[1]); // use the shaders associated with the shader program

	glBindVertexArray(g_VAO[1]); // make VAO active

	glUniform1i(g_texSampler_Index[1], 0);

	// draw screenspace quad
	glActiveTexture(GL_TEXTURE0);

	if (g_bRenderDepth)
	{
		glUniform1i(g_renderDepth_Index, 1);
		glBindTexture(GL_TEXTURE_2D, g_textureID[3]);
	}
	else
	{
		glUniform1i(g_renderDepth_Index, 0);
		glBindTexture(GL_TEXTURE_2D, g_textureID[2]);
	}

	glDrawArrays(GL_TRIANGLES, 0, 6);

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
	// if the 0 key is pressed that a screen capture
	if (key == GLFW_KEY_0 && action == GLFW_PRESS)
	{
		// compute size of the image
		int size = g_windowWidth * g_windowHeight * 3;

		// allocate memory to store the image
		unsigned char* outBuffer = new unsigned char[size];

		// read pixels from the colour buffer
		glReadPixels(0, 0, g_windowWidth, g_windowHeight, GL_RGB, GL_UNSIGNED_BYTE, (GLvoid*)outBuffer);

		// write the image to a file
		writeBitmapRGBImage("images/screenshot.bmp", (char*)outBuffer, g_windowWidth, g_windowHeight);

		// deallocate memory
		delete[] outBuffer;
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
	std::cerr << description << std::endl; // output error description
}

int main(void)
{
	GLFWwindow* window = NULL; // pointer to a GLFW window handle
	TwBar* TweakBar;		   // pointer to a tweak bar

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
	glfwSetCursorPosCallback(window, cursor_position_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	// initialise AntTweakBar
	TwInit(TW_OPENGL_CORE, NULL);

	// give tweak bar the size of graphics window
	TwWindowSize(g_windowWidth, g_windowHeight);
	TwDefine(" TW_HELP visible=false "); // disable help menu
	TwDefine(" GLOBAL fontsize=3 ");	 // set large font size

	// create a tweak bar
	TweakBar = TwNewBar("Main");
	TwDefine(" Main label='Controls' refresh=0.02 text=light size='220 220' ");

	// depth test entries
	TwAddVarRW(TweakBar, "DepthTest", TW_TYPE_BOOLCPP, &g_bDepthTest, " group='Depth Buffer' ");
	TwAddVarRW(TweakBar, "RenderDepth", TW_TYPE_BOOLCPP, &g_bRenderDepth, " group='Depth Buffer' ");

	// create transformation entries
	TwAddVarRW(TweakBar, "Translate x", TW_TYPE_FLOAT, &g_translation[0], " group='Transformation' min=-2.0 max=2.0 step=0.1 ");
	TwAddVarRW(TweakBar, "Translate y", TW_TYPE_FLOAT, &g_translation[1], " group='Transformation' min=-2.0 max=2.0 step=0.1 ");
	TwAddVarRW(TweakBar, "Translate z", TW_TYPE_FLOAT, &g_translation[2], " group='Transformation' min=-5.0 max=5.0 step=0.1 ");

	// initialise rendering states
	init(window);

	// the rendering loop
	while (!glfwWindowShouldClose(window))
	{
		update_scene(); // update the scene

		// which face to cull
		if (g_bDepthTest)
			glEnable(GL_DEPTH_TEST); // enable depth buffer test
		else
			glDisable(GL_DEPTH_TEST); // enable depth buffer test

		render_scene(); // render the scene

		TwDraw(); // draw tweak bar(s)

		glfwSwapBuffers(window); // swap buffers
		glfwPollEvents();		 // poll for events
	}

	// clean up
	if (g_texImage[0])
		delete[] g_texImage[0];
	if (g_texImage[1])
		delete[] g_texImage[1];

	glDeleteProgram(g_shaderProgramID[0]);
	glDeleteProgram(g_shaderProgramID[1]);
	glDeleteBuffers(2, g_VBO);
	glDeleteVertexArrays(2, g_VAO);
	glDeleteTextures(4, g_textureID);
	glDeleteFramebuffers(1, &g_FBO);

	// uninitialise tweak bar
	TwTerminate();

	// close the window and terminate GLFW
	glfwDestroyWindow(window);
	glfwTerminate();

	exit(EXIT_SUCCESS);
}
