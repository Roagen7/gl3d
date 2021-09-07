//
// Created by roagen on 07.09.2021.
//

#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex> &vertices, std::vector<GLuint> &indices, std::vector<Texture> &textures) {
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;

    vao.Bind();

    //create vbo and ebo for vao
    auto vbo = VBO(vertices);
    auto ebo = EBO(indices);

    //link vbo attributes to vao
    vao.LinkAttribs(vbo, 0, 3, GL_FLOAT, 11 * sizeof(float), (void*)nullptr); // pos
    vao.LinkAttribs(vbo, 1, 3, GL_FLOAT, 11 * sizeof(float), (void*) (3 * sizeof(float))); //normals
    vao.LinkAttribs(vbo, 2, 3, GL_FLOAT, 11 * sizeof(float), (void*) (6 * sizeof(float))); //colors
    vao.LinkAttribs(vbo, 3, 2 , GL_FLOAT, 11 * sizeof(float), (void*) (9 * sizeof(float))); // textures

    vao.Unbind();
    vbo.Unbind();
    ebo.Unbind();


}

void Mesh::Draw(Shader &shader, Camera &camera) {
    shader.Activate();
    vao.Bind();

    uint numDiffuse = 0;
    uint numSpecular = 0;

    for(uint i = 0; i < textures.size(); i++){
        std::string num;
        std::string type = textures[i].type;
        if(type == "diffuse"){
            num = std::to_string(numDiffuse++);
        } else if(type == "specular"){
            num = std::to_string(numSpecular++);
        }

        textures[i].texUnit(shader,(type + num).c_str(), i);
        textures[i].Bind();
    }

    glUniform3f(glGetUniformLocation(shader.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
    camera.Matrix(shader, "camMatrix");
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}
