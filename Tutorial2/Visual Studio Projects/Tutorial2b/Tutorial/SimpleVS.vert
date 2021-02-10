#version 330 core

// input data (different for all executions of this shader)
layout(location = 0) in vec3 aPosition;

void main()
{
	// set vertex position
    gl_Position = vec4(aPosition, 1.0f);
}

