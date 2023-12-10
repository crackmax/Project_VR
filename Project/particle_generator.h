#ifndef PARTICLE_GENERATOR_H
#define PARTICLE_GENERATOR_H
#define _USE_MATH_DEFINES
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <cmath>
#include <vector>
#include "shader.h"
struct Particle {
	float x, y, z;
	float vx, vy, vz;
	float lifetime;
	float r, g, b;
};

std::vector<Particle> particles;

const int numParticles = 600;
const float emissionRate = 0.05f;

void emitParticles() {
	for (int i = 0; i < numParticles; ++i) {
		if ((rand() / (float)RAND_MAX) < emissionRate) {
			Particle particle;

			float theta = 2.0f * M_PI * (rand() / (float)RAND_MAX);
			float phi = acos(2.0f * (rand() / (float)RAND_MAX) - 1.0f);

			// random spawn in sphere at center of sun
			particle.x = ((static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * 4.0f - 2.0f) * sin(phi) * cos(theta);
			particle.y = ((static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * 4.0f - 2.0f) * sin(phi) * sin(theta);
			particle.z = ((static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * 4.0f - 2.0f) * cos(phi);

			float speed = 0.001f;

			particle.vx = speed * sin(phi) * cos(theta);
			particle.vy = speed * sin(phi) * sin(theta);
			particle.vz = speed * cos(phi);

			particle.lifetime = 4.0f;
			particle.r = 1.0f;
			particle.g = 0.3f;
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
		particle.r -= 0.01f;
		particle.g -= 0.005f;
		particle.lifetime -= 0.005f;
	}
	// Remove particle if they died
	particles.erase(std::remove_if(particles.begin(), particles.end(),
		[](const Particle& p) { return p.lifetime <= 0.0f; }),
		particles.end());
}

void renderParticles(Shader shader) {
	GLuint particleVAO, particleVBO;

	glGenVertexArrays(1, &particleVAO);
	glBindVertexArray(particleVAO);

	glGenBuffers(1, &particleVBO);
	glBindBuffer(GL_ARRAY_BUFFER, particleVBO);
	// gives particles data, total size of data in bytes that is passed: particles.size() * sizeof(Particle)
	glBufferData(GL_ARRAY_BUFFER, particles.size() * sizeof(Particle), particles.data(), GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)offsetof(Particle, x));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)offsetof(Particle, r));


	glDrawArrays(GL_POINTS, 0, particles.size());

	// Cleanup because otherwise memory would be full fast
	glDeleteVertexArrays(1, &particleVAO);
	glDeleteBuffers(1, &particleVBO);
}

#endif