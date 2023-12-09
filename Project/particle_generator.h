#ifndef PARTICLE_GENERATOR_H
#define PARTICLE_GENERATOR_H
#define _USE_MATH_DEFINES
#include <GLFW/glfw3.h>
#include <cmath>
#include <vector>
struct Particle {
    float x, y, z;      // Position
    float vx, vy, vz;   // Velocity
    float lifetime;     // Time until particle dies
    float r, g, b;      // Color
};

std::vector<Particle> particles;

const float sphereRadius = 1.0f;
const int numParticles = 1000;
const float emissionRate = 0.1f;
const float gravity = -0.005f;

void emitParticles() {
    for (int i = 0; i < numParticles; ++i) {
        if ((rand() / (float)RAND_MAX) < emissionRate) {
            float theta = 2.0f * M_PI * (rand() / (float)RAND_MAX);
            float phi = acos(2.0f * (rand() / (float)RAND_MAX) - 1.0f);

            Particle particle;
            particle.x = sphereRadius * sin(phi) * cos(theta);
            particle.y = sphereRadius * sin(phi) * sin(theta);
            particle.z = sphereRadius * cos(phi);

            particle.vx = 0.02f * ((rand() / (float)RAND_MAX) - 0.5f);
            particle.vy = 0.05f * ((rand() / (float)RAND_MAX) + 0.5f);
            particle.vz = 0.02f * ((rand() / (float)RAND_MAX) - 0.5f);

            particle.lifetime = 1.0f;
            particle.r = 1.0f;
            particle.g = 0.5f * (rand() / (float)RAND_MAX);
            particle.b = 0.0f;

            particles.push_back(particle);
        }
    }
}

void updateParticles() {
    for (auto& particle : particles) {
        particle.x += particle.vx;
        particle.y += particle.vy;
        particle.z += particle.vz;
        particle.vy += gravity;
        particle.lifetime -= 0.01f;
    }

    // Remove dead particles
    particles.erase(std::remove_if(particles.begin(), particles.end(),
        [](const Particle& p) { return p.lifetime <= 0.0f; }),
        particles.end());
}

void renderParticles() {
    glBegin(GL_POINTS);
    for (const auto& particle : particles) {
        glColor3f(particle.r, particle.g, particle.b);
        glVertex3f(particle.x, particle.y, particle.z);
    }
    glEnd();
}
#endif