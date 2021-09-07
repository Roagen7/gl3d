//
// Created by roagen on 07.09.2021.
//

#include "VAO.h"

VAO::VAO() {
    glGenVertexArrays(1, &ID);
}

void VAO::LinkAttribs(VBO& VBO, GLuint layout,  GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset) {
    VBO.Bind();
    glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset); //layout (location = 0) == first argument
    glEnableVertexAttribArray(layout);
    VBO.Unbind();

}

void VAO::Bind() {
    glBindVertexArray(ID);
}

void VAO::Unbind() {
    glBindVertexArray(0);
}

void VAO::Delete() {
    glDeleteVertexArrays(1,&ID);
}