//
// Created by roagen on 07.09.2021.
//

#ifndef UNTITLED4_EBO_H
#define UNTITLED4_EBO_H

#include <glad/glad.h>

class EBO {
public:
    GLuint ID;
    EBO(GLuint* indices, GLsizeiptr size);

    void Bind();
    void Unbind();
    void Delete();
};


#endif //UNTITLED4_EBO_H
