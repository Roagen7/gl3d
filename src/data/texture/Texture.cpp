//
// Created by roagen on 07.09.2021.
//

#include "Texture.h"
#include "../../../include/stb/stb_image.h"

Texture::Texture(const char *image, GLenum texType, GLenum slot, GLenum format, GLenum pixelType, bool hasAlpha) {
    int widthImg, heightImg, numColCh;
    type = texType;

    //LOAD THE TEXTURE SOURCE IMAGE FLIPPED UPSIDE DOWN
    stbi_set_flip_vertically_on_load(true);
    unsigned char* bytes = stbi_load(image, &widthImg, &heightImg, &numColCh, 0);

    //generate texture and set it to be active on specified slot, also bind it to the type
    glGenTextures(1, &ID);
    glActiveTexture(slot);
    glBindTexture(texType, ID);


    //SETTINGS
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexImage2D(texType, 0 ,GL_RGB, widthImg, heightImg, 0, format, pixelType, bytes);
    glGenerateMipmap(texType);

    // free the memory and unbind the texture
    stbi_image_free(bytes);
    glBindTexture(texType, 0);
}

void Texture::texUnit(Shader& shader, const char *uniform, GLuint unit) {
    // specify the tex0

    GLuint texUni = glGetUniformLocation(shader.ID, uniform);
    shader.Activate();
    glUniform1i(texUni, unit);
}

void Texture::Bind() {
    glBindTexture(type, ID);
}

void Texture::Unbind() {
    glBindTexture(type, 0);
}

void Texture::Delete() {
    glDeleteTextures(1,&ID);
}