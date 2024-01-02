#include "engine.h"
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include <ctime>
#include <iostream>
#include <random>
#include <vector>
#include <list>
#include <cmath>


float sign(float val) {
	if (val < 0.0) {
		return -1.0;
	}
	return 1.0;
}

float random() {

	int val = rand();
	int max = RAND_MAX;
	return ((float) (val)) / ((float) (max));

}


Engine::Engine(std::vector<GLfloat> vertices, std::vector<GLuint> indices, int numParticles, float particleSize, glm::vec3 bboxDim) {
	this->numParticles = numParticles;
	this->Vertices = vertices;
	this->Indices = indices;
	this->particleSize = particleSize; //The particle size is the particle radius. 
	this->particleSpacing = 2.1*particleSize;
	
	srand((int) time(0));
	
	this->bboxWidth = bboxDim.x;
	this->bboxHeight = bboxDim.y;
	this->bboxLength = bboxDim.z;
	this->dampingCoefficient = 0.9;
	this->kernelRadius = particleSize * 5;
	this->pressureMultiplier = K;
	this->viscosity = 0.01;
	this->gravitationalAcceleration = 9.81;
	this->frame = 0;
	this->particleMass = 1.0f;
	this->gridSize = kernelRadius;

	std::default_random_engine generator;
	std::uniform_real_distribution<float> distribution(0.0, 1.0);
	
	for (int i = 0; i < numParticles; i++) {
		//add a model view matrix for each particle. position the particle accordingly.


	


		Particle particle = Particle(glm::vec3(0.0f, 0.0f, 0.0f));


		particle.Velocity = glm::vec3(0, 0, 0);
		particle.Acceleration = glm::vec3(0.0, 0.0, 0.0);
		particle.mass = particleMass;

		//update the model matrix
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(particleSize));
		model = glm::translate(model, particle.Position);

		this->ModelMatrices.push_back(model);
		this->Particles.push_back(particle);
	}

	arrangeParticles();



};


void Engine::arrangeParticles() {
	//Check if particle array and modelmatrix array have the same size as newly update numParticles.
	int diffParticles = numParticles - Particles.size();
	if (diffParticles < 0) {
		//we want to remove x amount of particles
		
		Particles.erase(Particles	.begin(), Particles.begin() - diffParticles);
		ModelMatrices.erase(ModelMatrices.begin(), ModelMatrices.begin() - diffParticles);
	}
	else if (diffParticles > 0) {
		for (int i = 0; i < diffParticles; i++) {

			Particle particle = Particle(glm::vec3(0.0f, 0.0f, 0.0f));
			particle.mass = particleMass;
			particle.Velocity = glm::vec3(0, 0, 0);
			particle.Acceleration = glm::vec3(0.0, 0.0, 0.0);

			Particles.push_back(particle);
			// we want to add to the model matrix as well;s

			glm::mat4 model = glm::mat4(1.0f);
			model = glm::scale(model, glm::vec3(particleSize));
			model = glm::translate(model, particle.Position);

			ModelMatrices.push_back(model);
		}
	}




	//we want to rearrange all the particles and their positions into 3d grids. 
	int number_per_dimension = int(round(pow(numParticles, 1.0 / 3.0)));

	if (pow(number_per_dimension, 3.0) < numParticles) {
		//we add an extra dimension
		number_per_dimension = number_per_dimension + 1;
	}

	//we want to generate a list of vec3's containing positions and assign them to the particles.
	std::vector<glm::vec3> positionList;
	glm::vec3 centerOfMass = glm::vec3(0.0, 0.0, 0.0);
	float totalMass = 0.0;

	for (int i = 0; i < number_per_dimension; i++) {
		for (int j = 0; j < number_per_dimension; j++) {
			for (int k = 0; k < number_per_dimension; k++) {

				float ex = 0.1 * particleSpacing * random();
				float ey = 0.1 * particleSpacing * random();
				float ez = 0.1 * particleSpacing * random();

				glm::vec3 position = glm::vec3(i * particleSpacing + ex, j * particleSpacing + ey, k * particleSpacing + ez);
				centerOfMass = centerOfMass + position;
				
				positionList.push_back(position);
			}
		}
	}

	//find the com. 
	centerOfMass = (1.0f/ (float)(numParticles)) * centerOfMass;
	//we want to shift all the positions in the position list by center of mass. 



	//we confirm that the positionList is the same size as the number of particles
	std::cout << "Particle List size " << Particles.size() << std::endl;
	std::cout << "Position List size " << positionList.size() << std::endl;

	//we now give each particle a position
	for (int i = 0; i < numParticles; i++) {
		Particles[i].Position = positionList[i];
	}

	updatePosition();
}




