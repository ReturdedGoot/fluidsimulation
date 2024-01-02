#pragma once

#include<glm/glm.hpp>
#include<vector>
#include <cmath>

class Particle {
public:
	glm::vec3 Position;
	glm::vec3 Velocity;
	glm::vec3 Acceleration;


	glm::vec3 pressureForce;
	glm::vec3 viscosityForce;


	glm::vec3 predictedPosition;


	float size;
	float mass;

	float density;
	float pressure;

	float restDensity;

	Particle(glm::vec3 position);
	glm::vec3 getColor();

};
