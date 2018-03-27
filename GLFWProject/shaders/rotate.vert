#version 330 core
/* rotate.vert - some vector rotation functions */

// time contains the seconds since the program was linked.
//uniform float time;


//varying vec3 normal;
//varying vec3 position;

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;

// Values that stay constant for the whole mesh.
uniform mat4 MVP;

uniform float inTime;

//
// Rotation functions.
// Angles are in radians.
//

vec4 rotateAroundX( float angle, vec4 v )
{
	float sa = sin( angle );
	float ca = cos( angle );

	return vec4( v.x,
				 ca*v.y - sa*v.z,
				 sa*v.y + ca*v.z,
				 v.w );
}

//---------

vec4 rotateAroundY( float angle, vec4 v )
{
	float sa = sin( angle );
	float ca = cos( angle );

	return vec4( sa*v.z + ca*v.x,   v.y,
				 ca*v.z - sa*v.x,   v.w );
}

//---------

vec4 rotateAroundZ( float angle, vec4 v )
{
	float sa = sin( angle );
	float ca = cos( angle );

	return vec4( ca*v.x - sa*v.y,
				 sa*v.x + ca*v.y,
				 v.z, v.w );
}

//---------

mat4 rotationMatrix(vec3 axis, float angle)
{
    axis = normalize(axis);
    float s = sin(angle);
    float c = cos(angle);
    float oc = 1.0 - c;
    
    return mat4(oc * axis.x * axis.x + c,           oc * axis.x * axis.y - axis.z * s,  oc * axis.z * axis.x + axis.y * s,  0.0,
                oc * axis.x * axis.y + axis.z * s,  oc * axis.y * axis.y + c,           oc * axis.y * axis.z - axis.x * s,  0.0,
                oc * axis.z * axis.x - axis.y * s,  oc * axis.y * axis.z + axis.x * s,  oc * axis.z * axis.z + c,           0.0,
                0.0,                                0.0,                                0.0,                                1.0);
}

//
// entry point
//
void main( void )
{
	

	// rotate vertex and normal
	//vec4 vertex = rotateAroundY( time, 
	//			  rotateAroundZ( time,
	//			  rotateAroundX( time, gl_Vertex ) ) );
	//vec3 N = vec3(rotateAroundY( time, 
	//			  rotateAroundZ( time,
	//			  rotateAroundX( time, vec4(gl_Normal,1.0) ) ) ) );

	// now apply the application provided transformations
	//normal = gl_NormalMatrix * N;
	//position = vec3( gl_ModelViewMatrix * vertex );

	mat4 rotate; 
	rotate = rotationMatrix(vec3(1,0,0), inTime);
	rotate = rotate * rotationMatrix(vec3(0,1,0), inTime);
	rotate = rotate * rotationMatrix(vec3(0,0,1), inTime);

	gl_Position =  MVP * rotate * vec4(vertexPosition_modelspace,1);

	// rotate the model

	//gl_Position = gl_ModelViewProjectionMatrix * vertex;


//	gl_Position = gl_ModelViewProjectionMatrix
//				* rotateAroundY( time, vertex );

//	gl_Position = gl_ModelViewProjectionMatrix
//				* rotateAroundX( time, vertex );

//	gl_Position = ftransform();
}

