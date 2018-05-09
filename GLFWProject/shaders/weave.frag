#version 330 core

in VS_OUT {
    vec3 FragPosWorldSpace;
    vec3 LightPosTangentSpace;
    vec3 CameraPosTangentSpace;
    vec3 FragPosTangentSpace;
} fs_in;

in vec3 LightColor;
in vec3 MaterialDiffuseColor;

out vec3 color;

bool useParallax = false;

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

float fNorm(float a, float e){
	float min = -1;
	float max = 1 + r;
	return (min)/(max-min);
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


	//####### Parallax #######
	if (useParallax){
	// View direction
	vec3 v = normalize(  fs_in.CameraPosTangentSpace  - fs_in.FragPosTangentSpace );

	vec2 p = fs_in.FragPosTangentSpace.xy;
	vec2 pa = p + fNorm(a,e) * v.xy;

	a = pa.x;
	e = pa.y;
}
	// ####################

	


	// Normal of the computed fragment, in tangent space
	vec3 n = normalize(vec3(partialDerivativeX(a,e), partialDerivativeY(a,e), f(a,e)));

	// Direction of the light (from the fragment to the light)
	vec3 l = normalize(  fs_in.LightPosTangentSpace  - fs_in.FragPosTangentSpace );

	vec3 MaterialAmbientColor = vec3(0.2) * MaterialDiffuseColor;

	// Cosine of the angle between the normal and the light direction,
	// clamped above 0
	float cosTheta = clamp( dot( n,l ), 0,1 );

	color = MaterialDiffuseColor * LightColor * cosTheta;
	//color = MaterialAmbientColor + MaterialDiffuseColor * LightColor * cosTheta;
	//color = MaterialAmbientColor * 50 * cosTheta;

	float beep = 0.2;
	vec3 testCol = vec3(1,1,1);
	vec3 testCol2 = vec3(0,0.6,0.5);
/*
	if(    abs(fs_in.FragPosTangentSpace.x) < beep 
	    && abs(fs_in.FragPosTangentSpace.y) < beep 
	  //&& abs(fs_in.FragPosTangentSpace.z) < beep    
	){
		color = testCol * cosTheta;
	} else {
		color = testCol2 * (cosTheta );
	}
*/
	// light debug
	//color = abs(l);

	// ae debug
	//vec3 aeCol = vec3(a,e,0);
	//color = abs(aeCol);

	// xy debug
	//vec3 xyCol = vec3(0.1,0.1,0.1);
	//color = xyCol;


	// position debug
	// r - x
	// g - y
	// b - z
	//color = cosTheta * fs_in.FragPosTangentSpace;
}
