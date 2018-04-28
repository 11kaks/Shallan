#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;

// Notice that the "2" here equals the "2" in glVertexAttribPointer
layout(location = 2) in vec3 vertexNormal_modelspace;

// Values that stay constant for the whole mesh.
uniform mat4 MVP;
uniform mat4 M;
uniform mat4 V;
uniform vec3 LightPosition_worldspace;
uniform vec3 CameraPosition_worldspace;

// Outputs
out vec3 EyeDirection_cameraspace;
out vec3 Position_worldspace;
out vec3 Normal_cameraspace;
out vec3 LightDirection_cameraspace;
out vec3 LightColor;
out vec3 MaterialDiffuseColor;

// new stuff

out vec3 posMS;
out vec3 normMS;


out VS_OUT {
    vec3 FragPosWorldSpace;
    vec3 LightPosTangentSpace;
    vec3 ViewPosTangentSpace;
    vec3 FragPosTangentSpace;
} vs_out;

// -------


vec3 normal(){
	return vertexNormal_modelspace;
}

vec3 tangent(){
	return cross(vertexNormal_modelspace,vec3(1,0,0));
}

vec3 bitangent(){
	return cross(vertexNormal_modelspace,tangent());
}



// Super hacky version!

void main (void){
	// Output position of the vertex, in clip space : MVP * position
	posMS = vertexPosition_modelspace;
	normMS = vertexNormal_modelspace;

	gl_Position =  MVP * vec4(vertexPosition_modelspace,1);


//##############################################

	vs_out.FragPosWorldSpace   = vec3(M * vec4(vertexPosition_modelspace, 1.0));
    
    vec3 T   = normalize(mat3(M) * tangent());
    vec3 B   = normalize(mat3(M) * bitangent());
    vec3 N   = normalize(mat3(M) * normal());
    mat3 TBN = transpose(mat3(T, B, N));

    vs_out.LightPosTangentSpace = TBN * LightPosition_worldspace;
    vs_out.ViewPosTangentSpace  = TBN * CameraPosition_worldspace;
    vs_out.FragPosTangentSpace  = TBN * vs_out.FragPosWorldSpace;


//##############################################



	// Position of the vertex, in worldspace : M * position
	Position_worldspace = (M * vec4(vertexPosition_modelspace,1)).xyz;

	// Vector that goes from the vertex to the camera, in camera space.
	// FIXME light direction is somewhere else!
	vec3 vertexPosition_cameraspace = ( V * M * vec4(vertexPosition_modelspace,1)).xyz;
	EyeDirection_cameraspace = vec3(0,0,0) - vertexPosition_cameraspace;

	// Vector that goes from the vertex to the light, in camera space. M is ommited because it's identity.
	vec3 LightPosition_cameraspace = ( V *  vec4(LightPosition_worldspace,1)).xyz;
	LightDirection_cameraspace = LightPosition_cameraspace + EyeDirection_cameraspace;
	
	// Normal of the the vertex, in camera space
	// Only correct if ModelMatrix does not scale the model ! Use its inverse transpose if not.
	Normal_cameraspace = ( V * M * vec4(vertexNormal_modelspace,0)).xyz; 

	// FIXME kovakoodattu valon väri
	LightColor = vec3(1.0,1.0,1.0);
	MaterialDiffuseColor = vec3(1.0,0,0);

}
