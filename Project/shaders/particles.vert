#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
out vec3 ParticleColor;
uniform mat4 M;
uniform mat4 itM;
uniform mat4 V;
uniform mat4 P;
void main()
{
    vec3 FragPos = vec3(M * vec4(position, 1.0));
    gl_Position = P * V * vec4(FragPos, 1.0);
    ParticleColor = vec3(color.r,color.g, color.b);
}
