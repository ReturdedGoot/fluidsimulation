#include "line.h"

Line::Line(glm::vec3 start, glm::vec3 end) {
	//we add the starting vertices to the vertices array
	vertices.push_back(start.x);
	vertices.push_back(start.y);
	vertices.push_back(start.z);
	vertices.push_back(end.x);
	vertices.push_back(end.y);
	vertices.push_back(end.z);

	//call the init buffers method
	initBuffers();
}


void Line::initBuffers() {
	vao.Bind();
	vbo = VBO(&vertices.front(), sizeof(GLfloat) * vertices.size());
	vao.LinkAttrib(vbo, 0, 3, GL_FLOAT, 6 * sizeof(GLfloat), (void*)0);
	
	vao.Unbind();
	vbo.Unbind();
}

void Line::DrawLine() {
	vao.Bind();
	glDrawArrays(GL_LINES, 0, 2);
	vao.Unbind();
}