void Engine::updatePosition() {
	for (int i = 0; i < numParticles; i++) {

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, this->Particles[i].Position);
		model = glm::scale(model, glm::vec3(particleSize));

		this->ModelMatrices[i] = model;
	}
}


void Engine::updateBoundingBox(float length, float width, float height) {
	bboxLength = length;
	bboxWidth = width;
	bboxHeight = height;
}


glm::vec3 Engine::getColor2(float speed) {

	float fraction = (float)(speed) / (float)(255.0);
	float cutoff = 1.0;
	float val = fmin(abs(fraction) / cutoff, 1.0);


	//now we do the color interpolation
	float rgb[] = { 0, 0, 0 };


	rgb[0] = val;
	rgb[1] = 1 - val;
	

	return glm::vec3(rgb[0], rgb[1], rgb[2]);

}

glm::vec3 Engine::getColor(int index) {

	float fraction = (float)(index) / (float)(numParticles);
	float cutoff = 1.0;
	float val = fmin(abs(fraction) / cutoff, 1.0);


	//now we do the color interpolation
	float rgb[] = { 0, 0, 0 };

	if (val <= 0.5f) {
		//we want to interpolate between b and g values. 
		rgb[1] = val;
		rgb[2] = 1 - val;
	}
	else {
		rgb[0] = val;
		rgb[1] = 1 - val;
	}

	return glm::vec3(rgb[0], rgb[1], rgb[2]);

}

void Engine::particleCollisions(float deltaTime) {
	
	//for each particle we want to check the other particles and then reflect in the other way

	for (int i = 0; i < numParticles; i++) {
		for (int j = i + 1; j < numParticles; j++) {
			Particle &p1 = Particles[i];
			Particle &p2 = Particles[j];

			glm::vec3 displacement = p2.Position - p1.Position;

			if (glm::length(displacement) <= 2 * particleSize) {
				//There is a collision event and we want to reflect the particles. Swap their velocities.
				glm::vec3 tempVelocity = p2.Velocity;
				p2.Velocity = p1.Velocity;
				p1.Velocity = tempVelocity;
			}

		}
	}

}


/*
Poly 6 Kernel function - General usage
*/
 float poly6kernel(glm::vec3 distance, float radius) {

	float kernel = 0;
	if (glm::length(distance) < radius) {
		kernel = pow(pow(radius, 2.0) - pow(glm::length(distance), 2.0), 2.0);
	}
	float normalizationFactor = 315.0 / (64.0 * M_PI * pow(radius, 9.0));
	
	return kernel*normalizationFactor;
}

 /*
	Spiky function  - For Pressure calculations.
 */
 float spikyKernel(glm::vec3 distance, float radius) {

	 float kernel = 0;
	 if (glm::length(distance) < radius) {
		 kernel = pow(radius - glm::length(distance) ,3.0);
	 }
	 float normalizationFactor = 15.0 / ( M_PI * pow(radius, 6.0));

	 return kernel * normalizationFactor;
 }	


 /*
	Spiky function  - For Pressure calculations.
 */
 float spikyKernelDerivative(glm::vec3 distance, float radius) {

	 float kernel = 0;
	 if (glm::length(distance) < radius) {
		 kernel = pow(radius - glm::length(distance), 2.0);
	 }
	 float normalizationFactor = (15.0*3.0) / (M_PI * pow(radius, 6.0));

	 return kernel * normalizationFactor;
 }





 /*
	Viscosity function  - For viscosity calculations.
 */
 float viscosityKernalDerivative(glm::vec3 displacement, float radius) {
	 float kernel = 0;
	 float distance = glm::length(displacement);
	 if (distance < radius) {
		 kernel = distance - radius;
	 }
	 
	 float normalizationFactor = (45) / (M_PI * pow(radius, 6.0));

	 return kernel * normalizationFactor;

 }




 float Engine::getDensity(int index) {

	 float density = 0;
	 Particle& p1 = Particles[index];

	 for (int i : getNeighbours(index)) {
		 Particle& p2 = Particles[i];
		 density += p1.mass * poly6kernel(p1.predictedPosition - p2.Position, kernelRadius);
	 }

	 //if (density < 1e-9)
	 	 //density = 1e-9;
	 return density;	
 }



