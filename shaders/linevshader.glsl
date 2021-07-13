#version 430
layout(location = 0) in vec3 pos;
out vec4 vColor;

uniform mat4 modelToWorld;
uniform mat4 worldToCamera;
uniform mat4 cameraToView;
uniform vec4 col;

void main()
{
    gl_Position = cameraToView * worldToCamera * modelToWorld * vec4(pos[0],pos[1],pos[2], 1.0);
	vColor = col;
	//vColor = vec4(0.8, 0.8, 0.8, 0.9);
}
