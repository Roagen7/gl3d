//
// Created by roagen on 07.09.2021.
//

#ifndef UNTITLED4_SHADER_H
#define UNTITLED4_SHADER_H

#include<string>
#include<fstream>
#include<sstream>
#include<iostream>
#include<cerrno>
#include <glad/glad.h>

std::string exec(const char* cmd);
std::string get_file_contents(const char* filename);

class Shader {
public:
    GLuint ID;
    Shader(const char* vertexFile, const char* fragmentFile);

    void Activate();
    void Delete();

    void compileErrors(unsigned int shader, const char* type);
};




#endif //UNTITLED4_SHADER_H
