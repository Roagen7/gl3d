//
// Created by roagen on 07.09.2021.
//

#include "Texture.h"
#include "../../../include/stb/stb_image.h"

Texture::Texture(const char *image, const char* texType, GLuint slot) {
    int widthImg, heightImg, numColCh;
    type = texType;

    //LOAD THE TEXTURE SOURCE IMAGE FLIPPED UPSIDE DOWN
    stbi_set_flip_vertically_on_load(true);
    unsigned char* bytes = stbi_load(image, &widthImg, &heightImg, &numColCh, 0);
    std::cout << widthImg << std::endl;
    //generate texture and set it to be active on specified slot, also bind it to the type
    glGenTextures(1, &ID);
    glActiveTexture(GL_TEXTURE0 + slot);
    unit = slot;
    glBindTexture(GL_TEXTURE_2D, ID);


    //SETTINGS
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    if(numColCh == 4) glTexImage2D(GL_TEXTURE_2D, 0 ,GL_RGBA, widthImg, heightImg, 0, GL_RGBA, GL_UNSIGNED_BYTE, bytes);
    else if(numColCh == 3) glTexImage2D(GL_TEXTURE_2D, 0 ,GL_RGBA, widthImg, heightImg, 0, GL_RGB, GL_UNSIGNED_BYTE, bytes);
    else if(numColCh == 1) glTexImage2D(GL_TEXTURE_2D, 0 ,GL_RGBA, widthImg, heightImg, 0, GL_RED, GL_UNSIGNED_BYTE, bytes);
    else throw std::invalid_argument("Texture type recognition failed");
//    glTexImage2D(GL_TEXTURE_2D, 0 ,GL_RGB, widthImg, heightImg, 0, format, pixelType, bytes);
    glGenerateMipmap(GL_TEXTURE_2D);

    // free the memory and unbind the texture
    stbi_image_free(bytes);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::texUnit(Shader& shader, const char *uniform, GLuint unit) {
    // specify the tex0
    //uniform = diffuse0 / specular0
    GLuint texUni = glGetUniformLocation(shader.ID, uniform);
    shader.Activate();
    glUniform1i(texUni, unit);
}

void Texture::Bind() {
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, ID);
}

void Texture::Unbind() {
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Delete() {
    glDeleteTextures(1,&ID);
}