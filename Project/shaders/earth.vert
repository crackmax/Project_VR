#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 tex_coord;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitangent;

out vec2 f_tex_coord;
out vec3 v_frag_coord;
out vec3 v_normal;
out mat3 TBN;

uniform mat4 M;
uniform mat4 itM;
uniform mat4 V;
uniform mat4 P;

void main() {
    vec4 frag_coord = M * vec4(position, 1.0);
    gl_Position = P * V * frag_coord;
    vec3 T = normalize(vec3(M * vec4(tangent, 0.0)));
    vec3 B = normalize(vec3(M * vec4(bitangent, 0.0)));
    vec3 N = normalize(vec3(M * vec4(normal, 0.0)));
    TBN = mat3(T, B, N);
    v_normal = normalize(vec3(itM * vec4(normal, 1.0)));

    // Pass texture coordinates to fragment shader
    f_tex_coord = tex_coord;

    // Pass fragment position to fragment shader
    v_frag_coord = frag_coord.xyz;
}
