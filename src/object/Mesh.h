//
// Created by roagen on 07.09.2021.
//

#ifndef UNTITLED4_MESH_H
#define UNTITLED4_MESH_H

#include "../data/VAO.h"
#include "../data/texture/Texture.h"
#include "../spatial/camera/Camera.h"
#include <string>
#include <vector>


class Mesh {
public:
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<Texture> textures;

    VAO vao;
    Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, std::vector<Texture>& textures);
    void Draw(Shader& shader, Camera& camera);
};


#endif //UNTITLED4_MESH_H
