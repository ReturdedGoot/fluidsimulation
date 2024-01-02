#include "box.h"


Box::Box(float height, float width, float length) {
	//we want to generate the vertices for a box 

	//x-axis is width
	//y-axis is height
	//z-axis is length

	//create the vertices
	std::vector<GLfloat> vertices = {
		 width / 2,  height / 2,  length / 2, //front top right    - 0
		 width / 2, -height / 2,  length / 2, //front bottom right - 1
		-width / 2,  height / 2,  length / 2, //front top left     - 2
		-width / 2, -height / 2,  length / 2, //front bottom left  - 3
		 width / 2,  height / 2, -length / 2, //back top right     - 4
		 width / 2, -height / 2, -length / 2, //back bottom right  - 5
		-width / 2,  height / 2, -length / 2, //back top left      - 6
		-width / 2, -height / 2, -length / 2, //back bottom left   - 7

	};

	
	std::vector<GLuint> indices = {
		7, 6, 5, //back face
		4, 5, 6,

		// bottom face
		7, 3, 5,
		3, 5, 1,

		//top face
		0, 2, 4,
		2, 4, 6,

		//right face
		0, 1, 5,
		0, 5, 4,

		//left face
		7, 3, 2,
		7, 2, 6,

		//front face
		0, 1, 2, 
		1, 2, 3
	};

	//resize the box so that it matches the dimensions specified
	

	this->vertices = vertices;
	this->indices = indices;

	this->height = height;
	this->width = width;
	this->length = length;

	initBuffer();
}




void Box::initBuffer() {
	vao.Bind();
	vbo = VBO(&vertices.front(), sizeof(GLfloat)*vertices.size());
	ebo = EBO(&indices.front(), sizeof(GLuint)*indices.size(), false);
	ebo.Bind();

	//we want to link the attributes
	vao.LinkAttrib(vbo, 0, 3, GL_FLOAT, 3 * sizeof(GLfloat), (void*)0); 


	//we want to now unbind all the elements
	vao.Unbind();
	vbo.Unbind();
	ebo.Unbind();
}


void Box::updateBox() {
	vertices = {
		 width / 2,  height / 2,  length / 2, //front top right    - 0
		 width / 2, -height / 2,  length / 2, //front bottom right - 1
		-width / 2,  height / 2,  length / 2, //front top left     - 2
		-width / 2, -height / 2,  length / 2, //front bottom left  - 3
		 width / 2,  height / 2, -length / 2, //back top right     - 4
		 width / 2, -height / 2, -length / 2, //back bottom right  - 5
		-width / 2,  height / 2, -length / 2, //back top left      - 6
		-width / 2, -height / 2, -length / 2, //back bottom left   - 7
	};

	//we can 
	vbo.update(&vertices.front(), sizeof(GLfloat) * vertices.size());

}




void Box::DrawBox() {
	vao.Bind();
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	vao.Unbind();
}