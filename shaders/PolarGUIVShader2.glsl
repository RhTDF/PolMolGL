#version 450 core
layout(location = 0) in vec2 origin;
layout(location = 1) in float radius;
layout(location = 2) in float minAngle;
layout(location = 3) in float maxAngle;
//uniform in float radius;
//uniform in float minAngle;
//uniform in float maxAngle;
out vec4 vColor;

uniform mat4 modelToWorld;
uniform mat4 cameraToView;

out VS_OUT
{
	float radius;
	float minAngle;
	float maxAngle;
} vs_out;

void main()
{
    gl_Position = cameraToView * modelToWorld * vec4(origin, 1.0, 1.0);
	vs_out.radius = radius;
	vs_out.minAngle = minAngle;
	vs_out.maxAngle = maxAngle;
}
