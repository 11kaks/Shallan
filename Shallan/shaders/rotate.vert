#version 330 core
/* rotate.vert - some vector rotation functions */

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;

// Notice that the "1" here equals the "1" in glVertexAttribPointer
layout(location = 1) in vec3 vertexColor;

// Values that stay constant for the whole mesh.
uniform mat4 MVP;

// Time from application
uniform float inTime;

// Output color for fragment shader
out vec3 fragmentColor;

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
	mat4 rotate; 
	rotate = rotationMatrix(vec3(1,0,0), inTime);
	rotate = rotate * rotationMatrix(vec3(0,1,0), inTime);
	rotate = rotate * rotationMatrix(vec3(0,0,1), inTime);
	
	fragmentColor = vertexColor;

	gl_Position =  MVP * rotate * vec4(vertexPosition_modelspace,1);

}

