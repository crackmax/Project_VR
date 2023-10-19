#version 330 core
in vec3 position; 
in vec2 texcoord; 
out vec2 uv; 
	
uniform mat4 M;
uniform mat4 V; 
uniform mat4 P;

void main(){ 
	gl_Position = P*V*M*vec4(position, 1);
	uv = texcoord; 
}