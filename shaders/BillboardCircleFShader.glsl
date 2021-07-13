#version 450 core

uniform sampler2D tex0;

in vec2 Vertex_UV;
in vec4 Vertex_Color;
flat in int Vertex_ID;
out vec4 FragColor;
void main (void)
{
        vec2 uv = Vertex_UV.xy;
        vec2 orig = vec2(0.5, 0.5);
        if(distance(uv,orig) > 0.5) discard;
        FragColor =  vec4(vec3((1-distance(uv,orig)) * Vertex_Color), 1.0);
}


