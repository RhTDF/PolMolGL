#version 450 core
layout (points) in;
layout (triangle_strip, max_vertices = 9) out;
uniform vec3 colVec;


in VS_OUT
{
	vec4 vcolor;
} gs_in[];

out GS_OUT{
	vec4 fColor;
} gs_out;

void main() {
	
	gl_Position = gl_in[0].gl_Position + vec4(0.0, 0.0, 0.0, 0.0);  //1
	gs_out.fColor = gs_in[0].vcolor;
    EmitVertex();
    gl_Position = gl_in[0].gl_Position + vec4(-0.5, 0.5, 0.0, 0.0);  //2
        gs_out.fColor = vec4(0.0,0,0,1);
    EmitVertex();
	gl_Position = gl_in[0].gl_Position + vec4(0.5, 0.5, 0.0, 0.0);  //3
        gs_out.fColor = vec4(0.0,0,0.0,1);
    EmitVertex();
	gl_Position = gl_in[0].gl_Position + vec4(0.0, 0.0, 0.0, 0.0);  //1
	gs_out.fColor = gs_in[0].vcolor;
    EmitVertex();
	gl_Position = gl_in[0].gl_Position + vec4(0.5, -0.5, 0.0, 0.0);  //4
        gs_out.fColor = vec4(0,0,0.0,1);
    EmitVertex();
	gl_Position = gl_in[0].gl_Position + vec4(0.0, 0.0, 0.0, 0.0);  //1
	gs_out.fColor = gs_in[0].vcolor;
    EmitVertex();
	gl_Position = gl_in[0].gl_Position + vec4(-0.5, -0.5, 0.0, 0.0);  //5
        gs_out.fColor = vec4(0.0,0.0,0,1);
    EmitVertex();
	gl_Position = gl_in[0].gl_Position + vec4(0.0, 0.0, 0.0, 0.0);  //1
	gs_out.fColor = gs_in[0].vcolor;
    EmitVertex();
	gl_Position = gl_in[0].gl_Position + vec4(-0.5, 0.5, 0.0, 0.0);  //2
        gs_out.fColor = vec4(0.0,0,0,1);
    EmitVertex();

    EndPrimitive();
}  
