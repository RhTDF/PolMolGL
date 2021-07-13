#version 450 core

layout (points) in;
layout (triangle_strip) out;
layout (max_vertices = 4) out;    
   
// GLSL Hacker automatic uniforms:
uniform mat4 gxl3d_ProjectionMatrix;

uniform float particle_size;

in Vertex
{
  vec4 color;
} vertex[];


out vec2 Vertex_UV;
out vec4 Vertex_Color;
flat out int Vertex_ID;
   
void main (void)
{
  vec4 P = gl_in[0].gl_Position;

  // a: left-bottom 
  vec2 va = P.xy + vec2(-0.5, -0.5) * particle_size;
  gl_Position = gxl3d_ProjectionMatrix * vec4(va, P.zw);
  Vertex_UV = vec2(0.0, 0.0);
  Vertex_Color = vertex[0].color;
  Vertex_ID = 0;
  EmitVertex();  
  
  // b: left-top
  vec2 vb = P.xy + vec2(-0.5, 0.5) * particle_size;
  gl_Position = gxl3d_ProjectionMatrix * vec4(vb, P.zw);
  Vertex_UV = vec2(0.0, 1.0);
  Vertex_Color = vertex[0].color;
  Vertex_ID = 1;
  EmitVertex();  
  
  // d: right-bottom
  vec2 vd = P.xy + vec2(0.5, -0.5) * particle_size;
  gl_Position = gxl3d_ProjectionMatrix * vec4(vd, P.zw);
  Vertex_UV = vec2(1.0, 0.0);
  Vertex_Color = vertex[0].color;
  Vertex_ID = 2;
  EmitVertex();  

  // c: right-top
  vec2 vc = P.xy + vec2(0.5, 0.5) * particle_size;
  gl_Position = gxl3d_ProjectionMatrix * vec4(vc, P.zw);
  Vertex_UV = vec2(1.0, 1.0);
  Vertex_Color = vertex[0].color;
  Vertex_ID = 3;
  EmitVertex();  

  EndPrimitive();  
}   
