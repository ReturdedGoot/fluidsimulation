#define _USE_MATH_DEFINES

#include "sphere.h"
#include <cmath>
#include <iostream>

Sphere::Sphere(int radius, int latitudes, int longitudes)
{
    float sectorCount = latitudes;
    float stackCount = longitudes;

    float x, y, z, xy;                              // vertex position
    float nx, ny, nz, lengthInv = 1.0f / radius;    // vertex normal
    float s, t;                                     // vertex texCoord

    float sectorStep = 2 * (float) M_PI / sectorCount;
    float stackStep = (float) M_PI / stackCount;
    float sectorAngle, stackAngle;

    for (int i = 0; i <= stackCount; ++i)
    {
        stackAngle = (float) M_PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
        xy = radius * cosf(stackAngle);             // r * cos(u)
        z = radius * sinf(stackAngle);              // r * sin(u)

        // add (sectorCount+1) vertices per stack
        // first and last vertices have same position and normal, but different tex coords
        for (int j = 0; j <= sectorCount; ++j)
        {
            sectorAngle = j * sectorStep;           // starting from 0 to 2pi

            // vertex position (x, y, z)
            x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
            y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);

            // normalized vertex normal (nx, ny, nz)
            nx = x * lengthInv;
            ny = y * lengthInv;
            nz = z * lengthInv;
            normals.push_back(nx);
            normals.push_back(ny);
            normals.push_back(nz);

            // vertex tex coord (s, t) range between [0, 1]
            s = (float)j / sectorCount;
            t = (float)i / stackCount;
            //texCoords.push_back(s);
            //texCoords.push_back(t);
        }
    }


    // generate CCW index list of sphere triangles
// k1--k1+1
// |  / |
// | /  |
// k2--k2+1
   
    std::vector<int> lineIndices;
    int k1, k2;
    for (int i = 0; i < stackCount; ++i)
    {
        k1 = i * (sectorCount + 1);     // beginning of current stack
        k2 = k1 + sectorCount + 1;      // beginning of next stack

        for (int j = 0; j < sectorCount; ++j, ++k1, ++k2)
        {
            // 2 triangles per sector excluding first and last stacks
            // k1 => k2 => k1+1
            if (i != 0)
            {
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
            }

            // k1+1 => k2 => k2+1
            if (i != (stackCount - 1))
            {
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }

            // store indices for lines
            // vertical lines for all stacks, k1 => k2
            lineIndices.push_back(k1);
            lineIndices.push_back(k2);
            if (i != 0)  // horizontal lines except 1st stack, k1 => k+1
            {
                lineIndices.push_back(k1);
                lineIndices.push_back(k1 + 1);
            }
        }
    }


    //Call the init method
    initBuffers();
}


void Sphere::initBuffers() {
    //Generate the vertex array for the vbo.
    for (int i = 0; i < vertices.size() - 3; i += 3) {
        vboArray.push_back(vertices[i]);
        vboArray.push_back(vertices[i + 1]);
        vboArray.push_back(vertices[i + 2]);

        vboArray.push_back(normals[i]);
        vboArray.push_back(normals[i + 1]);
        vboArray.push_back(normals[i + 2]);
    }


    //we initialize the VAO, VBO and EBO's
    vao.Bind();
    vbo = VBO(&vboArray.front(), sizeof(GLfloat) * vboArray.size());
    ebo = EBO(&indices.front(), sizeof(GLuint) * indices.size(), true);
    ebo.Bind();
    

    //Link attributes to VAO
    vao.LinkAttrib(vbo, 0, 3, GL_FLOAT, 6 * sizeof(GLfloat), (void*)0); //VBO-1 to VAO1
    vao.LinkAttrib(vbo, 1, 3, GL_FLOAT, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat))); //VBO-1 to VAO1

    //we can now unbind everything;
    vao.Unbind();
    vbo.Unbind();
    ebo.Unbind();
}

void Sphere::DrawSphere() {
    //Drawing the sphere is simple
    vao.Bind();
    glDrawElements(GL_TRIANGLES, 2 * indices.size(), GL_UNSIGNED_INT, 0);
    vao.Unbind();
}