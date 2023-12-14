#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include<glad/glad.h>
#include<string>
#include<fstream>
#include<sstream>
#include<iostream>
#include<cerrno>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

std::string get_file_contents(const char* filename);

class Shader
{
	public:
		GLuint id;
		Shader(const char* vertexFile, const char* fragmentFile);

		void Activate();
		void Delete();

		void setBool(const std::string& name, bool value);
		void setInt(const std::string& name, int value);
		void setFloat(const std::string& name, float value);
		void setMat4(const std::string& name, glm::mat4 matrix);

};

#endif