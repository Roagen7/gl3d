//
// Created by roagen on 07.09.2021.
//

#ifndef UNTITLED4_VBO_H
#define UNTITLED4_VBO_H

#include <glad/glad.h>
class VBO {
public:
    GLuint ID;
    VBO(GLfloat* vertices, GLsizeiptr size);

    void Bind();
    void Unbind();
    void Delete();
};


#endif //UNTITLED4_VBO_H
