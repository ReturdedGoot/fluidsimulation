#pragma once
#include<glad/glad.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<vector>
#include<math.h>

#include "VBO.h"
#include "VAO.h"

class Line {
public:
	std::vector<GLfloat> vertices;

	VBO vbo;
	VAO vao;

	
	Line(glm::vec3 start, glm::vec3 end);
	void DrawLine();
	void initBuffers();

};