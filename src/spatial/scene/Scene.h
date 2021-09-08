//
// Created by roagen on 08.09.2021.
//

#ifndef UNTITLED4_SCENE_H
#define UNTITLED4_SCENE_H


#include "../../object/Mesh.h"
#include "../../object/material/Material.h"
#include "node/Node.h"

class Scene {
public:
    std::vector<Mesh> meshes;
    std::vector<Material> materials;
    Node rootNode;

//    Scene(meshes, materials, rootNode);
};


#endif //UNTITLED4_SCENE_H
