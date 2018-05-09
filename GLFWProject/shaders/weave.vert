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

out vec3 LightColor;
out vec3 MaterialDiffuseColor;

out VS_OUT {
    vec3 FragPosWorldSpace;
    vec3 LightPosTangentSpace;
   // vec3 ViewPosTangentSpace;
    vec3 FragPosTangentSpace;
} vs_out;

vec3 mainAxis = vec3(1,0,0);

vec3 normal(){
	return vertexNormal_modelspace;
}

vec3 tangent(){
	return cross(vertexNormal_modelspace,mainAxis);
}

vec3 bitangent(){
	return cross(vertexNormal_modelspace,tangent());
}


void main (void){

	// Our normal and main axis cannot point to same direction
	// or else we'll get tangent == zero vector.
	if(length(cross(mainAxis, vertexNormal_modelspace)) < 0.001){
		mainAxis =  mainAxis + vec3(0,1,0);
	}


//##############################################

	vs_out.FragPosWorldSpace   = vec3(M * vec4(vertexPosition_modelspace, 1.0));
    
    vec3 T   = normalize(mat3(M) * tangent());
    vec3 B   = normalize(mat3(M) * bitangent());
    vec3 N   = normalize(mat3(M) * normal());

	//vec3 T   = normalize(mat3(M) * vec3(1,0,0));
    //vec3 B   = normalize(mat3(M) * vec3(0,1,0));
    //vec3 N   = normalize(mat3(M) * vec3(0,0,1));

    mat3 TBN = transpose(mat3(T, B, N));

    vs_out.LightPosTangentSpace = TBN * LightPosition_worldspace;
   // vs_out.ViewPosTangentSpace  = TBN * CameraPosition_worldspace;
    vs_out.FragPosTangentSpace  = TBN * vs_out.FragPosWorldSpace;


//##############################################

	gl_Position =  MVP * vec4(vertexPosition_modelspace,1);

	// FIXME kovakoodattu valon väri
	LightColor = vec3(1.0,1.0,1.0);
	MaterialDiffuseColor = vec3(1.0,0,0);

}
