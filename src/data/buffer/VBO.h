//
// Created by roagen on 07.09.2021.
//

#ifndef UNTITLED4_VBO_H
#define UNTITLED4_VBO_H



#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec3 color;
    glm::vec2 texUV;
};


class VBO {
public:
    GLuint ID;
    VBO(GLfloat* vertices, GLsizeiptr size);
    VBO(std::vector<Vertex>& vertices);

    void Bind();
    void Unbind();
    void Delete();
};


#endif //UNTITLED4_VBO_H
