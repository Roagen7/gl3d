//
// Created by roagen on 07.09.2021.
//

#ifndef UNTITLED4_VAO_H
#define UNTITLED4_VAO_H

#include <glad/glad.h>

#include "buffer/VBO.h"
#include "buffer/EBO.h"

class VAO {
public:
    GLuint ID;
    VAO();
    void LinkAttribs(VBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset);

    void Bind();
    void Unbind();
    void Delete();
};


#endif //UNTITLED4_VAO_H
