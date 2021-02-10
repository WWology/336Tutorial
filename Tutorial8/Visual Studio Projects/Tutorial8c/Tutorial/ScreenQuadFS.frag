#version 330 core

// interpolated values from the vertex shaders
in vec2 vTexCoord;

uniform sampler2D uTextureSampler;
uniform int uRenderDepth;

// output data
out vec3 fColor;

void main()
{
	// set output color
	if(uRenderDepth != 1)
		fColor = texture(uTextureSampler, vTexCoord).rgb;
	else
		fColor = (texture(uTextureSampler, vTexCoord).rrr * 10.0f) - 9.0f;
}