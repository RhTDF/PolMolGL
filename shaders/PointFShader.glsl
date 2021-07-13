#version 450 core

in VS_OUT
{
	vec4 vcolor;
} fs_in;

out vec4 color;

void main()
{
	color = fs_in.vcolor;
}
