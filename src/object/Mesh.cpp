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
//        std::cout << textures[i].type << std::endl;
        textures[i].texUnit(shader,(type + num).c_str(), i);
//        std::cout << type + num << std::endl;
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





void Mesh::fromObjFile(std::string filename, std::vector<Vertex> &vs, std::vector<GLuint> &ids, bool hasTexture)  {
    std::ifstream input(filename);
    if(!input.is_open()){
        std::cout << "[ERROR] COULDN'T OPEN FILE " + filename << std::endl;
        return ;
    }
//
//    int lineCount = 1;
//
    std::vector<glm::vec3> cachedPos;
    std::vector<glm::vec3> cachedNorm;
    std::vector<glm::vec2> cachedTexs;

    while(!input.eof()){
        char x[128];
        char trash;
        std::strstream l;

        input.getline(x,128);
        l << x;
        if(x[0] == 'v'){
            if(x[1] == 'n'){
                //normal
                glm::vec3 v;
                l >> trash >> v.x >> v.y >> v.z;
                cachedNorm.push_back(v);
            } else if(x[1] == 't'){
                //texture
                glm::vec2 v;
                l >> trash >> trash >> v.y >> v.x;
                std::cout << v.x <<" " << v.y << std::endl;
                v.x = 1 - v.x;
//                v.y = 1 - v.y;
                cachedTexs.push_back(v);
            } else {
                //position
                glm::vec3 v;
                l >> trash >> v.x >> v.y >> v.z;
                cachedPos.push_back(v);
            }

        } else if(x[0] == 'f'){
            //face
            l >> trash;
            std::string tokens[9];
            int tokenCount = -1;


            while (!l.eof()){

                char c = l.get();

                if (c == ' ' || c == '/')
                    tokenCount++;
                else
                    tokens[tokenCount].append(1, c);
            }

            //v1/vt1/vn1
            tokens[tokenCount].pop_back();
            //v1[tokens[0] - 1]
            glm::vec3 pos1 = cachedPos[stoi(tokens[0]) - 1];
            glm::vec3 pos2 = cachedPos[stoi(tokens[3]) - 1];
            glm::vec3 pos3 = cachedPos[stoi(tokens[6]) - 1];

            glm::vec2 tex1 = cachedTexs[stoi(tokens[1]) - 1];
            glm::vec2 tex2 = cachedTexs[stoi(tokens[4]) - 1];
            glm::vec2 tex3 = cachedTexs[stoi(tokens[7]) - 1];

            glm::vec3 norm1 = cachedNorm[stoi(tokens[2]) - 1];
            glm::vec3 norm2 = cachedNorm[stoi(tokens[5]) - 1];
            glm::vec3 norm3 = cachedNorm[stoi(tokens[8]) - 1];

            Vertex v1{}, v2{}, v3{};
            v1.position = pos1; v1.normal = norm1; v1.color = {1.0, 1.0, 1.0}; v1.texUV = tex1;
            v2.position = pos2; v2.normal = norm2; v2.color = {1.0, 1.0, 1.0}; v2.texUV = tex2;
            v3.position = pos3; v3.normal = norm3; v3.color = {1.0, 1.0, 1.0}; v3.texUV = tex1;

            vs.push_back(v1);
            ids.push_back(vs.size() - 1);
            vs.push_back(v2);
            ids.push_back(vs.size() - 1);
            vs.push_back(v3);
            ids.push_back(vs.size() - 1);
        }




    }


}
