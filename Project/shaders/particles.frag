#version 330 core

in vec3 ParticleColor;

out vec4 FragColor;

void main() {
    FragColor = vec4(ParticleColor, 1.0);
}
