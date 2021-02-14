// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <cstddef>
#include <cstdio> // for C++ i/o
#include <iostream>
#include <string>

#include <AntTweakBar.h> // include AntTweakBar for GUI
#include <GL/glew.h>	 // include GLEW
#include <GLFW/glfw3.h>	 // include GLFW (which includes the OpenGL header)

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
	-0.5f, 0.5f, 0.0f, // position
	1.0f, 0.0f, 0.0f,  // colour
	// vertex 2
	-0.5f, -0.5f, 0.0f, // position
	1.0f, 0.0f, 0.0f,	// colour
	// vertex 3
	0.5f, 0.5f, 0.0f, // position
	1.0f, 0.0f, 0.0f, // colour

	// triangle 2
	// vertex 1
	0.5f, 0.5f, 0.0f, // position
	0.0f, 1.0f, 0.0f, // colour
	// vertex 2
	-0.5f, -0.5f, 0.0f, // position
	0.0f, 1.0f, 0.0f,	// colour
	// vertex 3
	0.5f, -0.5f, 0.0f, // position
	0.0f, 1.0f, 0.0f,  // colour
};

GLuint g_VBO = 0;			  // vertex buffer object identifier
GLuint g_VAO = 0;			  // vertex array object identifier
GLuint g_shaderProgramID = 0; // shader program identifier

GLuint g_windowWidth = 800; // window dimensions
GLuint g_windowHeight = 800;
bool g_wireFrame = false;						// wireframe on or off
float g_backgroundColor[] = {0.0f, 0.0f, 0.0f}; // variables to set the background color

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
	glClearColor(g_backgroundColor[0], g_backgroundColor[1], g_backgroundColor[2], 1.0f); // set clear background colour
	glClear(GL_COLOR_BUFFER_BIT);														  // clear colour buffer

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
	GLFWwindow* window = NULL;			   // pointer to a GLFW window handle
	TwBar* TweakBar;					   // pointer to a tweak bar
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
	TwDefine(" Main label='My GUI' refresh=0.02 text=light size='220 250' ");

	// create display entries
	TwAddVarRW(TweakBar, "Wireframe", TW_TYPE_BOOLCPP, &g_wireFrame, " group='Display' ");
	TwAddVarRW(TweakBar, "BgColor", TW_TYPE_COLOR3F, &g_backgroundColor, " label='Background Color' group='Display' opened=true ");

	// create frame info entries
	TwAddVarRO(TweakBar, "FPS", TW_TYPE_INT32, &FPS, " group='Frame' ");
	TwAddVarRO(TweakBar, "Frame Time", TW_TYPE_DOUBLE, &frameTime, " group='Frame' precision=4 ");

	// make the frame group a sub category of the display group
	TwDefine(" Main/Frame group='Display' ");

	// display a separator
	TwAddSeparator(TweakBar, NULL, NULL);

	// initialise rendering states
	init();

	// the rendering loop
	while (!glfwWindowShouldClose(window))
	{
		if (g_wireFrame)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		render_scene(); // render the scene

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		TwDraw(); // draw tweak bar(s)

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
