#pragma once


#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include <ctime>
#include <iostream>
#include <random>
#include <vector>
#include <list>

#include <cmath>
#include "mathDefines.h"
#include "particle.h"
#include <unordered_map>

class Grid {

public:

	float cellSize;
	float bboxHeight;
	float bboxWidth;
	float bboxLength;

	int mapSize;

	

	/* Constructor*/
	Grid();
	Grid(float cellSize, int mapSize, glm::vec3 bboxDim);

	/*updateParticlePosition*/
	void addParticle(int index, glm::vec3 particlePosition);
	void clearMap();

	std::list<int> getNeighbours(int particleIndex, glm::vec3 particlePosition, float searchRadius);
	
	int hashFunction(int i, int j, int k);
private:
	/* hashmap */
	std::unordered_map<int, std::list<int>> hashmap;



};