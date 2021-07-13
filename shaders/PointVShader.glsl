#version 450 core

layout (location = 0) in vec3 color;
layout (location = 1) in vec3 offset;
uniform mat4 modelToWorld;
uniform mat4 worldToCamera;
uniform mat4 cameraToView;
uniform int contextVal;

out VS_OUT
{
	vec4 vcolor;
} vs_out;


void main()
{
//	const vec4 vertices[3] = vec4[3](vec4( 0.25, -0.25, -1.0, 1.0),
//									 vec4(-0.25, -0.25, -1.0, 1.0),
//									 vec4( 0.25,  0.25, -1.0, 1.0));
	
    float gray = dot(color.rgb, vec3(0.299, 0.587, 0.114));
    if (contextVal == 1) vs_out.vcolor = vec4(vec3(gray), 1.0);
    else vs_out.vcolor = vec4(color, 0.3);
    gl_Position = cameraToView * worldToCamera * modelToWorld * (vec4(offset[0],offset[1],offset[2], 1.0));
}
