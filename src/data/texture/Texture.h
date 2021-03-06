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
    const char* type;
    GLuint unit;
    Texture(const char* image, const char* texType, GLuint slot);

    void texUnit(Shader& shader, const char* uniform, GLuint unit);
    void Bind();
    void Unbind();
    void Delete();

};


#endif //UNTITLED4_TEXTURE_H
