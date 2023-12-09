#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 tex_coord;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitangent;

out vec3 v_frag_coord;
out vec3 v_normal;
uniform mat4 M;
uniform mat4 itM;
uniform mat4 V;
uniform mat4 P;


void main(){
	vec4 frag_coord = M*vec4(position, 1.0);
	gl_Position = P*V*frag_coord;
	v_normal = vec3(itM * vec4(normal, 1.0));
	v_frag_coord = frag_coord.xyz;
}