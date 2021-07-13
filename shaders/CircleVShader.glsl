
#version 450 core
layout(location = 0) in vec3 vertex;
layout(location = 1) in vec2 value;
uniform mat4 modelToWorld;
uniform mat4 cameraToView;
uniform mat4 worldToCamera;
out vec2 val;

void main() {
    val = value;
    gl_Position = cameraToView * worldToCamera * modelToWorld * vec4(vertex, 1.0);
}

