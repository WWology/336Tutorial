#version 330 core

// input data (different for all executions of this shader)
in vec3 aPosition;
in vec3 aNormal;

// light and material structs
struct Light
{
	vec3 position;	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

// uniform input data
uniform mat4 uModelViewProjectionMatrix;
uniform mat4 uModelViewMatrix;
uniform mat4 uViewMatrix;
uniform Light uLight;
uniform Material uMaterial;

// output data (will be interpolated for each fragment)
out vec3 vColor;

void main()
{
	// set vertex position
    gl_Position = uModelViewProjectionMatrix * vec4(aPosition, 1.0);

	// eye/camera space
	vec3 EC_VertexPosition = (uModelViewMatrix * vec4(aPosition, 1.0)).xyz;
	vec3 EC_Normal = (uModelViewMatrix * vec4(aNormal, 0.0)).xyz;

	vec3 N = normalize(EC_Normal);
	vec3 L = normalize((uViewMatrix * vec4(uLight.position, 1.0)).xyz - EC_VertexPosition);
	vec3 E = normalize(-EC_VertexPosition);
	vec3 R = reflect(-L, N);

	vec3 ambient  = uLight.ambient * uMaterial.ambient;
    vec3 diffuse  = uLight.diffuse * uMaterial.diffuse * max(dot(L, N), 0.0);
	vec3 specular = vec3(0.0f, 0.0f, 0.0f);

	if(dot(L, N) > 0.0f)
	    specular = uLight.specular * uMaterial.specular * pow(max(dot(E, R), 0.0), uMaterial.shininess);

	// the colour of each vertex will be interpolated
	// to produce the colour of each fragment
	vColor = diffuse + specular + ambient;
}

