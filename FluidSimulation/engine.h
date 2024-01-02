#pragma once
#include<vector>
#include<glm/glm.hpp>
#include <glad/glad.h>
#include <list>

#include "mathDefines.h"
#include "particle.h"
#include "grid.h"


#define K 8.31446261815324	


class Engine {

public:
	std::vector<GLfloat> Vertices;
	std::vector<GLuint>  Indices;

	std::vector<glm::mat4> ModelMatrices;

	/* Simulation Objects*/
	std::vector<Particle> Particles;
	int frame;

	int numParticles;
	float particleSize;
	float particleSpacing;
	float particleMass;

	float dampingCoefficient;
	float kernelRadius;
	float restDensity;
	float pressureMultiplier;
	float viscosity;
	float gravitationalAcceleration;

	float bboxHeight;
	float bboxWidth;
	float bboxLength;
	float gridSize;

	bool hasStarted = false;

	float maxSpeed = 0.0;
	float maxPressureForce = 0.0;

	Grid gridObject;

	std::vector<std::list<int>> neighbourList;
	std::vector<std::vector<std::vector<std::list<int>>>> grid;
	int numGridLength;
	int numGridWidth;
	int numGridHeight;

	Engine(std::vector<GLfloat> vertices, std::vector<GLuint> indices, int numParticles, float particleSize, glm::vec3 bboxDim);

	void update(float deltaTime);
	void arrangeParticles();
	void updatePosition();

	void updateBoundingBox(float length, float width, float height);
	glm::vec3 getColor(int index);
	glm::vec3 getColor2(float speed);

private:
	void resolveCollisions(float deltaTime);
	void boundaryCollisions(float deltaTime);
	void particleCollisions(float deltaTime);
	void particleCollisions2(float deltaTime);

	float getDensity(int index);

	void updateDensityPressure(float deltaTime);
	void updatePressureForces(float deltaTime);
	void updateViscosityForces(float deltaTime);

	std::list<int> getNeighbours(int index);
	std::list<int> findNeighbours(int i);
	glm::vec3 getGridPosition(glm::vec3 position);
	void gridCreation();
};