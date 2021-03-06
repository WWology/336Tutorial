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
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "shader.h"

// struct for vertex attributes
typedef struct Vertex
{
	GLfloat position[3];
	GLfloat normal[3];
} Vertex;

// struct for mesh properties
typedef struct Mesh
{
	Vertex* pMeshVertices;		// pointer to mesh vertices
	GLint numberOfVertices;		// number of vertices in the mesh
	GLint* pMeshIndices;		// pointer to mesh indices
	GLint numberOfFaces;		// number of faces in the mesh
} Mesh;

// light and material structs
typedef struct Light
{
	glm::vec3 position;
} Light;

typedef struct Material
{
	glm::vec3 surfaceColour;
	glm::vec3 coolColour;
	glm::vec3 warmColour;
	float shininess;
} Material;

// Global variables
Mesh g_mesh;					// mesh

GLuint g_IBO = 0;				// index buffer object identifier
GLuint g_VBO = 0;				// vertex buffer object identifier
GLuint g_VAO = 0;				// vertex array object identifier
GLuint g_shaderProgramID = 0;	// shader program identifier

// locations in shader
GLuint g_MVP_Index = 0;
GLuint g_MV_Index = 0;
GLuint g_V_Index = 0;
GLuint g_lightPositionIndex = 0;
GLuint g_materialColourIndex = 0;
GLuint g_materialCoolIndex = 0;
GLuint g_materialWarmIndex = 0;
GLuint g_materialShininessIndex = 0;
GLuint g_alphaIndex = 0;
GLuint g_betaIndex = 0;

GLfloat g_alpha;
GLfloat g_beta;

glm::mat4 g_modelMatrix;		// object's model matrix
glm::mat4 g_viewMatrix;			// view matrix
glm::mat4 g_projectionMatrix;	// projection matrix

Light g_light;					// light properties
Material g_material;			// material properties

GLuint g_windowWidth = 800;		// window dimensions
GLuint g_windowHeight = 600;

float g_rotateAngleX = 90.0f;
float g_rotateAngleZ = 0.0f;

bool load_mesh(const char* fileName, Mesh* mesh);

static void init(GLFWwindow* window)
{
	glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
	glEnable(GL_DEPTH_TEST);	// enable depth buffer test

	// create and compile our GLSL program from the shader files
	g_shaderProgramID = loadShaders("PerFragLightingVS.vert", "GoochShadingFS.frag");

	// find the location of shader variables
	GLuint positionIndex = glGetAttribLocation(g_shaderProgramID, "aPosition");
	GLuint normalIndex = glGetAttribLocation(g_shaderProgramID, "aNormal");
	g_MVP_Index = glGetUniformLocation(g_shaderProgramID, "uModelViewProjectionMatrix");
	g_MV_Index = glGetUniformLocation(g_shaderProgramID, "uModelViewMatrix");
	g_V_Index = glGetUniformLocation(g_shaderProgramID, "uViewMatrix");

	g_lightPositionIndex = glGetUniformLocation(g_shaderProgramID, "uLight.position");

	g_materialColourIndex = glGetUniformLocation(g_shaderProgramID, "uMaterial.surfaceColour");
	g_materialCoolIndex = glGetUniformLocation(g_shaderProgramID, "uMaterial.coolColour");
	g_materialWarmIndex = glGetUniformLocation(g_shaderProgramID, "uMaterial.warmColour");
	g_materialShininessIndex = glGetUniformLocation(g_shaderProgramID, "uMaterial.shininess");

	g_alphaIndex = glGetUniformLocation(g_shaderProgramID, "uAlpha");
	g_betaIndex = glGetUniformLocation(g_shaderProgramID, "uBeta");

	// initialise model matrix to the identity matrix
	g_modelMatrix = glm::mat4(1.0f);

	// initialise view matrix
	g_viewMatrix = glm::lookAt(glm::vec3(0.0f, 0.0f, 4.0f), glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	float aspectRatio = static_cast<float>(width) / height;

	// initialise projection matrix
	g_projectionMatrix = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f);

	// load mesh
	g_mesh.pMeshVertices = NULL;
	g_mesh.pMeshIndices = NULL;
	load_mesh("models/torus.obj", &g_mesh);

	// initialise point light properties
	g_light.position = glm::vec3(5.0f, 5.0f, 5.0f);

	// initialise material properties
	g_material.surfaceColour = glm::vec3(0.2f, 0.5f, 0.3f);
	g_material.coolColour = glm::vec3(0.0f, 0.0f, 0.8f);
	g_material.warmColour = glm::vec3(0.8f, 0.8f, 0.0f);
	g_material.shininess = 40.0f;

	// Gooch shading alpha and beta values
	g_alpha = 0.4f;
	g_beta = 0.4f;

	// generate identifier for VBOs and copy data to GPU
	glGenBuffers(1, &g_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, g_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*g_mesh.numberOfVertices, g_mesh.pMeshVertices, GL_STATIC_DRAW);

	// generate identifier for IBO and copy data to GPU
	glGenBuffers(1, &g_IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLint) * 3 * g_mesh.numberOfFaces, g_mesh.pMeshIndices, GL_STATIC_DRAW);

	// generate identifiers for VAO
	glGenVertexArrays(1, &g_VAO);

	// create VAO and specify VBO data
	glBindVertexArray(g_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, g_VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_IBO);
	glVertexAttribPointer(positionIndex, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, position)));
	glVertexAttribPointer(normalIndex, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, normal)));

	glEnableVertexAttribArray(positionIndex);	// enable vertex attributes
	glEnableVertexAttribArray(normalIndex);
}

