#version 430
layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 colour;
layout(location = 2) in vec3 startPoint;
out vec4 vColor;

uniform mat4 modelToWorld;
uniform mat4 worldToCamera;
uniform mat4 cameraToView;
uniform int contextVal;
uniform bool flatten;

void main()
{
    if (flatten) gl_Position = cameraToView * worldToCamera * modelToWorld * vec4(pos[0]-startPoint[0],pos[1]-startPoint[1],1.0, 1.0);
    else gl_Position = cameraToView * worldToCamera * modelToWorld * vec4(pos-startPoint, 1.0);
    if (contextVal == 1) vColor = vec4(0.3,0.3,0.3, pos[2]/10.0 - 0.1);
    else vColor = vec4(colour, 1.0);
}
