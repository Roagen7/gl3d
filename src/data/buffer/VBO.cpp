//
// Created by roagen on 07.09.2021.
//

#include "VBO.h"

VBO::VBO(GLfloat *vertices, GLsizeiptr size) {
    glGenBuffers(1,&ID);
    glBindBuffer(GL_ARRAY_BUFFER, ID);
    glBufferData(GL_ARRAY_BUFFER,size, vertices, GL_STATIC_DRAW); //tell graphics card that the data won't be frequently modified
}

void VBO::Bind() {
    glBindBuffer(GL_ARRAY_BUFFER,ID);
}

void VBO::Unbind() {
    glBindBuffer(GL_ARRAY_BUFFER,0);
}

void VBO::Delete() {
    glDeleteBuffers(1, &ID);
}
