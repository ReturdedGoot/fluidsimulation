#pragma once
#include<vector>
#include<glm/glm.hpp>
#include "particle.h"
#include <glad/glad.h>


class Engine {

public:
	std::vector<GLfloat> Vertices;
	std::vector<GLuint>  Indices;

	int numParticles;

	std::vector<glm::mat4> ModelMatrices;
	std::vector<Particle> Particles;

	Engine(std::vector<GLfloat> vertices, std::vector<GLuint> indices, int numParticles);

	void update(float deltaTime);
	void updatePosition();
};