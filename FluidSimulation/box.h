#pragma once

#include<glad/glad.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<vector>
#include<math.h>

#include "VBO.h"
#include "VAO.h"
#include "EBO.h"

class Box {
public:

	std::vector<GLfloat> vertices;
	std::vector<GLuint> indices;

	VBO vbo;
	VAO vao;
	EBO ebo;

	float height;
	float width;
	float length;

	Box(float height, float width, float length);
	void DrawBox();
	void initBuffer();
	void updateBox();
};