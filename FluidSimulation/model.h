#pragma once
#include<vector>
#include<glm/glm.hpp>
#include "particle.h"



class Model {

public:
	std::vector<float> Vertices;
	std::vector<int>  Indices;

	int numParticles;

	std::vector<glm::mat4> ModelMatrices;
	std::vector<Particle> Particles;

	Model(std::vector<float> vertices, std::vector<int> indices, int numParticles);
};