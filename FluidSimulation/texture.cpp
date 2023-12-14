#include<iostream>
#include "texture.h"
#include "stb_image.h"
#include<string.h>

Texture::Texture(const char* filename)
{
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	//set the texture wrapping and filtering. 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//Note: we use mipmap for minifying to prevent artificats. 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); //WHEN minifying the mipmap we use linear filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //when magnifying the mipmap we use linear filtering as well.

	stbi_set_flip_vertically_on_load(true);
	data = stbi_load(filename, &width, &height, &nrChannels, 0);
	if (data)
	{
		if ( (std::string(filename)).find(".png") != std::string::npos) {
			//file is a .png meaning it contains transperancy values
			//we want to use the RGBA option.
			std::cout << "Found!" << std::endl;
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}
		else {
			//file is another format containing only RGB values and no alpha values
			//we want to use the RGB option
			std::cout << "Not Found!" << std::endl;
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}

		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
}

void Texture::Bind()
{
	glBindTexture(GL_TEXTURE_2D, texture);
}



