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

int checkBack(std::vector<Vertex> prev_vs, Vertex curr_v);
class Mesh {
public:
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<Texture> textures;
    glm::mat4 model = glm::mat4(1.0f);
    glm::vec3 translation = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);


    VAO vao;
    Mesh();
    Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, std::vector<Texture>& textures,  glm::vec3 color = {1.0f, 1.0f, 1.0f});
    Mesh(std::vector<Vertex>& vertices, std::vector<Texture>& textures,  glm::vec3 color = {1.0f, 1.0f, 1.0f});
    void Draw(Shader& shader,
              Camera& camera,
              glm::mat4 matrix,
              glm::vec3 translation = {0.0,0.0,0.0},
              glm::quat rotation = {1.0f, 0.0f, 0.0f, 0.0f},
              glm::vec3 scale = {1.0f,1.0f,1.0f});
    void DrawNoEl(Shader& shader, Camera& camera, glm::mat4 matrix);

    static void fromObjFile(std::string filename, std::vector<Vertex> &vs, std::vector<GLuint> &ids, bool hasTexture = false);
};


#endif //UNTITLED4_MESH_H
