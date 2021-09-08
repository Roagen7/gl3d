//
// Created by roagen on 08.09.2021.
//

#ifndef UNTITLED4_MODEL_H
#define UNTITLED4_MODEL_H


#include "../shader/Shader.h"
#include "../spatial/camera/Camera.h"
#include "../../include/json/json.hpp"


using json = nlohmann::json;
class Model {
public:
    Model(const char* file);
    void Draw(Shader &shader, Camera& camera);
private:
    const char* file;
    std::vector<unsigned char> data;
    json JSON;

    std::vector<unsigned char> getData();
    std::vector<float> getFloats(json accessor);
    std::vector<GLuint> getIndices(json accessor);

    std::vector<glm::vec2> groupFloatsVec2(std::vector<float> floatVec);
    std::vector<glm::vec3> groupFloatsVec3(std::vector<float> floatVec);
};


#endif //UNTITLED4_MODEL_H
