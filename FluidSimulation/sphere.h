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


#define M_PI  3.14159265358979323846 


class Sphere {
public:

    std::vector<float> vertices;
    std::vector<float> normals;
    std::vector<float> uv;
    std::vector<unsigned int> indices;
    std::vector<float> vboArray;

    VBO vbo;
    VAO vao;
    EBO ebo;


    Sphere(int radius, int latitudes, int longitudes);
    void DrawSphere();
    void initBuffers();
};