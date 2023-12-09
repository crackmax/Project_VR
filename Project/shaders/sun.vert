#version 330 core
in vec3 position;
in vec2 tex_coord;
in vec3 normal;

out vec2 f_tex_coord;
out vec3 FragPos;
out vec3 Normal;

uniform mat4 M;
uniform mat4 itM;
uniform mat4 V;
uniform mat4 P;

void main() {
    FragPos = vec3(M * vec4(position, 1.0));
    gl_Position = P * V * vec4(FragPos, 1.0);

    // Transform normals correctly
    Normal = normalize(vec3(itM * vec4(normal, 1.0)));

    // Pass texture coordinates to fragment shader
    f_tex_coord = tex_coord;
}
