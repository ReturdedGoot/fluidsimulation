#ifndef HEADER_CLASS_H
#define HEADER_CLASS_H
#include<glad/glad.h>

class EBO
{
public:
	GLuint id;
	EBO(GLuint* vertices, GLsizeiptr size);

	void Bind();
	void Unbind();
	void Delete();


};


#endif // !HEADER_CLASS_H
