#version 330 core

// input data (different for all executions of this shader)
in vec3 aPosition;

// output data (will be interpolated for each fragment)
out vec2 vTexCoord;

void main()
{
	// set vertex position
    gl_Position = vec4(aPosition, 1.0);

	vTexCoord = 0.5f * (aPosition.xy + vec2(1.0f, 1.0f));
}

