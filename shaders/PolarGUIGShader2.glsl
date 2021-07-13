#version 450 core
layout (points) in;
layout (line_strip, max_vertices = 42) out;



in VS_OUT {
	float radius;
	float minAngle;
	float maxAngle;
} gs_in[]; 

out GS_OUT{
	vec4 fColor;
} gs_out;

void main() {
	float PI = 3.1415926535897932384626433832795;
	float PI_2 = 1.57079632679489661923;
	float PI_4 = 0.785398163397448309616;
	float TWO_PI = 6.28318530717958647692528676;
	float xPos = gl_in[0].gl_Position[0];
	float yPos = gl_in[0].gl_Position[1];
	float zPos = gl_in[0].gl_Position[2];
	
	float radius = 0.5;
	
	gl_Position = gl_in[0].gl_Position; //Origin
	gs_out.fColor = vec4(1.0, 1.0, 1.0, 1.0);
	EmitVertex();
	
	//gl_Position = vec4(xPos + gs_in[0].radius * cos(gs_in[0].minAngle), yPos + gs_in[0].radius * sin(gs_in[0].minAngle), zPos, 1.0);
	gl_Position = vec4(xPos + radius * cos(PI_2), yPos + radius * sin(PI_2), zPos, 1.0);
	gs_out.fColor = vec4(1.0, 1.0, 1.0, 1.0);
	EmitVertex();
	
	gl_Position = gl_in[0].gl_Position; //Origin
	gs_out.fColor = vec4(1.0, 1.0, 1.0, 1.0);
	EmitVertex();
	
	//gl_Position = vec4(xPos + gs_in[0].radius * cos(gs_in[0].maxAngle), yPos + gs_in[0].radius * sin(gs_in[0].maxAngle), zPos, 1.0);
	gl_Position = vec4(xPos + radius * cos(PI_4), yPos + radius * sin(PI_4), zPos, 1.0);
	gs_out.fColor = vec4(1.0, 1.0, 1.0, 1.0);
	EmitVertex();
	EndPrimitive();
	/*
	for (int i = 0; i < 36; i++) {
		gl_Position = vec4(xPos + gs_in[0].radius * cos(i * TWO_PI / 36.0), yPos + gs_in[0].radius * sin(i * TWO_PI / 36.0), zPos, 1.0);
		gs_out.fColor = vec4(0.0, 0.0, 1.0, 1.0);
		EmitVertex();
	}
	
	EndPrimitive();*/
}