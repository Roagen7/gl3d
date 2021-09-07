//
// Created by roagen on 07.09.2021.
//

#ifndef UNTITLED4_TEXTURE_H
#define UNTITLED4_TEXTURE_H

#include <glad/glad.h>
#include "../../shader/Shader.h"

class Texture {
public:
    GLuint ID;
    GLenum type;
    Texture(const char* image, GLenum texType, GLenum slot, GLenum format, GLenum pixelType, bool hasAlpha = false);

    void texUnit(Shader& shader, const char* uniform, GLuint unit);
    void Bind();
    void Unbind();
    void Delete();

};


#endif //UNTITLED4_TEXTURE_H
