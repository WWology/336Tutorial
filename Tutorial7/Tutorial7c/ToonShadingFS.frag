#version 330 core

// interpolated values from the vertex shaders
in vec3 vPos;
in vec3 vN;

// light and material structs
struct Light
{
	vec3 position;
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	int type;
};

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

// uniform input data
uniform mat4 uViewMatrix;
uniform Light uLight;
uniform Material uMaterial;
uniform float uEdgeThickness;
uniform int uNumOfThresholds;
uniform float uSpecularCutoff;

// output data
out vec3 fColor;

void main()
{
    vec3 N = normalize(vN);
    vec3 L;

	// determine whether the light is a point light source or directional light
	if(uLight.type == 0)
		L = normalize((uViewMatrix * vec4(uLight.position, 1.0f)).xyz - vPos);
	else
		L = normalize((uViewMatrix * vec4(-uLight.direction, 0.0f)).xyz);

    vec3 E = normalize(-vPos);
    vec3 H = normalize(L + E);

/*
	// output black edge if normal is pointing away from the camera
	if(dot(E, N) < uEdgeThickness)
	{
		fColor = vec3(0.0f, 0.0f, 0.0f);	
		return;
	}
*/	

	// calculate the ambient, diffuse and specular components
	vec3 ambient  = uLight.ambient * uMaterial.ambient;
    vec3 diffuse;
	diffuse = uLight.diffuse * uMaterial.diffuse * max(dot(L, N), 0.0);
/*
	// calculate diffuse component by thresholding the values
	float thresholdFactor = floor(max(dot(L, N), 0.0) * uNumOfThresholds)/uNumOfThresholds;
    diffuse = thresholdFactor * uLight.diffuse * uMaterial.diffuse;
*/

	// calculate specular component
	vec3 specular = vec3(0.0f, 0.0f, 0.0f);

	if(dot(L, N) > 0.0f)
	{
		float specularFactor = pow(max(dot(N, H), 0.0), uMaterial.shininess);
		
		// only calculate specular component if above cutoff threshold
//		if(specularFactor > uSpecularCutoff)
		specular = specularFactor * uLight.specular * uMaterial.specular;
	}

	// set output color
	fColor = diffuse + specular + ambient;
}