#version 430
layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 colour;
out vec4 vColor;

uniform mat4 modelToWorld;
uniform mat4 worldToCamera;
uniform mat4 cameraToView;
uniform int contextVal;

void main()
{
    gl_Position = cameraToView * worldToCamera * modelToWorld * vec4(pos, 1.0);
        if (contextVal == 1) vColor = vec4(0.7,0.7,0.7, 0.65);
        else vColor = vec4(colour, 1.0);

}
