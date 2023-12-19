#ifndef HEADER_CLASS_H
#define HEADER_CLASS_H
#include<glad/glad.h>

class EBO
{
public:
	GLuint id;
	bool multiple;
	EBO();
	EBO(GLuint* vertices, GLsizeiptr size, bool Multiple);

	void Bind();
	void Unbind();
	void Delete();


};


#endif // !HEADER_CLASS_H
