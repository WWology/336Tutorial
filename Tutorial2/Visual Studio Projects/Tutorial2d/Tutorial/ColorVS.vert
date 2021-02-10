#version 330 core

// input data (different for all executions of this shader)
in vec3 aPosition;
in vec3 aColor;

// output data (will be interpolated for each fragment)
out vec3 vColor;

void main()
{
	// set vertex position
    gl_Position = vec4(aPosition, 1.0f);

	// the color of each vertex will be interpolated
	// to produce the color of each fragment
	vColor = aColor;
}

