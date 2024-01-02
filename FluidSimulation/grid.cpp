#include "grid.h"


Grid::Grid() {
}

/*
Description:
	Constructor for Grid Object. 

Input: size of cell, size of the hashmap and the dimensions of the boundingbox.
*/
Grid::Grid(float cellSize, int mapSize, glm::vec3 bboxDim) {
	this->cellSize = cellSize;
	this->mapSize = mapSize;

	this->bboxWidth = bboxDim.x;
	this->bboxHeight = bboxDim.y;
	this->bboxLength = bboxDim.z;

}


/*

Description:
	Function adds the particle to the unordered map hash map based on the particles position.
	Particles closer together typically endup closer in the hashmap.

Input: particle index and the particles spatial position.
output: None


*/
void Grid::addParticle(int index, glm::vec3 particlePosition) {
	// Gives cell position of particle.
	int i = floor((particlePosition.x + bboxWidth) / cellSize);
	int j = floor((particlePosition.y + bboxHeight)/ cellSize);
	int k = floor((particlePosition.z + bboxLength) / cellSize);

	//Compute hash of cell indices
	int hash = hashFunction(i, j, k);
	//Add the index into the hashmap.
	hashmap[hash].push_back(index);
}

/*
Description:
	Gets the neighbouring particles indices based on the particles position.

Input: Particle index and particle position
Output: List of all neighbouring particles within atleast distance <= cellSize*2.
*/
std::list<int> Grid::getNeighbours(int particleIndex, glm::vec3 particlePosition, float searchRadius) {

	// Gives cell position of particle.
	int i = floor((particlePosition.x + bboxWidth) / cellSize);
	int j = floor((particlePosition.y + bboxHeight) / cellSize);
	int k = floor((particlePosition.z + bboxLength) / cellSize);

	//Compute the minimum and maximum cell positions to search. 
	int minX = floor((particlePosition.x + bboxWidth - searchRadius) / cellSize);
	int minY = floor((particlePosition.y + bboxHeight - searchRadius)/ cellSize);
	int minZ = floor((particlePosition.z + bboxLength - searchRadius) / cellSize);

	int maxX = ceil((particlePosition.x + bboxWidth + searchRadius) / cellSize);
	int maxY = ceil((particlePosition.y + bboxHeight + searchRadius) / cellSize);
	int maxZ = ceil((particlePosition.z + bboxLength + searchRadius) / cellSize);

	std::list<int> neighbourIndices;

	//We loop over the neighbouring cell values within that search radius. 
	for (int x = minX; x <= maxX; x++) {
		for (int y = minY; y <= maxY; y++) {
			for (int z = minZ; z <= maxZ; z++) {

				//Compute the cell's hash function
				int hash = hashFunction(x, y, z);
				
				//add all indices in cell to neighbour index function.
				for (int neighbourIndex : hashmap[hash]) {
					neighbourIndices.push_back(neighbourIndex);
				}
			}
		}
	}
	
	return neighbourIndices;
}

/*
Description:
	Function is called upon every new timestep. The hashmap is cleared to update
	particle positions. 

Input: None
Output: None
*/
void Grid::clearMap() {
	hashmap.clear();
}

/*

Description:
	Function computes the hash value given the cell indices.

Input: i, j, k are indices to the cell
Output: a hashed index value given the cell values. 


*/
int Grid::hashFunction(int i, int j, int k) {
	int p1 = 73856093;
	int p2 = 19349663;
	int p3 = 83492791;

	return ((p1 * i) ^ (p2 * j) + (p3 * k)) % mapSize;
}