//
// Created by roagen on 07.09.2021.
//

#include <strstream>
#include "Mesh.h"
Mesh::Mesh()= default;
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

void Mesh::Draw(Shader &shader, Camera &camera,  glm::mat4 matrix,
                glm::vec3 translation,
                glm::quat rotation,
                glm::vec3 scale) {
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

    auto trans = glm::mat4(1.0f);
    auto rot = glm::mat4(1.0f);
    auto sca = glm::mat4(1.0f);

    trans = glm::translate(trans, translation);
    rot = glm::mat4_cast(rotation);
    sca = glm::scale(sca,scale);

    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "translation"), 1, GL_FALSE, glm::value_ptr(trans));
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "rotation"), 1, GL_FALSE, glm::value_ptr(rot));
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "scale"), 1, GL_FALSE, glm::value_ptr(sca));
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(matrix));

    glUniform3f(glGetUniformLocation(shader.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);
//    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));
    camera.Matrix(shader, "camMatrix");
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}

Mesh Mesh::fromObjFile(std::string filename) {
    std::ifstream input(filename);
    if(!input.is_open()){
        std::cout << "[ERROR] COULDN'T OPEN FILE " + filename << std::endl;
        return {};
    }
//
//    int lineCount = 1;
//
//    std::vector<Vertex> vertices;
//
//
//
//
//    std::vector<sf::Vector3<double>> vs;
//    std::vector<sf::Vector2<double>> ts;
//
//    while(!input.eof()){
//        char x[128];
//        input.getline(x, 128);
//        std::strstream l;
//        l << x;
//        char trash;
//
//        if(x[0] == 'v' && x[1] != 'n' && x[1] != 't'){
//            sf::Vector3<double> v;
//            l >> trash >> v.x >> v.y >> v.z;
//            vs.push_back(v);
//
//        } else if(x[0] == 'v' && x[1] == 't'){
//
//
//            sf::Vector2<double> t;
//            l >> trash >>trash>> t.x >> t.y;
//
//
////            t.x = 1.0 - t.x;
//            t.y = 1.0 - t.y;
//            ts.push_back(t);
//
//        } else if(x[0] == 'f' ){
//                l >> trash;
//
//                std::string tokens[6];
//                int tokenCount = -1;
//
//
//                while (!l.eof())
//                {
//
//                    char c = l.get();
////                    std::cout << "tuta" << std::endl;
//                    if (c == ' ' || c == '/')
//                        tokenCount++;
//                    else
//                        tokens[tokenCount].append(1, c);
//                }
//
//
//                tokens[tokenCount].pop_back();
//
//                //            std::cout << ts[stoi(tokens[1]) - 1].x << std::endl;
//                //            std::cout << tokens[0] <<"/" << tokens[1] <<" " << tokens[2] << "/" << tokens[3] << " " <<tokens[4] <<  "/"<<tokens[5] << std::endl;
//
//
//
//                m.triangles.emplace_back( vs[stoi(tokens[0]) - 1], vs[stoi(tokens[2]) - 1], vs[stoi(tokens[4]) - 1],
//                                          ts[stoi(tokens[1]) - 1], ts[stoi(tokens[3]) - 1], ts[stoi(tokens[5]) - 1]);
//
//            }
//        }
//        lineCount++;
//    }
////    std::cout << ts.size() << std::endl;
//    return m;
//
//
//
//
//    return {};
}