/*
	We attempt to use the particle-based fluid interactions paper in order to make the fluid simulation realistic. 
*/
void Engine::particleCollisions2(float deltaTime) {
		//for each particle we want to check the other particles and then reflect in the other way

	for (int i = 0; i < numParticles; i++) {
		for (int j = i + 1; j < numParticles; j++) {
			Particle& p1 = Particles[i];
			Particle& p2 = Particles[j];

			glm::vec3 displacement = p2.Position - p1.Position;

			float distance = glm::length(displacement);


			if (distance <= 2 * particleSize) {
				
				glm::vec3 displacementNormal = glm::normalize(displacement);
				
				//move the particles out of eachother. 
				p1.Position = p1.Position -  0.5f * (2*particleSize - distance) * displacementNormal;
				p2.Position = p2.Position + 0.5f * (2 * particleSize - distance) * displacementNormal;

				//There is a collision event and we want to reflect the particles. Swap their velocities.



				glm::vec3 tempVelocity = p2.Velocity;
				p2.Velocity = p1.Velocity;
				p1.Velocity = tempVelocity;
			}

			// Move the particles out of each other? 
			
		}
	}

}





void Engine::boundaryCollisions(float deltaTime) {

	//if we have collided with the bounding box at any location, we want to reverse the velocity in that specific direction. 
	for (Particle& particle : this->Particles) {

		if (abs(particle.Position.x) > bboxWidth / 2.0) {
			//then we want to position the particle back into the box;
			particle.Position.x = (bboxWidth / 2.0) * sign(particle.Position.x);
			particle.Velocity.x = -particle.Velocity.x * dampingCoefficient;
		}

		if (abs(particle.Position.y) > bboxHeight / 2.0) {
			particle.Position.y = (bboxHeight / 2.0) * sign(particle.Position.y);
			particle.Velocity.y = -particle.Velocity.y * dampingCoefficient;
		}

		if (abs(particle.Position.z) > bboxLength / 2.0) {
			particle.Position.z = (bboxLength / 2.0) * sign(particle.Position.z);
			particle.Velocity.z = -particle.Velocity.z * dampingCoefficient;
		}

	}
}

void Engine::resolveCollisions(float deltaTime) {

	//particleCollisions2(deltaTime);
	boundaryCollisions(deltaTime);
}



void Engine::updateDensityPressure(float deltaTime) {




	for (int i = 0; i < numParticles; i++) {

		Particle& particle = Particles[i];
		particle.density = getDensity(i);

		//We can use a different pressure multiplier if we want
		float pressure = pressureMultiplier * (particle.density - particle.restDensity);
		//now we calculate the pressure force
		particle.pressure = pressure;
	}
}

void Engine::updatePressureForces(float deltaTime) {

	for (int i = 0; i < numParticles; i++) {
		//calculate the 
		glm::vec3 force = glm::vec3(0.0, 0.0, 0.0);
		Particle& p1 = Particles[i];

		//sum 
		for (int j : getNeighbours(i)) {

			if (i == j) {
				continue;
			}
			
			if (p1.density <= 0.0) {
				continue;
			}

			Particle& p2 = Particles[j];
			if (p2.density <= 0.0) {
				continue;
			}

			glm::vec3 dir = glm::normalize(p1.predictedPosition - p2.predictedPosition);
			glm::vec3 curForce = dir * p2.mass * ((p1.pressure + p2.pressure) / (2 * p2.density)) * spikyKernelDerivative(p1.predictedPosition - p2.predictedPosition, kernelRadius);
			force -= curForce;
			//add the pressure force to array
		}
		p1.pressureForce = force;
	}
}

void Engine::updateViscosityForces(float deltaTime) {

	for (int i = 0; i < numParticles; i++) {
		glm::vec3 force = glm::vec3(0.0, 0.0, 0.0);

		Particle& p1 = Particles[i];

		for (int j : getNeighbours(i)) {
			if (i == j) {
				continue;
			}

			Particle& p2 = Particles[j];

	

			if (p2.density <= 0.0) {
				continue;
			}


			//viscosity force calculations
			float viscosityKernel = viscosityKernalDerivative(p1.predictedPosition - p2.predictedPosition, kernelRadius);
			force -= viscosity * p2.mass * ((p2.Velocity - p1.Velocity) / p2.density) * viscosityKernel;
			float test = 0.5;
		}
		p1.viscosityForce = force;
	}
}


/*
Generating grid box
*/


glm::vec3 Engine::getGridPosition(glm::vec3 position) {
	//we want to get the coordinates relative to the boxes.
	int xpos = glm::floor((position.x + (bboxWidth/2)) /(gridSize));	//x is width
	int ypos = glm::floor((position.y + (bboxHeight / 2)) / (gridSize));// y is height
	int zpos = glm::floor((position.z + (bboxLength / 2)) / (gridSize)); //z is length


	//do a check hear
	if (xpos > numGridWidth  - 1) 
		xpos = numGridWidth  - 1;
	if (ypos > numGridHeight - 1)
		ypos = numGridHeight - 1;
	if (zpos > numGridLength - 1)
		zpos = numGridLength - 1;

	return glm::vec3(xpos, ypos, zpos);
}


