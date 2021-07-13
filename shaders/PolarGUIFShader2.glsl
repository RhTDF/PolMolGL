#version 450 core
out vec4 FragColor;

in GS_OUT{
	vec4 fColor;
} fs_in;  

void main()
{
    FragColor = fs_in.fColor;  
}