// function used to update the scene
static void update_scene()
{
	// update model matrix
	g_modelMatrix = glm::rotate(glm::radians(g_rotateAngleX), glm::vec3(1.0f, 0.0f, 0.0f))
		* glm::rotate(glm::radians(g_rotateAngleZ), glm::vec3(0.0f, 0.0f, 1.0f));
}

// function used to render the scene
static void render_scene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// clear colour buffer and depth buffer

	glUseProgram(g_shaderProgramID);	// use the shaders associated with the shader program

	glBindVertexArray(g_VAO);		// make VAO active

	// set uniform shader variables
	glm::mat4 MVP = g_projectionMatrix * g_viewMatrix * g_modelMatrix;
	glm::mat4 MV = g_viewMatrix * g_modelMatrix;
	glUniformMatrix4fv(g_MVP_Index, 1, GL_FALSE, &MVP[0][0]);
	glUniformMatrix4fv(g_MV_Index, 1, GL_FALSE, &MV[0][0]);
	glUniformMatrix4fv(g_V_Index, 1, GL_FALSE, &g_viewMatrix[0][0]);

	glUniform3fv(g_lightPositionIndex, 1, &g_light.position[0]);

	glUniform3fv(g_materialColourIndex, 1, &g_material.surfaceColour[0]);
	glUniform3fv(g_materialCoolIndex, 1, &g_material.coolColour[0]);
	glUniform3fv(g_materialWarmIndex, 1, &g_material.warmColour[0]);
	glUniform1f(g_materialShininessIndex, g_material.shininess);

	glUniform1f(g_alphaIndex, g_alpha);
	glUniform1f(g_betaIndex, g_beta);

	glDrawElements(GL_TRIANGLES, g_mesh.numberOfFaces * 3, GL_UNSIGNED_INT, 0);	// display the vertices based on their indices and primitive type

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

	// create Gooch shading entries
	TwAddVarRW(TweakBar, "Alpha", TW_TYPE_FLOAT, &g_alpha, " group='Gooch Shading' min=0.0 max=1.0 step=0.01");
	TwAddVarRW(TweakBar, "Beta", TW_TYPE_FLOAT, &g_beta, " group='Gooch Shading' min=0.0 max=1.0 step=0.01");

	// create material properties entries
	TwAddVarRW(TweakBar, "Colour", TW_TYPE_COLOR3F, &g_material.surfaceColour[0], " group='Material' ");
	TwAddVarRW(TweakBar, "Cool", TW_TYPE_COLOR3F, &g_material.coolColour[0], " group='Material' ");
	TwAddVarRW(TweakBar, "Warm", TW_TYPE_COLOR3F, &g_material.warmColour[0], " group='Material' ");

	// create transformation entries
	TwAddVarRW(TweakBar, "Rotate x-axis", TW_TYPE_FLOAT, &g_rotateAngleX, " group='Transformation' min=-360 max=360 step=1");
	TwAddVarRW(TweakBar, "Rotate z-axis", TW_TYPE_FLOAT, &g_rotateAngleZ, " group='Transformation' min=-360 max=360 step=1");

	// create light position
	TwAddVarRW(TweakBar, "Light: x", TW_TYPE_FLOAT, &g_light.position[0], " group='Light Position' min=-10.0 max=10.0 step=0.1");
	TwAddVarRW(TweakBar, "Light: y", TW_TYPE_FLOAT, &g_light.position[1], " group='Light Position' min=-10.0 max=10.0 step=0.1");
	TwAddVarRW(TweakBar, "Light: z", TW_TYPE_FLOAT, &g_light.position[2], " group='Light Position' min=0.0 max=10.0 step=0.1");

	// initialise rendering states
	init(window);

	// the rendering loop
	while (!glfwWindowShouldClose(window))
	{
		update_scene();		// update the scene
		render_scene();		// render the scene

		TwDraw();			// draw tweak bar(s)

		glfwSwapBuffers(window);	// swap buffers
		glfwPollEvents();			// poll for events
	}

	// clean up
	if (g_mesh.pMeshVertices)
		delete[] g_mesh.pMeshVertices;
	if (g_mesh.pMeshIndices)
		delete[] g_mesh.pMeshIndices;

	glDeleteProgram(g_shaderProgramID);
	glDeleteBuffers(1, &g_IBO);
	glDeleteBuffers(1, &g_VBO);
	glDeleteVertexArrays(1, &g_VAO);

	// uninitialise tweak bar
	TwTerminate();

	// close the window and terminate GLFW
	glfwDestroyWindow(window);
	glfwTerminate();

	exit(EXIT_SUCCESS);
}

