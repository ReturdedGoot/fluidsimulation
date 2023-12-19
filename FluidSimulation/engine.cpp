#include "engine.h"
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>


Engine::Engine(std::vector<GLfloat> vertices, std::vector<GLuint> indices, int numParticles) {
	this->numParticles = numParticles;
	this->Vertices = vertices;
	this->Indices = indices;
	
	for (int i = 0; i < numParticles; i++) {
		//add a model view matrix for each particle. position the particle accordingly.
		Particle particle = Particle(glm::vec3(0.0f, 0.0f, 0.0f+(float)i));

		//update the model matrix
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, particle.Position);

		this->ModelMatrices.push_back(model);
		this->Particles.push_back(particle);
	}

};




void Engine::updatePosition() {
	for (int i = 0; i < numParticles; i++) {
		this->ModelMatrices[i] = glm::translate(this->ModelMatrices[i], this->Particles[i].Position);
	}
}


/*
Run's the main code for the particle positions. 
*/
void Engine::update(float deltaTime) {
	glm::vec3 velocity = glm::vec3(0.0f, -1.0f, 0.0f) * 0.01f * deltaTime;
	//add this velocity to the poisition vector
	for (Particle &particle : this->Particles) {

		particle.Position = particle.Position + velocity;
	}


	//we want to update the model view matrix as well. 
	updatePosition();
}