//
// Created by roagen on 07.09.2021.
//

#include <array>
#include <memory>
#include "Shader.h"



std::string exec(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}


std::string get_file_contents(const char* filename) {
    std::ifstream in(filename, std::ios::binary);
    if (in)
    {
        std::string contents;
        in.seekg(0, std::ios::end);
        contents.resize(in.tellg());
        in.seekg(0, std::ios::beg);
        in.read(&contents[0], contents.size());
        in.close();
        return(contents);
    }
    throw(errno);

}

Shader::Shader(const char *vertexFile, const char *fragmentFile) {
    std::string vertexCode = get_file_contents(vertexFile);
    std::string fragmentCode = get_file_contents(fragmentFile);




    const char* vertexSource = vertexCode.c_str();
    const char* fragmentSource = fragmentCode.c_str();

    //compile the shaders

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER); //create shader object
    glShaderSource(vertexShader, 1, &vertexSource, nullptr); //assign to the object the source code
    glCompileShader(vertexShader); //compile the object to machine code
    this->compileErrors(vertexShader, "VERTEX");


    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader,1,&fragmentSource, nullptr);
    glCompileShader(fragmentShader);
    this->compileErrors(fragmentShader, "FRAGMENT");


    // program for graphics pipeline, called each time an object is being rendered
    ID = glCreateProgram();

    //attach and link vertex and fragment shaders to program
    glAttachShader(ID,vertexShader);
    glAttachShader(ID, fragmentShader);
    glLinkProgram(ID);
    this->compileErrors(ID, "PROGRAM");

    //garbage collection
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);




}

void Shader::Activate() {
    glUseProgram(ID);
}

void Shader::Delete() {
    glDeleteProgram(ID);
}

void Shader::compileErrors(unsigned int shader, const char *type) {
    GLint hasCompiled;
    char infoLog[1024];
    if(type != "PROGRAM"){
        glGetShaderiv(shader,GL_COMPILE_STATUS, &hasCompiled);
        if(hasCompiled == GL_FALSE){
            glGetShaderInfoLog(shader,1024, NULL, infoLog);
            std::cout << "SHADER_COMPILATION_ERROR::" << type << "\n" << std::endl;
        }
    } else {
        glGetProgramiv(shader, GL_COMPILE_STATUS, &hasCompiled);
        if(hasCompiled == GL_FALSE){
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "SHADER_LINKING_ERROR::" << type << "\n" << std::endl;
        }

    }
}
