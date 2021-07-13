#version 450 core

layout (location = 0) in vec3 gxl3d_Color;
layout (location = 1) in vec3 gxl3d_Position;

uniform mat4 gxl3d_ModelViewMatrix;
uniform int contextVal;

out Vertex
{
  vec4 color;
} vertex;

void main()
{
  gl_Position = gxl3d_ModelViewMatrix * vec4(gxl3d_Position, 1.0);
  if (contextVal == 1) vertex.color = vec4(0.7,0.7,0.7, 0.65);
  else vertex.color = vec4(gxl3d_Color, 1.0);
}
