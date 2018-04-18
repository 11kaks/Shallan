#version 330 core

in vec3 EyeDirection_cameraspace;
in vec3 Position_worldspace;
in vec3 Normal_cameraspace;
in vec3 LightDirection_cameraspace;
in vec3 LightColor;
in vec3 MaterialDiffuseColor;

out vec3 color;

void main (void){

	// Normal of the computed fragment, in camera space
	vec3 n = normalize( Normal_cameraspace );
	// Direction of the light (from the fragment to the light)
	vec3 l = normalize( LightDirection_cameraspace );

	vec3 MaterialAmbientColor = vec3(0.2) * MaterialDiffuseColor;

	// Cosine of the angle between the normal and the light direction,
	// clamped above 0
	//  - light is at the vertical of the triangle -> 1
	//  - light is perpendicular to the triangle -> 0
	//  - light is behind the triangle -> 0
	float cosTheta = clamp( dot( n,l ), 0,1 );

	color = MaterialAmbientColor + MaterialDiffuseColor * LightColor * cosTheta ;
}
