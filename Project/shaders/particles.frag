#version 330 core
in vec3 ParticleColor; // input color from vertex shader

out vec4 FragColor; // final color output

void main()
{
    FragColor = vec4(ParticleColor, 1.0);
}
