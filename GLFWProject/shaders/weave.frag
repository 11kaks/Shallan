#version 330 core

// Our sole output value is the color of a pixel.
out vec3 color;

in VS_OUT {
    vec3 FragPosWorldSpace;
    vec3 LightPosTangentSpace;
    vec3 CameraPosTangentSpace;
    vec3 FragPosTangentSpace;
} fs_in;

// Define PI
const float PI = 3.1415926535897932384626433832795;

vec3 LightColor = vec3(1.0,1.0,1.0);
vec3 MaterialDiffuseColor = vec3(
		228.0/255.0,
		217.0/255.0,
		111.0/255.0
	);
	
bool useParallax = false;
bool useNormalMap = true;
bool useSimpleMath = true;
bool useEdgeSoft = true;

// Softening factor. Final softening is scaled with scaleO and scaleY.
float edgeSoftFac = 5;

float r = 0.6;
float scaleX = 4.0;
float scaleY = 1.0;
float scaleO = 200.0;

float LightPower = 1.00;

/*
We want our floor to give either 0 or -1. However, 
floor(sin(pi)) == 1 and this causes bright pixels in areas
that should be dark.
*/
float floorSin(float x){
	float res = floor(sin(x));
	if(res > 0.99 && res < 1.01){
		return 0.0;
	}
	return res;
}

/*
Weave function.
*/
float f(float a, float e){
	float c = r * abs(sin(e));
	float b = PI * floorSin(e);

	float w = sin(a + b) + c;
	return w;
}

/*
Weave function normalized between 0-1.
*/
float fNorm(float a, float e){
	float weave_min = -1;
	float weave_max = 1 + r;
	return (f(a,e) - weave_min)/(weave_max - weave_min);
}

/*
Weave function normalized between -1 - 1.
*/
float fNormNP(float a, float e){
	float weave_min = -1;
	float weave_max = 1 + r;
	return 2 * (f(a,e) - weave_min)/(weave_max - weave_min) - 1;
}

/*
Range -1 - 1
*/
float partialDerivativeX(float a, float e){
//return 0;
	if(useSimpleMath){
		return cos( PI * floorSin(e)); // this looks the same
	} else {
		return cos(a + PI * floorSin(e)); // this is correct mathematically
	}
}

/*
Range -1 - 1 or just return 0.0 if simpleMath is used.
*/
float partialDerivativeY(float a, float e){
	
	// For simple math just use zero. It looks pretty good.
	float res = 0.0;

	if(!useSimpleMath){
		res =  r * cos(a) * sign(sin(e));
		// Normalized between -1 - 1
		res = res / res;
	}

	return res;
}

void main (void){

	// Scale x and y to be a and e.
	float x = fs_in.FragPosTangentSpace.x;
	float y = fs_in.FragPosTangentSpace.y;
	float a = (x * scaleO) / scaleX;
	float e = (y * scaleO) / scaleY;

	// Normal is by default the z direction in tangent space.
	vec3 n = vec3(0,0,1);

	// Direction of the light (from the fragment to the light)
	vec3 l = normalize(  fs_in.LightPosTangentSpace  - fs_in.FragPosTangentSpace );

	// Not working
	if (useParallax){
		// View direction
		vec3 v =  fs_in.CameraPosTangentSpace  - fs_in.FragPosTangentSpace ;

		vec2 vAE = vec2( (v.x * scaleO) / scaleX, (v.y * scaleO) / scaleY)  ;
		vec2 vAENorm = normalize(vAE);

		vec2 p = vec2(a,e);
		vec2 pa = p + fNorm(vAENorm.x,vAENorm.y) * vAENorm;

		a = pa.x;
		e = pa.y;
	}

	float q = fNorm(a,e);

	if(useEdgeSoft){
	// Soften along e since it is the direction with large contrasts.
		float finSoft = edgeSoftFac / scaleO / scaleY;
		float q2 = fNorm(a,e+finSoft);
		float q3 = fNorm(a,e+2*finSoft);
		float q4 = fNorm(a,e-finSoft);
		q = (q +q2 +q3 +q4 ) / 4.0;
	}

	if (useNormalMap){
		// Normal of the computed fragment, in tangent space
		 n = n * normalize(vec3( partialDerivativeX(a,e), partialDerivativeY(a,e), q));
	 }

	// Cosine of the angle between the normal and the light direction,
	// clamped above 0
	float cosTheta = clamp( dot( n,l ), 0,1 );

	color =  MaterialDiffuseColor * LightColor  * LightPower * cosTheta ;

	// Use to see where space origin lies (z can be anything)
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
}
