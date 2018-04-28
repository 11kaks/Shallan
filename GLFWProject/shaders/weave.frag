#version 330 core


// new stuff

in vec3 posMS;
in vec3 normMS;

in VS_OUT {
    vec3 FragPosWorldSpace;
    vec3 LightPosTangentSpace;
    vec3 ViewPosTangentSpace;
    vec3 FragPosTangentSpace;
} fs_in;

// -----

in vec3 EyeDirection_cameraspace;
in vec3 Position_worldspace;
in vec3 Normal_cameraspace;
in vec3 LightDirection_cameraspace;
in vec3 LightColor;
in vec3 MaterialDiffuseColor;

out vec3 color;


float r = 0.6;
float scaleX = 4.0;
float scaleY = 1.0;
float scaleO = 100.0;

float f(float a, float e){
	float c = r * abs(sin(e));
	float b = 3.14 * floor(sin(e));

	float w = sin(a + b) + c;
	return w;
}

float partialDerivativeX(float a, float e){
	//return cos(a + floor(sin(e))) + r * abs(sin(e));
	return cos(floor(sin(e))); // + a; explodes
}

float partialDerivativeY(float a, float e){
	//return cos(a + floor(sin(e))) * a + r * (sin(e) * cos(e)) / abs(sin(e));
	float res = 0.0;
	if(sin(e) >= 0.0){
		res = cos(e);
	} else {
		res = -cos(e);
	}
	return r*res;
	
}

// Super hacky version!

void main (void){

	float x = fs_in.FragPosTangentSpace.x;
	float y = fs_in.FragPosTangentSpace.y;

	float a = (x * scaleO) / scaleX;
	float e = (y * scaleO) / scaleY;

	// Normal of the computed fragment, in tangent space
	vec3 n = normalize(vec3(partialDerivativeX(a,e), partialDerivativeY(a,e), f(a,e)));

	//vec3 n = normalize( Normal_cameraspace );
	//vec3 n = normalize( normal);
	// Direction of the light (from the fragment to the light)
	vec3 l = normalize( fs_in.FragPosTangentSpace - fs_in.LightPosTangentSpace );

	vec3 MaterialAmbientColor = vec3(0.2) * MaterialDiffuseColor;

	// Cosine of the angle between the normal and the light direction,
	// clamped above 0
	//  - light is at the vertical of the triangle -> 1
	//  - light is perpendicular to the triangle -> 0
	//  - light is behind the triangle -> 0
	float cosTheta = clamp( dot( n,l ), 0,1 );

	color = MaterialAmbientColor + MaterialDiffuseColor * LightColor * cosTheta;
	//color = vec3(1,1,1);
}
