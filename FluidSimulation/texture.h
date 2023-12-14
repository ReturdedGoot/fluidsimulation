#ifndef TEXTURE_CLASS_H
#define TEXTURE_CLASS_H

#include <glad/glad.h>

class Texture {
public:
	GLuint texture;
	int height, width, nrChannels;
	unsigned char* data;
	Texture(const char* filename);

	void Bind();
	



};




#endif