#version 330 core

// interpolated values from the vertex shaders
in vec3 vPos;
in vec3 vN;

// light and material structs
struct Light
{
	vec3 position;
};

struct Material
{
	vec3 surfaceColour;
	vec3 coolColour;
	vec3 warmColour;
	float shininess;
};

// uniform input data
uniform mat4 uViewMatrix;
uniform Light uLight;
uniform Material uMaterial;
uniform float uAlpha;
uniform float uBeta;

// output data
out vec3 fColor;

void main()
{
    vec3 N = normalize(vN);
    vec3 L = normalize((uViewMatrix * vec4(uLight.position, 1.0f)).xyz - vPos);
    vec3 E = normalize(-vPos);
    vec3 H = normalize(L + E);

	// dot product between light and surface normal
	float dotLN = max(dot(L, N), 0.0);

	// Gooch shading blend between cool and warm colour
	vec3 kCool = uMaterial.coolColour + uAlpha * uMaterial.surfaceColour;
	vec3 kWarm = uMaterial.warmColour + uBeta * uMaterial.surfaceColour;
    vec3 colour = mix(kCool, kWarm, dotLN);

	// specular contribution
	float specular = 0.0f;

	if(dotLN > 0.0f)
	{
		specular = pow(max(dot(N, H), 0.0), uMaterial.shininess);
	}

	// set output color
	fColor = min(colour + specular, 1.0f);	
}