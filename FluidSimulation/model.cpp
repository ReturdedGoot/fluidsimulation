#include "model.h"


Model::Model(std::vector<float> vertices, std::vector<int> indices, int numParticles) {
	this->numParticles = numParticles;
	this->Vertices = vertices;
	this->Indices = indices;
	
	for (int i = 0; i < numParticles; i++) {
		//add a model view matrix for each particle. position the particle accordingly.
		Particle particle = Particle(glm::vec3(0.0f, 0.0f, 0.0f));
		glm::mat4 model = glm::mat4(1.0f);
		this-> modelMatrices.append
	}

};