bool load_mesh(const char* fileName, Mesh* mesh)
{
	// load file with assimp
	const aiScene* pScene = aiImportFile(fileName, aiProcess_Triangulate
		| aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices);

	// check whether scene was loaded
	if (!pScene)
	{
		cout << "Could not load mesh." << endl;
		return false;
	}

	// get pointer to mesh 0
	const aiMesh* pMesh = pScene->mMeshes[0];

	// store number of mesh vertices
	mesh->numberOfVertices = pMesh->mNumVertices;

	// if mesh contains vertex coordinates
	if (pMesh->HasPositions())
	{
		// allocate memory for vertices
		mesh->pMeshVertices = new Vertex[pMesh->mNumVertices];

		// read vertex coordinates and store in the array
		for (int i = 0; i < pMesh->mNumVertices; i++)
		{
			const aiVector3D* pVertexPos = &(pMesh->mVertices[i]);

			mesh->pMeshVertices[i].position[0] = (GLfloat)pVertexPos->x;
			mesh->pMeshVertices[i].position[1] = (GLfloat)pVertexPos->y;
			mesh->pMeshVertices[i].position[2] = (GLfloat)pVertexPos->z;
		}
	}

	// if mesh contains normals
	if (pMesh->HasNormals())
	{
		// read normals and store in the array
		for (int i = 0; i < pMesh->mNumVertices; i++)
		{
			const aiVector3D* pVertexNormal = &(pMesh->mNormals[i]);

			mesh->pMeshVertices[i].normal[0] = (GLfloat)pVertexNormal->x;
			mesh->pMeshVertices[i].normal[1] = (GLfloat)pVertexNormal->y;
			mesh->pMeshVertices[i].normal[2] = (GLfloat)pVertexNormal->z;
		}
	}

	// if mesh contains faces
	if (pMesh->HasFaces())
	{
		// store number of mesh faces
		mesh->numberOfFaces = pMesh->mNumFaces;

		// allocate memory for vertices
		mesh->pMeshIndices = new GLint[pMesh->mNumFaces * 3];

		// read normals and store in the array
		for (int i = 0; i < pMesh->mNumFaces; i++)
		{
			const aiFace* pFace = &(pMesh->mFaces[i]);

			mesh->pMeshIndices[i * 3] = (GLint)pFace->mIndices[0];
			mesh->pMeshIndices[i * 3 + 1] = (GLint)pFace->mIndices[1];
			mesh->pMeshIndices[i * 3 + 2] = (GLint)pFace->mIndices[2];
		}
	}

	// release the scene
	aiReleaseImport(pScene);

	return true;
}
