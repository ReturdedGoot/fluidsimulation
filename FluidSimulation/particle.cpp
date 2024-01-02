#include "particle.h"


Particle::Particle(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f)) {

	this->Position = position;
};


glm::vec3 Particle::getColor() {
	float speed = glm::length(Velocity);

	float cutoff = 1.0;
	float val = fmin( abs(speed)/cutoff , 1.0);
	

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