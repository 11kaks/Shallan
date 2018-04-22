#version 330 core


// new stuff

in vec3 posMS;
in vec3 normMS;

// -----

in vec3 EyeDirection_cameraspace;
in vec3 Position_worldspace;
in vec3 Normal_cameraspace;
in vec3 LightDirection_cameraspace;
in vec3 LightColor;
in vec3 MaterialDiffuseColor;

out vec3 color;

// Super hacky version!

void main (void){

	float r = 0.6;
	float x = posMS.x;
	float y = posMS.y;
	float scaleX = 4.0;
	float scaleY = 1.0;
	float scaleO = 100.0;

	float a = (x * scaleO) / scaleX;
	float e = (y * scaleO) / scaleY;

	float c = r * abs(sin(e));
	float b = 3.14 * floor(sin(e));

	float w = sin(a + b) + c;

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

	color = MaterialAmbientColor + MaterialDiffuseColor * LightColor * cosTheta * w;
}