void Engine::gridCreation() {

	
	//divide the 3 axis by kernel size and round up
	numGridLength = std::ceil(bboxLength/gridSize);
	numGridWidth = std::ceil(bboxWidth / gridSize);
	numGridHeight = std::ceil(bboxHeight / gridSize);



	std::vector<std::vector<std::vector<std::list<int>>>> gridlayout(numGridLength, std::vector<std::vector<std::list<int>>> (numGridWidth, std::vector<std::list<int>> (numGridHeight, std::list<int>()) ));
	grid = gridlayout;
	//for each of the particle assign it a grid.
	for (int i = 0; i < numParticles; i++) {
		Particle& particle = Particles[i];
		glm::vec3 gridPosition = getGridPosition(particle.Position);
		//we want to find

		//putting the index of the particle into the grid. 
		grid[gridPosition.z][gridPosition.x][gridPosition.y].push_back(i);		
	}

}


std::list<int> Engine::findNeighbours(int index) {
	Particle& particle = Particles[index];
	std::list<int> neighbours;
	//now we want to go through all the neighbours in the grid.
	/*
		visits a 3x3 grid around the particle. 
	*/
	glm::vec3 gridPos = getGridPosition(particle.Position);

	for (int x = std::max((int)gridPos.x - 1, 0); x <= std::min((int)gridPos.x + 1, numGridWidth-1); x++) {
		for (int y = std::max((int)gridPos.y - 1, 0); y <= std::min((int)gridPos.y + 1, numGridHeight - 1); y++) {
			for (int z = std::max((int)gridPos.y - 1, 0); z <= std::min((int)gridPos.z + 1, numGridLength - 1); z++) {
				//we want to visit all the particles in the grid cell.
				for (int neighbourIndex : grid[z][x][y]) {
					//add the neighbour index into the list of neighbours. 
					//we don't want to include our self. idiotic. 
					if (index == neighbourIndex)
						continue;
					neighbours.push_back(neighbourIndex);
				}

			}
		}
	}
	return neighbours;
}

std::list<int> Engine::getNeighbours(int index) {
	return neighbourList[index];
}

/*
Run's the main code for the particle positions.
*/
void Engine::update(float deltaTime) {

	
	deltaTime = 0.001;

	frame = frame + 1;

	if (!hasStarted) {
		// we need to modify this for rest densities. 
		for (Particle& particle : Particles) {
			particle.restDensity = 1.45;
		}
		hasStarted = true;
	}
	//create the grid
	gridCreation();
	//For each particle we want to construct a list and store it into another list. 
	std::vector<std::list<int>> particleNeighbours;
	
	//add each particles list of neighbours into this. 
	for (int i = 0; i < numParticles; i++) {
		particleNeighbours.push_back(findNeighbours(i));
	}

	neighbourList = particleNeighbours;


	//we use predictedPositions to calculate density and pressure forces. 
	for (Particle& particle : Particles) {
		
		glm::vec3 predictedVelocity = particle.Velocity + particle.Acceleration * deltaTime;
		particle.predictedPosition = particle.Position + predictedVelocity * deltaTime;
	}


	updateDensityPressure(deltaTime);
	updatePressureForces(deltaTime);
	updateViscosityForces(deltaTime);


	//adding gravity into the simulation

	for (int i = 0; i < numParticles; i++) {
		//get the pressure force
		Particle& particle = Particles[i];
		glm::vec3 gravity = Particles[i].mass* glm::vec3(0.0, -gravitationalAcceleration, 0.0);

		//add all internal forces
		glm::vec3 acceleration = glm::vec3(0.0, 0.0, 0.0);
		//add the pressure force to the acceleration
		if (particle.density > 0.0) {
			acceleration += (particle.pressureForce)/ particle.density;
		}
		//glm::vec3 resultantForce = particle.pressureForce + particle.density * gravity + particle.viscosityForce;
		particle.Acceleration = acceleration;


		

		//add this to the velocity
		particle.Velocity = particle.Velocity + particle.Acceleration * deltaTime;
		particle.Position = particle.Position + particle.Velocity * deltaTime;

		if (glm::length(particle.Velocity) > maxSpeed) {
			maxSpeed = glm::length(particle.Velocity);
		}

		if (glm::length(particle.pressureForce) > maxPressureForce) {
			maxPressureForce = glm::length(particle.pressureForce);
		}

	}


	//we want to update the model view matrix as well. 
	resolveCollisions(deltaTime);
	updatePosition();

}